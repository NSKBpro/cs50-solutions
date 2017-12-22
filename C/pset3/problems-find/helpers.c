/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include "helpers.h"
#include <stdio.h>
#include <string.h>


/**
 * Returns true if value is in array of n values, else false. Algorithm: BINARY SEARCH
 */
bool search(int value, int values[], int n)
{
    if(n<0) return false; //If size of array is less than 0

    int first = 0, last = n-1; //On beginning first index is always 0 and last index is n-1 (because counter starts from 0)
    int mid = (first+last)/2; //Get middle element
    
    while(first<=last){
        if(values[mid] < value) //If number we are searching for is in right half
            first = mid + 1;
        else if (values[mid]==value) //If current middle number is number we are searching for
            return true;
        else
            last = mid - 1; //If number we are searching for is in left half
    
    mid = (first+last)/2; //Get new middle element
    
    }
    
    return false; //If element we are searching for is not in array
}

/**
 * Sorts array of n values. Algorithm: COUNTING SORT
 */
void sort(int values[],int n){

  int max = 0, curr = 0, num = 0;
  for(int i = 0;i<n;++i) { //Loop through all values 
    if(values[i] > max) //Checks if current value is bigger than max variable
      max=values[i]; //Sets max to new value
  }

  int * counting_array = calloc(max, sizeof(int)); //Initialize all elements at 0 using calloc, and allocate enough memory to have index of biggest value
  for(curr = 0; curr < n;++curr)
    counting_array[values[curr]]++; //Count number of each value
  
  curr = 0;

  while(curr <= n){ //while counter is less or equal to total number of elements
    while(counting_array[num] > 0){ //Check if amount of current number is greater than 0 (as we decrement this when we put it in array)
      values[curr] = num; //Put current number in sorted array
      counting_array[num]--; //Decrement amount of current number 
      curr++; //Increase counter
      if(curr > n){ break; } //If increased counter is greater than n, break from while loop
    }
    num++;
  }

  return;
}
