import nltk
import re

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""

        # load positive
        with open('positive-words.txt') as f:
            lines = f.readlines()[36:]
        self.positives = [word.strip('\n') for word in lines]
        
        # load negative words
        with open('negative-words.txt') as f:
            lines = f.readlines()[36:]
        self.negatives = [word.strip('\n') for word in lines]

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        # split text into words list, calculate total score by looking at each word
        self.score = 0
        words_list = [w for w in re.split(r"\W", text) if w]
        for word in words_list:
            if word in self.positives:
                self.score += 1
            elif word in self.negatives:
                self.score -= 1
                        
        return self.score
