/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;


// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    //initialise grid array with dimensions according to d 
    
    
    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }
    

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    //update 1: now convinced about the win. all good now.
    // /**
    //  * not convinced about the win. need to print last state of the board first to check
    //  * 
    //  */
    //  printf("\n"); 
    // for (int i =0; i < d; i++)
    // {
    //     for (int j =0; j < d; j++)
    //     {
    //         if(board[i][j]==0)
    //         {
    //           printf("_\t"); 
    //         }
    //         else
    //         {
    //         printf("%i\t",board[i][j]);
    //         }
    //     }
    //     printf("\n");
    // }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init()
{
    // TODO
    int numbers = (d*d)-1;
    for (int i =0; i < d; i++)
    {
        for (int j =0; j < d; j++)
        {
            board[i][j] = numbers;
            numbers--;
            
            if( (d*d)%2==0)
            {
                if(board[i][j]==2)
                {
                    board[i][j] = 1;
                }
                else if(board[i][j]==1)
                {
                    board[i][j]= 2;
                }
            }
            
        }
    }
    // printf("\n****** Drawing Board to see ******\n");
    // for (int i =0; i < d; i++)
    // {
    //     for (int j =0; j < d; j++)
    //     {
    //         printf("%i\t",board[i][j]);
    //     }
    //     printf("\n");
    // }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // TODO
    //just a sample output of our array so far
    //printf("\n****** Drawing Board to see ******\n");
    for (int i =0; i < d; i++)
    {
        for (int j =0; j < d; j++)
        {
            if(board[i][j]==0)
            {
               printf("_\t"); 
            }
            else
            {
            printf("%i\t",board[i][j]);
            }
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // TODO
    //initialise indexes for the chosen tile's row and column, and blank tile's row and column
    int tileCol;
    int tileRow;
    int blankCol;
    int blankRow;
    
    /**
     * 
     * Also note that check50 assumes that you’re indexing into board a la board[row][column], not board[column][row].
     * 
     * this means my implementation actually obeys the above assumption. but variables are unfortunately in the reverse
    */
    
    //the blank tile always has a value of 0 in the board array.
    int blank = 0;
    for(int i=0; i<d; i++)
    {
        for (int j=0; j<d; j++)
        {
            if (board[i][j]==tile)
            {
                //printf("Tile %i is at [%i][%i]\n",tile,i,j);
                tileCol =i;
                tileRow =j;
            }
            if (board[i][j]==0)
            {
                blankCol =i;
                blankRow =j;
                //printf("Blank %i is at [%i][%i]\n",board[i][j],i,j);
            }
        }
    }
    //check if the two pieces, blank and tile to be moved are in the same Column (up to down)
    if(blankRow==tileRow)
    {
        if(tileCol-blankCol==1 || tileCol-blankCol==-1)
        {
            //perform swap. 
            //but first...print for tests
            //printf("Tile is at [%i][%i] and Blank is at [%i][%i]\n",tileCol,tileRow,blankCol,blankRow);
            board[blankCol][blankRow]=tile;
            board[tileCol][tileRow]=blank;
            
            return true;
        }
    }
    //check if they're in the same Row (across)
    else if(blankCol==tileCol)
    {
        //now that they're in the same column. see if they're adjacent to eachother
        //meaning their difference in index would be 1 or -1
        if(tileRow-blankRow==1 || tileRow-blankRow==-1)
        {
            //perform swap
            //printf("Tile is at [%i][%i] and Blank is at [%i][%i]\n",tileCol,tileRow,blankCol,blankRow);
            
            //set the tile being moved to be at the previous position of the blank tile.
            board[blankCol][blankRow]=tile;
            
            //set the blank tile to be at the previous position of the moved tile.
            board[tileCol][tileRow]=blank;
            return true;
        }
    }
    
    return false;
    
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // TODO
    int number = 1 ;
    for(int i=0; i<d; i++)
    {
        for (int j=0; j<d; j++)
        {
                
                //win condition. blank tile must be back to the bottom corner and the variable number should have reached (d*d)-1 atleast
                    if(number>=(d*d)-1 && board[d-1][d-1]==0)
                    {
                        // printf("Number is %i and board[%i][%i] is %i",number,i,j,board[i][j]);
                        // usleep(5000000);
                        return true;
                    }
                
              //check for each number  
            if(board[i][j]==number)
            {
                number++;
                continue;
            }
            else
            {
               return false; 
            }
        }
    }
    return false;
}
