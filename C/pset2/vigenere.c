#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//FUNCTIONS
string getUserInput();
void getCipherText(string input, string cipher);

int main(int argc, char **argv){
    if(argc != 2){ 
       printf("ERROR: you must pass argument!\nUsage: ./vigenere cipher\n");
       return 1;
   } else if (argc == 2) {
        for(int i=0,len=strlen(argv[1])-1;i<len;i++){ //Iterates through each character in argv[1] and if any of characters isn't alphabetic or its number it will return 1
            if((!isalpha(argv[1][i])) || isdigit(atoi(&argv[1][i]))){
                printf("ERROR: Argument must be string!\nUsage: ./vigenere cipher\n");
                return 1;
            }
        }
    }
    printf("plaintext:");
    string tmp = getUserInput();
    getCipherText(tmp,argv[1]);
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

void getCipherText(string input, string cipher){
    char outputCipher[strlen(input)]; // Memory for new ciphertext 
    int cipherCounter = 0;
    int cipherLength = strlen(cipher)-1;
    printf("ciphertext:");
    for(int i =0,length=strlen(input);i<length;i++){
        if(isalpha(input[i])){ //Checks if current character is alphabetic
            int currentCipherAlphabetic = (toupper(cipher[cipherCounter])-65)%26; //Converts every character of cipher key to uppercase and calculates its alphabetic index
            int cipherAsciiValue = input[i] + currentCipherAlphabetic; //Adds cipher(alphabetic) to current character(ascii)
            if(cipherCounter < cipherLength){ //Checks if it needs to increase counter for cipher key character, or if it needs to restart it to index 0
                cipherCounter++;
            }else{
                cipherCounter = 0;
            }
            if(!isalpha(cipherAsciiValue)){ //If current cipher character is now not valid alphabetic character
                if(isupper(input[i])){
                    int vigenereCharacter = (cipherAsciiValue-65)%26; //Get alphabetic index of character
                    outputCipher[i] = vigenereCharacter+65; //Convert back to ASCII
                }else{
                    int vigenereCharacter = (cipherAsciiValue-97)%26; //Get alphabetic index of character
                    outputCipher[i] = vigenereCharacter+97; //Convert back to ASCII
                    
                }
            }else
            outputCipher[i] = cipherAsciiValue; // If current cipher character is still valid alphabetic character
             if(isupper(input[i]) && outputCipher[i] > 90){ //if encrypted character goes out of range for upper case characters in ASCII table
                 if(currentCipherAlphabetic==25){ //Checks if current encrypted character is 'Z'
                    currentCipherAlphabetic = input[i] - 66; 
                    outputCipher[i] = 65 + currentCipherAlphabetic; // resets character to 'A' and adds alphabetic encrypted value, needed for FOOBAR example in unit50
                }else
                    outputCipher[i] = 65 + currentCipherAlphabetic; //If current character is not 'Z' it just adds alphabetic index
                    
             }else if (islower(input[i]) && outputCipher[i] < 97){ //if encrypted character goes out of range for lower case characters in ASCII table
               if(currentCipherAlphabetic==25){ //Checks if current encrypted character is 'z'
                    currentCipherAlphabetic = input[i] - 98;
                    outputCipher[i] = 97 + currentCipherAlphabetic; // resets character to 'a' and adds alphabetic encrypted value, needed for FooBar example in unit50
                }else
                    outputCipher[i] = 97 + currentCipherAlphabetic; //If current character is not 'z' it just adds alphabetic index
                }
            }else
                outputCipher[i] = input[i]; // If current character was not alphabetic character, just clone it to new string (symbols, numbers, ..)
        
        printf("%c",outputCipher[i]);
    }
}