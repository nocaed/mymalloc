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
void* mymalloc(size_t size, char* file, int line);

void myfree(void* ptr, char* file, int line);

bool isFirstCall();

static char myblock[4096];

void resetMetadata(metadata* ptr);

void collapse();

void printMeta();