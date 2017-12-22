/**
 * Resizes given bmp image for given zoom amount
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "bmp.h"


int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy zoom infile outfile\n");
        return 1;
    }
    // handle invalid zoom amount error
    if(atoi(argv[1]) < 0 || atoi(argv[1]) > 100) {
        fprintf(stderr, "Invalid zoom amount, it has to be between 1 and 100.\n");
        return 2;
    }

    double zoom = atof(argv[1]);
    
    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    //Save important information from original file , padding-width-image size , which we will have to edit
    int sourceWidth = bi.biWidth;
    int sourcePadding = (4 - (sourceWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int sourceBiSizeImage = bi.biSizeImage;
    
    //change dimensions of image , dimension * zoom amount
    bi.biWidth *= zoom;
    bi.biHeight *= zoom; 
    
    //calculate padding
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) %4) % 4;
    
    //change image size
    bi.biSizeImage = (bi.biWidth * abs(bi.biHeight) * 3) + padding*abs(bi.biHeight);
    bf.bfSize = bf.bfSize - sourceBiSizeImage + bi.biSizeImage;

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < sourceWidth; j++)
        {
                // temporary storage
                RGBTRIPLE triple;
    
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
    
                // write RGB triple to outfile
                for(int k = 0;k<zoom;++k) // Write same pixel zoom value times
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }
        
        // skip over padding, if any
        fseek(inptr, sourcePadding, SEEK_CUR);
        
        // Using demonstration code to add padding, actually add new padding now
        for (int l = 0; l < padding; ++l)
            fputc(0x00, outptr);
        
        
        long additional = sourceWidth*sizeof(RGBTRIPLE) + sourcePadding;
        // true if current i % zoom amount is == 0 , TRUE : write a new row // FALSE : repeat row one more time
        if(fmod((i+1),zoom) != 0)
            fseek(inptr, -additional, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
