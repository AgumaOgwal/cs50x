/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <ctype.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy n infile outfile\n");
        return 1;
    }
    if (atoi(argv[1])<=0 || atoi(argv[1])>100)
    {
        fprintf(stderr, "Usage: ./copy n infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];
    
    int size = atoi(argv[1]);
      bool needPadding = false;

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

    //**My Addition
    //Output some of infile's info.
    fprintf(stderr, "Resize Scale: %i\n", size);
    fprintf(stderr, "bi.Height: %i\n", bi.biHeight);
    fprintf(stderr, "bi.Width: %i\n", bi.biWidth);
    fprintf(stderr, "bi.SizeImage: %i\n", bi.biSizeImage);
    fprintf(stderr, "bf.bfSize: %i\n", bf.bfSize);
    //fprintf(stderr, "bi.SizeImage: %i\n", bi.biSizeImage);
    
    //** had to create these variables to keep track of the original heights and weights for the sake of looping to create the new outfile
    int origbiHeight = bi.biHeight;
    int origbiWidth = bi.biWidth;
    //**calculate new sizes
    //bitmapinfo header first
  
    bi.biHeight = bi.biHeight * size;
    if((bi.biWidth * size) %4 != 0)
    {
        fprintf(stderr, "Needs Padding: \n");
        needPadding = true;
    }
    bi.biWidth = bi.biWidth * size;
    
    //padding variable for purposes of computing overall file size
    int padder = 0;
    if(needPadding)
    {
        //***got this formula from later in this program
        padder = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4; 
        fprintf(stderr, "Needs padding of %i\n",padder); 
        bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padder) * abs(bi.biHeight);
    }
    else
    {
        bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padder) * abs(bi.biHeight);
    }
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    //**Output some of these new sizes and dimensions
    fprintf(stderr, "bi.Height: %i\n", bi.biHeight);
    fprintf(stderr, "bi.Width: %i\n", bi.biWidth);
    fprintf(stderr, "bi.SizeImage: %i\n", bi.biSizeImage);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    //**had to create this variable to remember the padding of the in file
    int origPadding = (4 - (origbiWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //as per Zamyla walkthrough (resize less comfy). I've chosen to use the "rewrite" method. The below is an array to help with that
    //update: Rewrite didnt work out so well. Will now implement with recopy.c. Effectively commenting out the below array
    //RGBTRIPLE pixelArray[bi.biWidth+2];
    
    // iterate over infile's scanlines bi.biWidth
    //for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    for (int i = 0, biHeight = abs(origbiHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        //for (int j = 0; j < bi.biWidth; j++)
            
            
            
            for (int j = 0; j < origbiWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
    
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
    
            
                // write RGB triple to outfile
                for(int x = 0; x < size; x++)
                {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
                
            }
    
        //write padding
            for(int x = 0; x < padding; x++)
            {
                fputc(0x00, outptr);
            }
            
            //recopy implementation starts. write pixels then go back to start of the file and write again. after, skip to the end of the file
            //iterate size-1 times...because we shall do the final copy after this.
            for (int j=0; j<size; j++)
            {
               // 1. Write pixels
               for (int p = 0; p < origbiWidth; p++)
                {
                    // temporary storage
                    RGBTRIPLE triple;
        
                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
        
                
                    // write RGB triple to outfile
                    for(int x = 0; x < size; x++)
                    {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    }
                    
                }
                // 2 . write padding
                for(int x = 0; x < padding; x++)
                {
                    fputc(0x00, outptr);
                }
                
                // 3. Go back to start of the row of the infile
                fseek(inptr, -(origbiWidth), SEEK_CUR);
                
            }
         
        // skip over padding, if any
        fseek(inptr, origPadding, SEEK_CUR);

        //then add it back (to demonstrate how)
        // for (int k = 0; k < origPadding; k++)
        // {
        //     fputc(0x00, outptr);
        // }
        
        
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
