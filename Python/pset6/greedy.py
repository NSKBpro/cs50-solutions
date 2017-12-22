#!/usr/bin/python3

import sys

#Global variables
numberOfCoins = 0
change = 0

def processCoin(coin):
    global numberOfCoins
    global change
    while change-coin >=0:
        change -= coin
        numberOfCoins += 1
    return    

def countQuarter():
    processCoin(25) #Runs while we can use quarters
    countDime()
    return

def countDime():
    processCoin(10) #Runs while we can use dimes
    countNickel()
    return

def countNickel():
    processCoin(5) #Runs while we can use nickels
    countPenny()
    return

def countPenny():
    processCoin(1) #Runs while we can use pennies
    return

def main():
    global change 
    while True:
        try:
            print("Hi! How much change is owed?")
            change = round(float(input()) * 100)
            if change > 0:
                break
        except ValueError:
            print("Invalid input, you must enter number!")    
        
    countQuarter() 
    print(numberOfCoins)

if __name__ == "__main__":
    main()
    
