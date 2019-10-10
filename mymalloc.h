#ifndef _HEADER_H
#define malloc(x) mymalloc(x)
#define free(x) myfree(x)
#endif

void* mymalloc(int size);

void myfree(void* ptr);