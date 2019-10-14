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
void thirdTest()
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

// same as C, but with variable byte sizes to allocate
void fourthTest()
{
    // seed the random number generator
    srand((unsigned) time(0));

    const int MEMORY_CAPACITY = 4096; // holds the maximum memory in bytes

    const short metadataSize = sizeof(metadata); // holds the size of metadata

    int remainingMemory = MEMORY_CAPACITY; // initialize remaining memory to total memory
    int mallocCntr = 0; // keeps track of the amount of calls to malloc
    int ptrsToFree = 0; // keeps track of how many pointers need to be freed
    int randNum; // generates a random number to determine a malloc (0) or free(1) call
    int i; // iterates through the array of pointers to free them all
    int mallocSize; // the number of bytes currently being malloc'd
    int ttlMallocBytes; // mallocSize + the size of metadata

    bool fiftyMallocs = false; // loop controller

    void* ptr; // current pointer to allocated memory

    void* ptrArr[fifty]; // array of pointers to allocated memory
    
    int mallocSizes[fifty]; // array of sizes of allocated memory blocks

    // while malloc has not been called fifty times
    while(fiftyMallocs == false)
    {
        // choose between malloc or free
        randNum = genRandomIntByRange(0, 1);

        // malloc roll
        if(randNum == 0)
        {
            // roll a number between [1, 64] in bytes to allocate
            mallocSize = genRandomIntByRange(1, 64);
            // total amount of bytes in memory will be allocated bytes plus the size of their metadata
            ttlMallocBytes = mallocSize + metadataSize;

            // if there is not enough memory...
            if(ttlMallocBytes > remainingMemory)
            {
                // ...print an error message
                printf("Error, cannot allocate more than %d bytes.\n", remainingMemory + metadataSize);
            }
            // otherwise, proceed with malloc
            else
            {
                // create a pointer to the allocated memory
                ptr = malloc(mallocSize);
                // add the pointer to the array
                ptrArr[ptrsToFree] = ptr;
                // add the amount of bytes now taken up by the allocated memory + metadata to mallocSizes
                mallocSizes[ptrsToFree] = ttlMallocBytes;
                // subtract the amount of bytes now in use from the remaining memory
                remainingMemory -= mallocSizes[ptrsToFree];

                // increment the counter of malloc calls
                mallocCntr++;
                // increment the amount of pointers that need to be freed
                ptrsToFree++;
            }
        }
        // otherwise, free roll
        else
        {
            // if there are pointers to free...
            if(ptrsToFree != 0)
            {
                // ...free the most recent pointer!
                free(ptrArr[ptrsToFree-1]);
                // add the freed bytes back to the remaining memory
                remainingMemory += mallocSizes[ptrsToFree-1];
                // decrement the amount of pointers to free
                ptrsToFree--;
            }
        }

        // if there have been fifty calls of malloc...
        if(mallocCntr == fifty)
        {
            // ...set loop condition to end
            fiftyMallocs = true;
        }
    }

    // free all remaining allocated memory
    for(i = 0; i < ptrsToFree; i++)
    {
        free(ptrArr[i]);
    }
}