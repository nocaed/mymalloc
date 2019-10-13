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
    int number;
    // number is the amount of bytes allocated
    sscanf(argv[2], "%d", &number);
    do {
        switch(command){
            case 'm':
                mymalloc(number);
                break;
            case 'f':
                // find some way to save pointers lol
                break;
            case 'a':
                break;
            case 'q':
                break;
            default:
                printf("why the fuck would you type %c you idiot\n", command);
            
        }
        printf("you gotta type q and a number to quit\n");
        printf("type command and number: ");
        scanf("%c %d", &command, &number);
    }while(command != 'q');

}

void* mymalloc(size_t size) {
    
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
    while(!foundSpace && metaPtr != NULL) { // we end if we either reach the end of the block or find enough memory
        if(!(metaPtr->inUse)) {
//            printf("it not in use");
            if(metaPtr->size >= size + metadataSize) { // if the metadata shows space that is big enough for the size
                                                            // and what the user allocated...
                // TODO need to code in case at the end where we don't care about space for metadata
                if(metaPtr->next != NULL) { // if this is not the last metadata block
                    // if it's not null then we gotta do some shit
                    isSplit = true;
                }
                resultPtr = (void*) (metaPtr + 1); // 
                foundSpace = true;
                metaPtr->size = size;
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
            newMeta.next = metaPtr;
            newMeta.size = (metaPtr - newMetaPtr - metadataSize);
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
void myfree(void* ptr)
{
    metadata* metaAddress = ptr - metadataSize; // Stores the address of the metadata for the pointer in metaAddress

    // if this is the last metadata in the memory...
    if(metaAddress -> next == NULL)
    {
        // ...reset the metadata to have it not in use
        resetMetadata(metaAddress);
    }
    // collapse stuff
    else
    {
        // TODO: COLLAPSE THIS BAD BITCH
    }
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

        }
        // advance metadata ptr
        metaPtr = metaPtr->next;

    }
}