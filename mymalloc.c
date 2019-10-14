#include "mymalloc.h"
/**
 * IMPORTANT INFO
 * Spaghetti ahead
 * 
 * Size of the metadata is 16 bytes
 * Definitions in mymalloc.h working
 * 
 * I have a lot of print statements throughout for debugging, please do not delete unless you 
 * really hate them
 * 
 * Tasks right now
 * - [x] Start myfree
 * - [x] Allow for splitting of chunks (ok i sort of did it, can't test yet)
 * - [ ] Improve testing environment to allow multiple inputs
 * 
 * I made a system to test this so you don't just have to hardcode malloc or free in main.
 * Basically run the program like this:
 * ./a.out m 2
 * the 2 can be any number, all that matters is that it's a number
 * 
 */
// This main method is just for testing purposes, the final product should not have it
const short metadataSize = sizeof(metadata); // holds the size of metadata

int main(int argc, char** argv) {
    if(argc < 3) {
        printf("Not enough arguments: Format it like so:\n");
        printf("<executable-name> <command> <number>\n");
        return -1;
    }
    // commands will be a single character
    char command = argv[1][0]; // m for malloc, f for free?, a for access? idfk
    char* ptr1 = malloc(20);
    printMeta();
    char* ptr2 = (char*) malloc(4);
    printMeta();
    free(ptr1);
    printMeta();
    char* ptr3 = malloc(2);
    printMeta();
    int number;
    int numPtrsSaved = 0;
    char* ptrs[100];
    // number is the amount of bytes allocated
    sscanf(argv[2], "%d", &number);
    // do {
    //     int i;
    //     for(i = 0; i < numPtrsSaved; i++) {
    //         printf("%d: %p\n", i, ptrs[i]);
    //     }
    //     switch(command){
    //         case 'm':
    //             ptrs[numPtrsSaved] = (char*) malloc(number);
    //             numPtrsSaved++;
    //             break;
    //         case 'f':
    //             // find some way to save pointers lol

    //             break;
    //         case 'a':
    //             break;
    //         case 'q':
    //             break;
    //         default:
    //             printf("why the fuck would you type %c you idiot\n", command);
            
            
    //     }
    //     printMeta();
    //     printf("\n\n");
    //     printf("you gotta type q and a number to quit\n");
    //     printf("type command and number: ");
    //     scanf("[^\n]%c %d", &command, &number);
    // }while(command != 'q');

}

void* mymalloc(size_t size, char* file, int line) {
    
    if(isFirstCall()) { 
        printf("it's the first call\n");
        metadata firstMetadata = {0x0404, 0, 4080, NULL}; // 0x0404 is just the code i chose to verify it.
        metadata* ptrFirstMetadata = (metadata*) myblock;
        *ptrFirstMetadata = firstMetadata; // setting first blocks to the meta data block

    } else {
        printf("it's been called before\n"); // debugging message
    }
    metadata* metaPtr = (metadata*) myblock; // start it at the first meta data block
    void* resultPtr = NULL; // the pointer we will return at the end of the function
    bool foundSpace = false; // did we find enough space for the user? This boolean shows it.
    bool isSplit = false; // will be true if the allocation causes a split between metadatas
    int distanceBtwnMetas;
    metadata* nextPtr;
    while(!foundSpace && metaPtr != NULL) { // we end if we either reach the end of the block or find enough memory
        if(!(metaPtr->inUse)) {
//            printf("it not in use");
            if(metaPtr->size >= size + metadataSize) { // if the metadata shows space that is big enough for the size
                                                            // and what the user allocated...
                // TODO need to code in case at the end where we don't care about space for metadata
                if(metaPtr->next != NULL) { // if this is not the last metadata block
                    // if it's not null then we gotta do some shit
                    isSplit = true;
                    nextPtr = metaPtr->next;
                }
                resultPtr = (void*) (metaPtr + 1); // 
                foundSpace = true;
                metaPtr->size = size;
                // might need to capture the ->next value before it gets changed if its a split
                metaPtr->next = resultPtr + size;
                metaPtr->inUse = 1;
            }
        }
        metaPtr = metaPtr->next;
    }

    // make new metadata piece
    if(foundSpace) {
        // new metadata should be at resultPtr + size i think?
        // This is making a new metadata piece if it's at the end and there are no metadatas after it
        // 
        metadata* newMetaPtr = resultPtr + size;
        short sizeLeft = (myblock + 4095) - (((char*) newMetaPtr) + 15);
        metadata newMeta = {0x0404, 0, sizeLeft, NULL};
        if(isSplit) {
            // WE CAN ASSUME THAT the previous metadata (resultptr - metadatasize) 
            // will not be null in prev->next
            // i want to do two things here:
            // change sizeLeft so that it is the distance between the result pointer and 
            // make newMeta->next point to the next metadata
            newMeta.next = nextPtr;
            newMeta.size = (((void*) nextPtr) - ((void*)newMetaPtr) - metadataSize);
            // can't test this until free works LUL
        }

        *newMetaPtr = newMeta;
 //       printf("Made new metadata\n");
    }
    return resultPtr;

}

bool isFirstCall() { // i dont know why i used bitwise operators but i really did not need to
    unsigned short firstByte = (short) 0 | ((char) myblock[0]);
    unsigned short secondByte = 0 | myblock[1];
    unsigned short firstTwoBytes = 0 | firstByte;
    firstTwoBytes = firstByte | (secondByte << 8);
    // printf("the first 2bytes make %d\n", firstTwoBytes);
    return !(firstTwoBytes == 0x0404);
}

// frees a pointer from memory
void myfree(void* ptr, char* file, int line)
{
    // need some error checks first
    metadata* metaAddress = ptr - metadataSize; // Stores the address of the metadata for the pointer in metaAddress
    resetMetadata(metaAddress);
    collapse();
}

// resets the state of the metadata to ignore the following block of allocated memory
void resetMetadata(metadata* ptr)
{
    ptr -> inUse = 0;
}

/*
 * 
 * I HAVE NOT TESTED THIS
 */
void collapse() {
    // start at beginning of block of memory
    metadata* metaPtr = (metadata*) myblock;

    while(metaPtr != NULL) {
        if(metaPtr->next == NULL) { // if we're at the end, stop
            break;
        }
        metadata* nextMetaPtr = metaPtr->next;

        if(!(metaPtr->inUse) && !(nextMetaPtr->inUse)) { // if current and next are both not in use
            // then we can merge
            // what does merging consist of?
            // 1. set tag in deleted metadata to 0
            nextMetaPtr->tag = 0;
            // 2. metaptr->next = nextMetaPtr->next
            metaPtr->next = nextMetaPtr->next;
            // 3. metaptr->size is now the old size + nextMetaPtr->size + metadataSize
            metaPtr->size = metaPtr->size + nextMetaPtr->size + metadataSize;
            continue;

        }
        // advance metadata ptr
        metaPtr = metaPtr->next;

    }
}

void printMeta() {
    metadata* metaPtr = (metadata*) myblock;
    int counter = 0;
    while(metaPtr != NULL) {
        printf("Metadata num: %d at address %p\n", counter, metaPtr);
        printf("In use: ");
        if(metaPtr->inUse) {
            printf("Yes\n");
        } else {
            printf("No\n");
        }
        printf("Size: %d\n", metaPtr->size);
        printf("Next: %p\n", metaPtr->next);
        printf("------------------------------------\n");
        metaPtr = metaPtr->next;
        counter++;
    }
    printf("\n\n");
}