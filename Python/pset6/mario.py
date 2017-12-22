#!/usr/bin/python3

import sys

def draw(height):
    for i in range(height+1):
       numOfBlank = height - i
       for j in range(numOfBlank):
           print(" ", end='')
       for k in range(i):
           print("#", end='')
       print("  ", end='')   
       for l in range(i):
           print("#", end='')
       print("")       

def main():
    height = -1
    while height <= 0 or height > 23:
        try:        
            height = int(input("Height:"))
        except ValueError:
                print("Invalid input, you must enter number!")
                
    draw(height)
    
if __name__ == "__main__":
    main()