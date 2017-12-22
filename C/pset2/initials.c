#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

//FUNCTIONS
string getUserInput();
void getInitials(string name);

int main(void){
    string userInput = getUserInput(); //Get string input from user
    getInitials(userInput); //Takes initial user input as argument and prints initials
}

string getUserInput(){
    string input;
    do{
        input = get_string();
    }while(input==NULL); //While input is not valid string get_string() loops
    return input;
}

void getInitials(string userInput){
    
    if(userInput[0] != ' ' && isalpha((int)userInput[0])) //Check only once if first character is not space but it is alphabetic,instead of checking every time in for loop
            printf("%c",toupper(userInput[0])); //Converts first character to upper case and prints it
        
    for(int i=1,inputLength=strlen(userInput);i<inputLength;i++){
        if(userInput[i-1] == ' ' && userInput[i] != ' ' && isalpha((int)userInput[i])) //Checks if previous character was space and current character is not space(but it is alphabetic)
            printf("%c",toupper(userInput[i])); //Converts current character to upper case and prints it
    }
    
    printf("\n");
}