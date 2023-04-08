#if !defined(ALLOCATOR_H)
#define ALLOCATOR_H

#include <stdlib.h>

typedef struct Baal_FreeStack {
    size_t size;
    void** buffer;
} Baal_FreeStack;

typedef struct Baal {
    size_t blockSize;
    size_t blocksNumber;
    char* buffer;
    char* cursor;
    Baal_FreeStack freeStack;
} Baal;

#define Baal_createStatic(name, blockSizeArg, blocksNumberArg)\
    char name##__buffer[blockSizeArg * blocksNumberArg];\
    void* name##__stack__buffer[blocksNumberArg];\
    Baal name##__data = {\
        .blockSize = blockSizeArg,\
        .blocksNumber = blocksNumberArg,\
        .cursor = name##__buffer,\
        .buffer = name##__buffer,\
        .freeStack = {\
            .size = 0,\
            .buffer = name##__stack__buffer,\
        }\
    };\
    Baal* name = &name##__data;\

Baal* Baal_create(size_t blockSize, size_t blocksNumber);
void Baal_destroy(Baal*);

void* Baal_alloc(Baal* allocator);
int Baal_free(Baal* allocator, void* block);
void Baal_clear(Baal* allocator);
void Baal_print(Baal* allocator);

#endif // ALLOCATOR_H
