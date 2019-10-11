#include "mymalloc.h"

// This main method is just for testing purposes, the final product should not have it
int main(int argc, char** argv) {
    if(argc < 3) {
        printf("Not enough arguments: Format it like so:\n");
        printf("<executable-name> <command> <number>");
    }
    // commands will be a single character
    char command = argv[1][0]; // m for malloc, f for free?, a for access? idfk
    int number;
    // number is the amount of bytes allocated
    sscanf(argv[2], "%d", &number);

    switch(command){
        case 'm':
            mymalloc(number);
            break;
        case 'f':
            // find some way to save pointers lol
            break;
        case 'a':
            break;
        default:
            printf("why the fuck would you type %c you idiot\n", command);
    }

}

void* mymalloc(int size) {
    
}