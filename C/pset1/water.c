#include <stdio.h>
#include <cs50.h>

//Functions
int getMinutes();


int main(void){
 
   int minutes = getMinutes(); //Get user input
   int perMinute = 12;
   int total = minutes * perMinute;
   printf("Total bottles:%d\n",total);
 
}

int getMinutes(){
   int minutes;
   do{
      printf("Enter number of minutes:");
      minutes = get_int();
   }while(minutes<0);
   return minutes;
}