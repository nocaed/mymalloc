#ifndef _HEADER_H
#include <stdlib.h>
#include <stdio.h>
#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

#endif
typedef enum {false, true} bool;

typedef struct _meta {
    short tag;
    char inUse;
    short size;
    struct _meta* next; // maybe void*?

} metadata;
void* mymalloc(size_t size);

void myfree(void* ptr);

bool isFirstCall();

static char myblock[4096];

void resetMetadata(metadata* ptr);