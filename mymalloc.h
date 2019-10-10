#ifndef _HEADER_H
#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)
#endif

void* mymalloc(int size);

void myfree(void* ptr);

static char myblock[4096];