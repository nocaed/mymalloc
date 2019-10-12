#include "mymalloc.h"
/**
 * IMPORTANT INFO
 * Spaghetti ahead
 * 
 * Size of the metadata is 16 bytes
 * Definitions in mymalloc.h not working
 * 
 * I have a lot of print statements throughout for debugging, please do not delete unless you 
 * really hate them
 * 
 * Right now all we can do is add metadata and allocate it
 * 
 * I made a system to test this so you don't just have to hardcode malloc or free in main.
 * Basically run the program like this:
 * ./a.out m 2
 * the 2 can be any number, all that matters is that it's a number
 * 
 */
// This main method is just for testing purposes, the final product should not have it
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
    while(!foundSpace && metaPtr != NULL) { // we end if we either reach the end of the block or find enough memory
        if(!(metaPtr->inUse)) {
            printf("it not in use");
            if(metaPtr->size >= size + sizeof(metadata)) {
                // TODO need to code in case at the end where we don't care about space for metadata
                resultPtr = (void*) (metaPtr + 1);
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
        metadata* newMetaPtr = resultPtr + size; // could cause an error if it's adding size * sizeof(metadata)
        short sizeLeft = (myblock + 4095) - (((char*) newMetaPtr) + 15);
        metadata newMeta = {0x0404, 0, sizeLeft, NULL};
        *newMetaPtr = newMeta;
        printf("Made new metadata\n");
    }
    return resultPtr;

}

bool isFirstCall() { // i dont know why i used bitwise operators but i really did not need to
    unsigned short firstByte = (short) 0 | ((char) myblock[0]);
    unsigned short secondByte = 0 | myblock[1];
    unsigned short firstTwoBytes = 0 | firstByte;
    firstTwoBytes = firstByte | (secondByte << 8);
    printf("the first 2bytes make %d\n", firstTwoBytes);
    return !(firstTwoBytes == 0x0404);
}