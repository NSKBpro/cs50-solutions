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
void swapEven(void);
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
void init(void)
{
    int current = (d*d)-1;
    for(int i = 0;i<d;++i){
        for(int j = 0;j<d;++j){ //Nested for , for matrix
            if(current>0 && !(i==d-1 && j==d-1)){ 
                board[i][j] = current; //Sets current element from current variable (which starts at d*d-1 and decrements)
                current--;
            }
        }
    }
    swapEven(); //Swaps first and second number if dimension is even
    board[d-1][d-1] = 0; //sets bottom right element as blank
}

/**
 * Checks if dimension is even, swaps first and second element if it is.
 */
void swapEven(void){
    if(d%2==0){ //Checks if dimension is even
        int tmp = board[d-1][d-2]; //Temp value for first number
        board[d-1][d-2] = board[d-1][d-3]; //Swap first and second
        board[d-1][d-3] = tmp; //Put back value from temp variable
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for(int i = 0;i<d;++i){
        for(int j = 0;j<d;++j){
            if(board[i][j] == 0)
                printf("_\t"); //Prints _ if value of current element is 0 (if its blank tile)
            else
                printf("%d\t", board[i][j]);
        }
        printf("\n\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    int tileRow, tileCol, blankRow, blankCol; // Current row and columns for tile and for blank 
    for(int i = 0;i<d;++i){
        for(int j = 0;j<d;++j){
            if(tile == board[i][j]){ //Gets row and column index of user input tile
                tileRow = i;
                tileCol = j;
            } else if(0 == board[i][j]){ //Gets row and column index of blank tile
                blankRow = i;
                blankCol = j;
            }
        }
    }
    
    if((board[tileRow][tileCol-1] == board[blankRow][blankCol] && tileCol!=0 && tileRow == blankRow) ||  //Checks if its allowed to move left
       (board[tileRow][tileCol+1] == board[blankRow][blankCol] && tileCol!=d-1  && tileRow == blankRow ) || //Checks if its allowed to move right
       (board[tileRow-1][tileCol] == board[blankRow][blankCol] && tileRow>0 && tileCol == blankCol) || //Checks if its allowed to move up
       (board[tileRow+1][tileCol] == board[blankRow][blankCol] && tileRow<d-1 && tileCol == blankCol)){ //Checks if its allowed to move down
            board[blankRow][blankCol] = board[tileRow][tileCol]; //Swaps user tile and blank tile
            board[tileRow][tileCol] = 0; 
            return true;
       }
   
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int expected = 1; //sets expected value counter 
    for(int i = 0;i<d;++i){
        for(int j = 0;j<d;++j){ //Nested for to iterate in 2d array
            if(i==d-1 && j==d-1) //If current element is last element, expect 0(_ , blank)
                expected=0;
                
            if(board[i][j] != expected){ //As soon as current element doesnt match expected value function returns false
                return false;
            }
            expected++;
        }
    }
    return true;
}
