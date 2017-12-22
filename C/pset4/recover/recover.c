/**
 * Tool to recover deleted photos from FAT memory card
 */
       
#include <stdio.h>
#include <stdlib.h>

FILE *currentImageptr;

int main(int argc, char *argv[])
{
     if(argc != 2){
         fprintf(stderr,"Usage: ./recover filename.raw\n");
         return 1;
     }
     
    // open card file 
    char *cardPath = argv[1]; 
    FILE *cardptr = fopen(cardPath, "r");
    if (cardptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", cardPath);
        return 2;
    }
    
    unsigned char buffer[512]; //Temporary buffer for 512bytes
    
    int count = 0, shouldWrite = 0;
    
    //repeat until end of card
    while(fread(buffer, sizeof(buffer), 1, cardptr)){ //read 512 bytes into a buffer
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff){ //check if first 3 bytes are indicating new JPG file
        
           if(currentImageptr != NULL){ //if its not first image, close previous image
               fclose(currentImageptr);
               shouldWrite = 0;
           }
           
               char imageName[7];
               sprintf(imageName, "%03d.jpg", count++); //Make proper file name 
               currentImageptr = fopen(imageName, "w"); //Create and open new image
               shouldWrite = 1; //Indicator that image is opened
        }
        
        if(shouldWrite != 0){
                   fwrite(&buffer,sizeof(buffer),1,currentImageptr); //Write current buffer into opened image
               }
    }
    
    //close any remaining files
    fclose(currentImageptr);
    fclose(cardptr);
    return 0;
 
}