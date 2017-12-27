/**
 * Copies a BMP piece by piece, just because.
 */
 
 /**
  * I had written another file now called resizeOld.c
  * But it was very buggy. So i started afresh
  * the major change here is creating completely new variables for the outfile's headers (BIMAPINFOHEADER and BITMAPFILEHEADER)
  */
       
#include <stdio.h>
#include <stdlib.h>

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
    
    //CREATE variables for the outfile's headers
    BITMAPFILEHEADER bf_out = bf;;
    BITMAPINFOHEADER bi_out = bi;
    
    //compute the new dimensions for the outfile. using the scale factor, size
    bi_out.biHeight = bi.biHeight * size;
    bi_out.biWidth = bi.biWidth * size;
    
    //compute the padding that will come with the outfile, as per bmp rules (width should be a multiple of 4)
    int newPadding = (4 - (bi_out.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    //compute the size of the new image/outfile (only dimensions and padding)
    bi_out.biSizeImage = ((sizeof(RGBTRIPLE) * bi_out.biWidth) + newPadding) * abs(bi_out.biHeight);
    
    //and finally, compute the new total size of the image, including its headers
    bf_out.bfSize = bi_out.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    //the below fprintf line was used for debugging. now removing it
    //fprintf(stderr, "Padding is %i\n",newPadding);
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_out, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_out, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    //unnecesary repetition
    newPadding = (4 - (bi_out.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        //recopy method starts here
        //basic logic is, write a line, then take the cursor back to the start of the infile current row, then write again
        //done in 3 steps: write pixesl. 2. write padding. 3. take cursor back
        //this should iterate n-1 times, where n is the scale factor.  this is so that one final time pixels are written before moving to the next row
            for (int j=0; j<size-1; j++)
            {
               // 1. Write pixels
               for (int k = 0; k < bi.biWidth; k++)
                {
                    // temporary storage
                    RGBTRIPLE triple;
        
                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
        
                
                    // write RGB triple to outfile
                    for(int l = 0; l < size; l++)
                    {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    }
                    
                }
                // 2 . write padding
                for(int m = 0; m < newPadding; m++)
                {
                    fputc(0x00, outptr);
                }
                
                // 3. Go back to start of the row of the infile
                int movement = bi.biWidth*sizeof(RGBTRIPLE);
                fseek(inptr, -movement, SEEK_CUR);
                
            }
            
            // iterate over pixels in scanline
            //this is the last tine its happening for the row
            for (int n = 0; n < bi.biWidth; n++)
            {
                // temporary storage
                RGBTRIPLE triple;
    
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
    
                // write RGB triple to outfile
                for(int p=0; p<size; p++)
                {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
                
            }
            //was getting a bug because this padding bit of code was within the previous loop.
            //did some troubleshooting with xxd -c 56 -g 3 -s 54 student.bmp > studentbytes.txt
            //had already previously created student bmp with ./resize 6 small.bmp student.bmp
            //torubleshooting showed that the problem was with the positioning of this loop
            //moved it out here and all is well now
            for (int q = 0; q < newPadding; q++)
                {
                    fputc(0x00, outptr);
                }
            //fseek(inptr, -(bi.biWidth * sizeof(RGBTRIPLE)), SEEK_CUR);
        
       
        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        // for (int k = 0; k < padding; k++)
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
