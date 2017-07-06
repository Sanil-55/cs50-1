#!/usr/bin/env python3

def main():
    """
    solution to CS50 problem set 6
    print out the minimum number of coins required to give a user change
    """
    change = float(input("O hai! How much change is owed?\n"))
    if change < 0:
        change = float(input("O hai! How much change is owed?\n"))
    print(calculate_change(change*100))
    
# 
def calculate_change(change):
    """
    @param: amount of change in cent
    @return: total number of coins (quarter, dime, nickel, penny)
    """
    coins = 0
    if change >= 25:
        coins += change // 25
        change = change % 25
    if change >= 10:
        coins += change // 10
        change = change % 10
    if change >= 5:
        coins += change // 5
        change = change % 5
    if change >= 1:
        coins += change
    return int(coins)
    
if __name__ == "__main__":
    main()