#ifndef _HEADER_H
//#define malloc(x) mymalloc(x, __FILE__, __LINE__)
//#define free(x) myfree(x, __FILE__, __LINE__)
#include <stdlib.h>
#include <stdio.h>
#endif
typedef enum {false, true} bool;

typedef struct _meta {
    short tag;
    char inUse;
    short size;
    struct _meta* next; // maybe void*?

} metadata;
void* mymalloc(int size);

void myfree(void* ptr);

bool isFirstCall();

static char myblock[4096];