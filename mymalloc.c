#include <stdlib.h>
#include <stdio.h>

#include "mymalloc.h"


const short metadataSize = sizeof(metadata); // holds the size of metadata
static char myblock[4096];
char* lastAddress = myblock + 4096;


/**
 * Allocates memory in myblock, if there is room. If there is enough room to create
 * a new metadata block, it creates one.
 */
void* mymalloc(size_t size, char* file, int line) {
    
    if(!isMetadata(myblock)) { 
        metadata firstMetadata = {0x0404, 0, 4092}; // 0x0404 is just the code i chose to verify it.
        metadata* ptrFirstMetadata = (metadata*) myblock;
        *ptrFirstMetadata = firstMetadata; // setting first blocks to the meta data block
    }
    metadata* metaPtr = (metadata*) myblock; // start it at the first meta data block
    void* resultPtr = NULL; // the pointer we will return at the end of the function
    bool foundSpace = false; // did we find enough space for the user? This boolean shows it.
    bool isSplit = false; // will be true if the allocation causes a split between metadatas
    metadata* nextPtr;
    while(!foundSpace && (char*) metaPtr < lastAddress - metadataSize - 1) { // we end if we either reach the end of the block or find enough memory
        resultPtr = (void*) (metaPtr + 1);
        if(!(metaPtr->inUse)) {

            if(metaPtr->size >= size + metadataSize + 1) { // if the metadata shows space that is big enough for the size
                                                            // and what the user allocated...
                if(metaPtr->size + metadataSize + ((char*) metaPtr) < lastAddress - metadataSize - 1) { // if this is not the last metadata block
                    // if it's not null then we gotta do some shit
                    isSplit = true;
                    nextPtr = (metadata*) (metaPtr->size + metadataSize + ((char*) metaPtr));
                }
                resultPtr = (void*) (metaPtr + 1);
                foundSpace = true;
                metaPtr->size = size;
                metaPtr->inUse = 1;
            } else if(metaPtr->size >= size) { // if you can't make a new metadata, just flip the inUse and return
                metaPtr->inUse = 1;
                resultPtr = (void*) (metaPtr + 1);
                return resultPtr;
            }
        }
        metaPtr = resultPtr + metaPtr->size;
    }

    // make new metadata piece
    if(foundSpace) {
        // This is making a new metadata piece if it's at the end and there are no metadatas after it
        metadata* newMetaPtr = resultPtr + size;
        short sizeLeft = (myblock + 4095) - (((char*) newMetaPtr) + (metadataSize - 1));
        metadata newMeta = {0x0404, 0, sizeLeft};
        if(isSplit) {
            // change sizeLeft so that it is the distance between the result pointer and 
            newMeta.size = (((void*) nextPtr) - ((void*)newMetaPtr) - metadataSize);
        }

        *newMetaPtr = newMeta;
    } else {
        fprintf(stderr, "\tError in file: %s at line: %d\n", file, line);
        fprintf(stderr, "\tNot enough space to allocate %zu bytes\n", size);
        
    }
    return resultPtr;

}

/**
 * Uses bitwise operators to extract the first two bytes of an address
 */
bool isMetadata(void* address) {
    char* addressChar = (char*) address;
    unsigned short firstByte = (short) 0 | (*addressChar);
    unsigned short secondByte = 0 | *(addressChar + 1);
    unsigned short firstTwoBytes = 0 | firstByte;
    firstTwoBytes = firstByte | (secondByte << 8);
    return (firstTwoBytes == 0x0404);
}

// frees a pointer from memory
void myfree(void* ptr, char* file, int line) {
    // Case: ptr is out of range of myblock
    if(ptr < ((void*) myblock) || ptr >= ((void*)myblock + 4096)) {
        fprintf(stderr, "\tError in file: %s at line: %d\n", file, line);
        fprintf(stderr, "\tCannot free address: %p\n", ptr);
        fprintf(stderr, "\tAddress outside of range for myblock\n");
        return;
    }
    metadata* metaAddress = ptr - metadataSize; // Stores the address of the metadata for the pointer in metaAddress
    // Case: ptr provided is within the first meta data block, which is impossible
    // to allocate memory to
    if((void*)metaAddress < (void*) myblock) {
        fprintf(stderr, "\tError in file: %s at line: %d\n", file, line);
        fprintf(stderr, "\tCannot free address: %p\n", ptr);
        fprintf(stderr, "\tMemory was not allocated at this address\n");
        return;
    }
    if(isMetadata(metaAddress)) {
        // Case: Trying to free something that is not in use
        if(!(metaAddress->inUse) && (char*)(metaAddress->size + metadataSize + metaAddress) != lastAddress) {
            fprintf(stderr, "\tError in file: %s at line: %d\n", file, line);
            fprintf(stderr, "\tCannot free address: %p\n", ptr);
            fprintf(stderr, "\tPointer was already freed\n");
            return;
        } else if(!(metaAddress->inUse)) {
            fprintf(stderr, "\tError in file: %s at line: %d\n", file, line);
            fprintf(stderr, "\tCannot free address: %p\n", ptr);
            fprintf(stderr, "\tMemory was not allocated at this address\n");
            return;
        }
    } else {
        // Case: Ptr points to something that doesn't have a metadata block associated to it.
        fprintf(stderr, "\tError in file: %s at line: %d\n", file, line);
        fprintf(stderr, "\tCannot free address: %p\n", ptr);
        fprintf(stderr, "\tMemory was not allocated\n");
        return;
    }
    resetMetadata(metaAddress);
    collapse();
}

// resets the state of the metadata to ignore the following block of allocated memory
void resetMetadata(metadata* ptr)
{
    ptr -> inUse = 0;
}

/**
 * Collapse iterates through myblock on the metadata. It merges adjacent free memory blocks.
 */
void collapse() {
    // start at beginning of block of memory
    metadata* metaPtr = (metadata*) myblock;

    while((char*) metaPtr < lastAddress - metadataSize - 1) {
        if(metaPtr->size + ((char*) metaPtr) + metadataSize == lastAddress) { // if we're at the end, stop
            break;
        }
        metadata* nextMetaPtr = (metadata*) ((char*) metaPtr + metaPtr->size + metadataSize);

        if(!(metaPtr->inUse) && !(nextMetaPtr->inUse)) { // if current and next are both not in use
            // then we can merge
            // what does merging consist of?
            // 1. set tag in deleted metadata to 0
            nextMetaPtr->tag = 0;
            // 2. metaptr->next = nextMetaPtr->next
//            metaPtr->next = nextMetaPtr->next;
            // 3. metaptr->size is now the old size + nextMetaPtr->size + metadataSize
            metaPtr->size = metaPtr->size + nextMetaPtr->size + metadataSize;
            continue;

        }
        // advance metadata ptr
        metaPtr = (metadata*)((char*) metaPtr + metaPtr->size + metadataSize);

    }
}

/**
 * This is a helper function for debugging. It displays the contents of each metadata node.
 */
void printMeta() {
    printf("Last address: %p\n", lastAddress);
    metadata* metaPtr = (metadata*) myblock;
    int counter = 0;
    printf("------------------------------------\n");
    while((char*)metaPtr + metaPtr->size + metadataSize <= lastAddress) {
        printf("Metadata num: %d at address %p\n", counter, metaPtr);
        printf("In use: ");
        if(metaPtr->inUse) {
            printf("Yes\n");
        } else {
            printf("No\n");
        }
        printf("Size: %d\n", metaPtr->size);
        printf("Next: %p\n", (char*) metaPtr + metaPtr->size + metadataSize);
        printf("------------------------------------\n");
        metaPtr = (metadata*) (((char*) metaPtr) + metaPtr->size + metadataSize);
        counter++;
        // if((char*)metaPtr + metaPtr->size + metadataSize == lastAddress) {
        //     break;
        // }
    }
    printf("\n\n");
}