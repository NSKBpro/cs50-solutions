/**
 * generate.c
 *
 * Generates pseudorandom numbers in [0,MAX), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */
 
#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// upper limit on range of integers that can be generated
#define LIMIT 65536

int main(int argc, string argv[])
{
    
    if (argc != 2 && argc != 3) //True if there are either too many argumennts (more than name of program, n and s) or if there is no argument at all (except program name)
    {
        printf("Usage: ./generate n [s]\n");
        return 1;
    }

    // casting string argument to int
    int n = atoi(argv[1]);

    
    if (argc == 3) // true if there is s argument
    {
        srand48((long) atoi(argv[2])); //Calls srand48 (which sets seed value), seed value is set to optional S argument 
    }
    else //goes into else block if there is only n argument
    {
        srand48((long) time(NULL)); //Calls srand48 (which sets seed value), seed value is time since Epoch (00:00:00 UTC, January 1, 1970), measured in seconds.
    }


    for (int i = 0; i < n; i++) //Loops from 0 to n argument user entered
    {
        printf("%i\n", (int) (drand48() * LIMIT)); //drand48() returns nonnegative double-precision floating point values from interval [0.0, 1.0) , its multiplied by constant LIMIT which is 65536
    }

    // success
    return 0;
}
