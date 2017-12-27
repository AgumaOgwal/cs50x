#include <stdio.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t  BYTE;

typedef struct 
{
        BYTE element;
}__attribute__((__packed__))
BUFFER;


int main (int argc, char *argv[])
{
    if(argc !=2 )
    {
        fprintf(stderr,"Usage: ./recover file\n");
        return 1;
    }
    
    char *memcardfile = argv[1];
    //open memory card file
    FILE *inptr = fopen(memcardfile,"r");
    
    if(inptr == NULL)
    {
        fprintf(stderr,"Could not Open file\n");
        return 2;
    }
    
    
    //find beginning of jpeg
    BUFFER buffer[512];
    bool okay = true;
    int count=0;
    // bool foundJpeg = false;
    // bool firstTime
    
    //FILE *img = NULL;
    while(okay)
    {
        //printf("Count is %i\n",count);
        //char *filename =malloc(sizeof(char)*7);  
      
        if (fread(&buffer,sizeof(BUFFER),512,inptr) == 512)
        // 
        {
            if(foundJpeg==false){
            FILE *img = NULL;
            }
            if(img != NULL)
            {
                fclose(img);
                
            }
        }  else{break;}      
            //printf("%lu\n",fread(&buffer,sizeof(BUFFER),512,inptr));
            
            //the below line to test output so far. if we are reading
            // for(int i=0; i<sizeof(buffer);i++)
            // {
            //     if(buffer[i].element==0xd8)
            //     {
            //         printf("We found a 0xd8");
            //     }
            // }
            
            if(buffer[0].element==0xff && buffer[1].element==0xd8 && buffer[2].element==0xff && (buffer[3].element & 0xf0) == 0xe0)
            {
                printf("We found a JPEG");
                // printf("Before sprintf...\n");
                // sprintf(filename, "%03i.jpg",count);
                // printf("sprintf...\n");
                count++;
                printf(" - %i\n",count);
                // foundJpeg = true;
                // printf("foundJpeg=true\n");
                // img = fopen(filename, "w");
                // fwrite(&buffer,sizeof(BUFFER),512,img);
            }
            // if (count==50)
            // {
            //     break;
            // }
        
        //free(filename);
    }
    
    
    
    //open a new JPEG
    
    
    
    //Write 512 bytes until new JPEG is found
    
    
    
    //Detect end of file  
    fclose(inptr);
}
