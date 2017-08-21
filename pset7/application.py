from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp
from datetime import datetime

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    # the user's holdings
    holdings = db.execute("SELECT symbol, shares \
                            FROM portfolio WHERE id = :id", \
                            id=session["user_id"])
    
    # initialize total value                                
    total_value = 0
    for s in holdings:
        symbol = s["symbol"]
        shares = s["shares"]
        stock = lookup(symbol) 
        price = stock["price"]
        total = shares * price
        total_value += total
        db.execute("UPDATE portfolio SET price = :price, \
                    total = :total WHERE id = :id AND symbol = :symbol", \
                    price=usd(price), total=usd(total), \
                    id=session["user_id"], symbol=symbol)
        
    # remaining cash    
    cash_remain = db.execute("SELECT cash FROM users \
                               WHERE id=:id", id=session["user_id"])
    
    # add remaining cash to stock value for total value
    total_value += cash_remain[0]["cash"]    
    
    updated_portfolio = db.execute("SELECT * FROM portfolio WHERE id = :id", \
                                    id=session["user_id"])
                               
    return render_template("index.html", stocks=updated_portfolio, \
                            cash=usd(cash_remain[0]["cash"]), total=usd(total_value))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # ensure user input stock ticker
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("please provide stock symbol/number of shares")
        
        # check if invalid stock symbol
        stock = lookup(request.form.get("symbol"))
        if stock == None:
            return apology("invalid stock symbol")    
        
        # ensure valid share input
        try:
            shares = int(request.form.get("shares"))
            if shares < 0:
                return apology("invalid number of shares")
        except:
            return apology("invalid number of shares")
            
        # check if have enough cash to buy 
        row = db.execute("SELECT cash FROM users WHERE id = :id", \
                        id=session["user_id"])
        cash = row[0]["cash"]
        cost = stock["price"] * shares
        if not row or cash < cost:
            return apology("not enough cash")
        else:
            # add transaction to history table
            db.execute("INSERT INTO histories (id, time, symbol, shares, price, buy) \
                    VALUES(:id, :time, :symbol, :shares, :price, :buy)",  \
                        id=session["user_id"], \
                        time=str(datetime.now().strftime('%Y-%m-%d')), \
                        symbol=stock["symbol"], shares=shares, \
                        price=stock["price"], buy="Buy")
                    
            # update user's cash in users table
            db.execute("UPDATE users SET cash = cash - :cost WHERE id = :id", \
                        cost=cost, id=session["user_id"])
                        
            # update user's portfolio 
            user_shares = db.execute("SELECT shares FROM portfolio \
                           WHERE id = :id AND symbol = :symbol", \
                           id=session["user_id"], symbol=stock["symbol"])
                           
            # create new if user doesn't have shares of that stock
            if not user_shares:
                db.execute("INSERT INTO portfolio (id, symbol, name, shares) \
                            VALUES(:id, :symbol, :name, :shares)", \
                            id=session["user_id"], symbol=stock["symbol"], name=stock["name"], shares=shares) 
                        
            # if user already have shares of that stock, change shares
            else:
                shares_update = user_shares[0]["shares"] + shares
                db.execute("UPDATE portfolio SET shares=:shares \
                            WHERE id=:id AND symbol=:symbol", \
                            shares=shares_update, id=session["user_id"], \
                            symbol=stock["symbol"])
                        
        return redirect(url_for("index"))
    
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")            


@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    histories = db.execute("SELECT * from histories WHERE id=:id", id=session["user_id"])
    
    return render_template("history.html", histories=histories)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # ensure user input stock ticker
        if not request.form.get("symbol"):
            return apology("please provide stock symbol")
        
        # look up quote for a symbol    
        quote = lookup(request.form.get("symbol"))
        
        # render quote if stock symbol is valid
        if quote != None:
            return render_template("quoted.html", quote=quote)
        else:
            return apology("invalid stock symbol")
    
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user. username: cs50 password: 50"""
    
    # forget any user_id
    session.clear()
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # ensure username was submitted
        if not request.form.get("username"):
            return apology("please provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("please provide password")
        
        # ensure password match
        elif not request.form.get("password") != request.form.get("password_2"):
            return apology("password does not match")
        
        # insert new user into users    
        result = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", \
        username=request.form.get("username"), hash=pwd_context.hash(request.form.get("password")))
        
        # ensure username does not exist 
        if not result:
            return apology("username already exist")
        
        # log in the user and redirect user to home page
        session["user_id"] = result
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")
    

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # ensure user input stock symbol
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("please provide stock symbol/number of shares")
        
        stock = lookup(request.form.get("symbol"))
        if stock == None:
            return apology("invalid stock symbol")
            
        shares = int(request.form.get("shares"))
        if shares < 0:
            return apology("invalid shares")
        
        # check if stock is available to sell
        available_share = db.execute("SELECT symbol, shares FROM portfolio \
                                        WHERE id = :id AND symbol = :symbol", \
                                        id=session["user_id"], symbol=stock["symbol"])
        if not available_share:
            return apology("do not hold this stock")
        elif available_share[0]["shares"] < shares:
            return apology("not enough shares to sell")
        else:
            # add transaction to history table 
            db.execute("INSERT INTO histories (id, time, symbol, shares, price, buy) \
                        VALUES (:id, :time, :symbol, :shares, :price, :buy)", \
                        id=session["user_id"], \
                        time=datetime.now().strftime('%Y-%m-%d'), \
                        symbol=stock["symbol"], shares=shares, \
                        price=stock["price"], buy='Sell')
            
            # update user's cash
            earning = stock["price"] * shares
            db.execute("UPDATE users SET cash = cash + :earning WHERE id = :id", \
                        earning=earning, id=session["user_id"])
            
            # update user's portfolio
            db.execute("UPDATE portfolio SET shares = shares - :shares \
                        WHERE id = :id AND symbol = :symbol", \
                        shares=shares, id=session["user_id"], symbol=stock["symbol"])
                        
            # drop rows with shares = 0 after sell
            db.execute("DELETE FROM portfolio WHERE shares = 0")
            
            # redirect user to home page
            return redirect(url_for("index"))
                
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("sell.html") 

