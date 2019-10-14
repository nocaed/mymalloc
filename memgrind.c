#include "mymalloc.h"

void firstTest();
void secondTest();
void thirdTest();
void fourthTest();
void fifthTest();
void sixthTest();

const int hundredFifty = 150; // loop counter
const int fifty = 50; // allocation counter

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

    void* ptr;

    for(i = 0; i < hundredFifty; i++)
    {
        //ptr = malloc(1);

    }
}