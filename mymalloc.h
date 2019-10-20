#ifndef _MYMALLOC_H
#define _MYMALLOC_H
#include <stdlib.h>
#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

#endif
typedef enum {false, true} bool;

typedef struct _meta {
    short tag;
    unsigned char inUse: 1;
    unsigned short size: 13;
//    struct _meta* next; // maybe void*?

} metadata;
void* mymalloc(size_t size, char* file, int line);

void myfree(void* ptr, char* file, int line);


bool isMetadata(void* address);

void resetMetadata(metadata* ptr);

void collapse();

void printMeta();