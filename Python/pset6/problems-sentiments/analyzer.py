import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        
        #Init sets
        self.positives = set()
        self.negatives = set()
        
        #Read positive words from data set
        with open(positives) as words:
            for word in words:
                if not word.startswith(';'):
                    self.positives.add(word.strip('\n'))
        #Read negative words from data set            
        with open(negatives) as words:
            for word in words:
                if not word.startswith(';'):
                    self.negatives.add(word.strip('\n'))
                    
    def analyze(self, text):
        
        #Will hold sum of score for every word
        totalScore = 0
        
        #Tokenize words from argument string
        argWords = nltk.tokenize.TweetTokenizer().tokenize(text)
        
        #Check score of every word 
        for argWord in argWords:
            alreadyFound = False
            for positiveWord in self.positives:
                if argWord.lower() == positiveWord:
                    totalScore += 1
                    alreadyFound = True
                    break
            
            if not alreadyFound:   
                for negativeWord in self.negatives:
                    if argWord.lower() == negativeWord:
                        totalScore -= 1
                        break   
                
        return totalScore
