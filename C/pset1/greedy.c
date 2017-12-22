#include <stdio.h>
#include <cs50.h>
#include <math.h>

//Functions
float getInput();
void processCoin(int);
void countQuarter();
void countDime();
void countNickel();
void countPenny();

//Global variables
int numberOfCoins = 0;
int change = 0;

int main(void){
    change = (int)round((getInput() * 100)); //Gets user input, multiplies it by 100 to get cents, rounds it (precision issues when we don't round it), casts it to int
    countQuarter();
    printf("%d\n",numberOfCoins);
}

float getInput(){
    float input;
    do{
        printf("Hi! How much change is owed?\n");
        input = get_float();
    }while(input <=0);
    return input;
}

void processCoin(int coin){
    while((change-coin)>=0){
        change -= coin;
        numberOfCoins++;
    }
}

void countQuarter(){
    processCoin(25); //Runs while we can use quarters
    countDime();
}

void countDime(){
    processCoin(10); //Runs while we can use dimes
    countNickel();
}

void countNickel(){
    processCoin(5); //Runs while we can use nickels
    countPenny();
}

void countPenny(){
    processCoin(1); //Runs while we can use pennies
}