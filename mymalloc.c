#include "mymalloc.h"
/**
 * IMPORTANT INFO
 * Size of the metadata is 16 bytes
 * Definitions in mymalloc.h not working
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

void* mymalloc(int size) {
    
    if(isFirstCall()) {
        printf("it's the first call\n");
        metadata firstMetadata = {0x0404, 0, 4080, NULL};
        metadata* ptrFirstMetadata = (metadata*) myblock;
        *ptrFirstMetadata = firstMetadata;

    } else {
        printf("it's been called before\n");
    }
    metadata* metaPtr = (metadata*) myblock;
    void* resultPtr = NULL;
    bool foundSpace = false;
    while(foundSpace || metaPtr == NULL) {
        if(!(metaPtr->inUse)) {
            printf("it not in use");
            if(size >= size + sizeof(metadata)) {
                // TODO need to code in case at the end where we don't care about space for metadata
                resultPtr = (void*) (metaPtr + 1);
                foundSpace = true;
            }
        }
        metaPtr = metaPtr->next;
    }

    // make new metadata piece
    


}

bool isFirstCall() {
    unsigned short firstByte = (short) 0 | ((char) myblock[0]);
    unsigned short secondByte = 0 | myblock[1];
    unsigned short firstTwoBytes = 0 | firstByte;
    firstTwoBytes = firstByte | (secondByte << 8);
    printf("the first 2bytes make %d\n", firstTwoBytes);
    return !(firstTwoBytes == 0x0404);
}