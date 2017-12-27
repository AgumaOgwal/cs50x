/**
 * PSET 5
 * RECOVER
 * Author:
 *  Humphrey Ogwal
 * 6th OCtober 2017
 */

#include <stdio.h>
#include <stdint.h>
#include <cs50.h>

//defining a new datatype called byte. which is basically an unsigned in of 8 bits/.. i think..
//originally borrowed from bmp.h ....or Microsoft has definitions for these types
typedef uint8_t  BYTE;

//declaring our struct we shall be using. A buffer....each element of which is a byte
typedef struct 
{
        BYTE element;
}__attribute__((__packed__))
BUFFER;


int main (int argc, char *argv[])
{
    //ensure correct usage as we are expecting a command line argument
    if(argc !=2 )
    {
        fprintf(stderr,"Usage: ./recover image\n");
        return 1;
    }
    
    //store the name of our memory card file. which is in argv[]
    char *memcardfile = argv[1];
    //open memory card file
    FILE *inptr = fopen(memcardfile,"r");
    
    //ensure the memory card file exists
    if(inptr == NULL)
    {
        fprintf(stderr,"Could not Open file\n");
        return 2;
    }
    
    
    //since each block in the input file is 512 Bytes
    //And since our custom variable BUFFER is only a Byte big
    //we use an array of buffers...accepting 512 values
    BUFFER buffer[512];
    
    //to help us keep count of our while loop
    int count=0;
    
    //to let us know when we start writing to a file
    bool hasStartedWriting = false;
    
    //declare a file pointer to which we shall be writing the image
    FILE *img = NULL;
    
    //we need to be naming these image file somehow
    //so we need to allocate some space in memeory for that
    //names will be in form 000.jpg
    //in other words [0][0][0][.][j][p][g][\0]
    //which are 7 characters in total, excluding the terminator \0
    char *filename = malloc(sizeof(char)*7);
    
    //lets just run this loop forever.
    //we shall terminate the loop when we can nolonger read any more from the input file
    while(true)
    {
        //printf("Count is %i\n",count);
           
        //Check if fread returns 512, i.e if a block of 512 bytes has been read
        //also....at this check fread has already executed
        //especially if it returned 512...i.e if the test was positive
        if (fread(&buffer,sizeof(BUFFER),512,inptr) == 512)
        // 
        {
            //hasStartedWriting is a variable that is triggered the moment a JPEG is found.
            //Check done at this point...to continue writing to the file
            //because at each iteration of the loop, the file is read ...above this in the if statement
            //hasStartedWriting will only be made false once again when it encounters another JPEG
            if(hasStartedWriting)
            {
                //check50 was failing
                //saying files not recovered correctly. yet the images are there
                //added this check to do nothing when we encounter another jpeg. this is to prevent us writing first bytes of the new jpeg to
                //the previous jpeg
                //----
                //thanks to this answer on stackexchange
                //https://cs50.stackexchange.com/questions/10799/pset4-recover-works-correctly-but-fails-check50
                if(buffer[0].element==0xff && buffer[1].element==0xd8 && buffer[2].element==0xff && (buffer[3].element & 0xf0) == 0xe0)
                {
                    
                }else
                {
                //we need to write to our file everytime we read from it
                fwrite(&buffer,sizeof(BUFFER),512,img);
                }
            }
            if(buffer[0].element==0xff && buffer[1].element==0xd8 && buffer[2].element==0xff && (buffer[3].element & 0xf0) == 0xe0)
            {
                //If it has already been writing to the file, then we close the file img we are writing to
                //afterthought: setting the flag to false wont make much difference anyway
                if(hasStartedWriting)
                {
                    //close the image file that we have been writing to. To allow us open another and write to it
                    fclose(img);
                    hasStartedWriting = false;
                }
                //sprintf helps us give a name to the jpegs dynamically. using count as the increment variable for naming the file
                sprintf(filename, "%03i.jpg",count);
                count++;
                //open a new file for writing, using a name generated above by sprintf
                img = fopen(filename, "w");
                //write to the file img, with &buffer as out source.
                fwrite(&buffer,sizeof(BUFFER),512,img);
                //to let ourselves know that we have now started writing to the file
                hasStartedWriting = true;
            }
        }
        else
        {
            //Stop Execution of the loop at this point
            //Because the File is no longer being read.
            break;
            
        } 
        //free(filename);
    }
    
    //we can now close our input file (card.raw)
    fclose(inptr);
    
    return 0;
}
