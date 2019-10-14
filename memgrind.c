#include "mymalloc.h"
#include <stdio.h>
#include <time.h>

void firstTest();
void secondTest();
void thirdTest();
void fourthTest();
void fifthTest();
void sixthTest();
int genRandomIntByRange(int upper, int lower);

const int hundredFifty = 150; // loop counter
const int fifty = 50; // allocation counter

// processes each test case
int main(int argc, char** argv)
{
    firstTest();
    secondTest();
    thirdTest();
    fourthTest();
    fifthTest();
    sixthTest();

    return 0;
}

// Mallocs 1 byte and frees it, 150 times
void firstTest()
{
    int i;

    char* ptr;

    for(i = 0; i < hundredFifty; i++)
    {
        ptr = (char*)malloc(1);
        free(ptr);
    }
}

// malloc 1 byte, store the pointer in an array 150 times, once 50 bytes have been allocated, free 50 1 by 1
void secondTest()
{
    int i; // outer loop counter for malloc/free cycle
    int j; // inner loop counter for freeing pointers
    int cntr = 0; // counts number of bytes allocated

    char* ptr; // current pointer being assigned memory

    char* ptrArr[fifty]; // array of pointers

    // loops 150 times
    for(i = 0; i < hundredFifty; i++)
    {
        // if there have been fifty allocations...
        if(cntr == fifty)
        {
            // ...iterate through the pointer array and free each pointer
            for(j = 0; j < fifty; j++)
            {
                free(ptrArr[j]);
            }

            // reset the counter
            cntr = 0;
        }

        // allocate memory for pointer
        ptr = (char*)malloc(1);
        // assign ptr to array
        ptrArr[cntr] = ptr;

        // increment counter
        cntr++;
    }

    // at the end of the loop, there's a block of memory allocated, so this frees it
    free(ptrArr[cntr-1]);
}

// randomly choose between malloc and free
void thirdCase()
{
    // seed the random number generator
    srand((unsigned) time(0));

    int mallocCntr = 0; // holds the number of times that malloc has been called
    int randNum; // holds a randomly generated number
    int ptrsToFree = 0; // holds the amount of pointers to free
    int i; // iterates through pointers that need to be freed

    char* ptr; // current memory being allocated

    char* ptrArr[50]; // contains pointers to malloc'd memory

    bool fiftyMallocs = false; // condition for loop

    // while there haven't been 50 mallocs...
    while(fiftyMallocs == false)
    {
        // ...generate random number between 0-1
        randNum = genRandomIntByRange(0, 1);

        // if the random number is 0...
        if(randNum == 0)
        {
            // ...call malloc, and store pointer somewhere
            ptr = (char*)malloc(1);
            ptrArr[ptrsToFree] = ptr;
            // increment calls of malloc
            mallocCntr++;
            // increment the amount of pointers to free
            ptrsToFree++;

        }
        // otherwise, we're freeing memory
        else
        {
            // if there's something to free...
            if(ptrsToFree != 0)
            {
                free(ptrArr[ptrsToFree-1]);
                ptrsToFree--;
            }
        }

        // if there's been 50 calls of malloc...
        if(mallocCntr == fifty)
        {
            // ...set loop condition to end state
            fiftyMallocs = true;
        }
    }

    // free everything thats left in the memory
    for(i = 0; i < ptrsToFree; i++)
    {
        free(ptrArr[i]);
    }
}

// generates a random number between lower-upper, inclusive
int genRandomIntByRange(int lower, int upper)
{
    int randNum = rand() % (upper - lower + 1) + lower;
    
    return randNum;
}