#include <stdio.h>
#include <cs50.h>

//Functions
int getHeight();
void printFirstPiramide(int,int);
void printSecondPiramide(int);

int main(void){
    int height = getHeight(); //Get user input

    for(int i = 1;i<=height;i++){
       
        printFirstPiramide(height,i); //Function send arguments - height from user and current counter, prints first piramide
        
        printf("  "); //Print space between piramides
        
        printSecondPiramide(i); //Function send argument - current counter, prints second piramide
        
        printf("\n");
    }
}

int getHeight(){
    int height;
    
    do{
        printf("Height:");
        height = get_int();
    }while(height<0 || height>23);
    
    return height;
}

void printFirstPiramide(int height,int counter){
    int numOfBlank = height - counter; //Number of spaces before # for current iteration
    
    for(int j=0;j<numOfBlank;j++)
    printf(" ");
    
    for(int k=0;k<counter;k++)
       printf("#");
}

void printSecondPiramide(int counter){
    for(int i = 1;i<=counter;i++)
       printf("#");
}