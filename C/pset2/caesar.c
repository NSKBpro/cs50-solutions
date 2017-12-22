#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//FUNCTIONS
string getUserInput();
int getCipherValue(int argCipher);
void getCipherText(string input,int cipher);

int main(int argc, char **argv){
    if(argc != 2 || atoi(argv[1]) <= 0){ //Checks if there is 1 argument passed, checks if argv[1] casted to int is greater than 0
        printf("ERROR: you must pass argument and it can't be negative!\nUsage: ./caesar k(where k is positive integer)\n");
        return 1;
    } 
    printf("plaintext:");
    string input = getUserInput();
    getCipherText(input,getCipherValue(atoi(argv[1]))); //Arguments : Input string from user, calls function to check cipher value
    printf("\n");
    return 0;
}

string getUserInput(){
    string input;
    do{
        input = get_string();
    }while(input==NULL);
    return input;
}

int getCipherValue(int argCipher){
    return (argCipher>26)?argCipher%26:argCipher; //If argument(cipher) is greater than 26 it will calculate cipher%26 , else it will just return cipher 
}

void getCipherText(string input, int cipher){
    char outputCipher[strlen(input)]; // Memory for new ciphertext string
    for(int i =0,length=strlen(input);i<length;i++){
        if(isalpha(input[i])){ //Checks if current character is alphabetic
            int cipherAsciiValue = input[i] + cipher; //Adds cipher to current character
            if(!isalpha(cipherAsciiValue)){ //If current cipher character is now not valid alphabetic character
                if(isupper(input[i])){
                    int caesarCharacter = (cipherAsciiValue%65)-26; //Get alphabetic index of character
                    outputCipher[i] = caesarCharacter+65; //Convert back to ASCII
                }else{
                    int caesarCharacter = (cipherAsciiValue%97)-26; //Get alphabetic index of character
                    outputCipher[i] = caesarCharacter+97; //Convert back to ASCII
                }
            }else
            outputCipher[i] = cipherAsciiValue; // If current cipher character is still valid alphabetic character
        }else
        outputCipher[i] = input[i]; // If current character was not alphabetic character, just clone it to new string (symbols, numbers, ..)
    }
    printf("ciphertext:%s",outputCipher);
}