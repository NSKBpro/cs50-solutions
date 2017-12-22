import sys
import helpers
from analyzer import Analyzer

class TweetsStore(Analyzer):

    def __init__(self, username, positives, negatives):
        
        # Call superclass (Analyzer) constructor to init Analyzer
        super().__init__(positives,negatives)
        
        #Try to get tweets
        tweets = helpers.get_user_timeline(username)
        
        #Make sure that get_user_timeline method got tweets successfully
        if not tweets == None:
            #Add all tweets to set
             self.__tweetsCollection = set(tweets)
        else:
            sys.exit("Error while fetching tweets.")
        
        #Make sure that user have tweets
        if len(self.__tweetsCollection) == 0:
            sys.exit(username + " hasn't Tweeted yet.")
        
    #Getter for all tweets    
    def getTweets(self):
        return self.__tweetsCollection
        