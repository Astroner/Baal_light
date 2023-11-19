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
#if defined(BAAL_IMPLEMENTATION)

#include <stdio.h>
#include <stdlib.h>

Baal* Baal_create(size_t blockSizeArg, size_t blocksNumberArg) {
    char* memory = malloc(
        sizeof(Baal)
        + blockSizeArg * blocksNumberArg 
        + blocksNumberArg * sizeof(void*) 
    );

    if(!memory) return NULL;

    Baal* result = (void*)memory;
    char* buffer = memory + sizeof(Baal);
    void** stack__buffer = (void**)(buffer + blockSizeArg * blocksNumberArg);

    result->blockSize = blockSizeArg;
    result->blocksNumber = blocksNumberArg;
    result->cursor = buffer;
    result->buffer = buffer;
    result->freeStack.size = 0;
    result->freeStack.buffer = stack__buffer;
    
    return result;
}

void Baal_destroy(Baal* baal) {
    free(baal);
}

void* Baal_alloc(Baal* allocator) {
    if(
        allocator->cursor == allocator->buffer + allocator->blocksNumber * allocator->blockSize &&
        allocator->freeStack.size == 0
    ) {
        return NULL;
    }
    if(allocator->freeStack.size > 0) {
        allocator->freeStack.size--;
        void* value = allocator->freeStack.buffer[allocator->freeStack.size];

        return value;
    }

    void* value = allocator->cursor;

    allocator->cursor += allocator->blockSize;

    return value;
}

int Baal_free(Baal* allocator, void* block) {
    if((char*)block < allocator->buffer || (char*)block > allocator->buffer + allocator->blocksNumber * allocator->blockSize) {
        printf("Pointer is not in allocator\n");
        return 1;
    }

    if((char*)block == allocator->cursor - allocator->blockSize) {
        allocator->cursor -= allocator->blockSize;
    } else {
        allocator->freeStack.buffer[allocator->freeStack.size] = block;
        allocator->freeStack.size++;
    }

    // if(allocator->cursor == allocator->buffer) {
    //     allocator->freeStack.size = 0;
    // }

    return 0;
}

void Baal_clear(Baal* allocator) {
    allocator->freeStack.size = 0;
    allocator->cursor = allocator->buffer;
}

void Baal_print(Baal* allocator){
    printf("Blocks:\n");
    for(size_t i = 0; i < allocator->blocksNumber; i++) {
        char* currentPointer = allocator->buffer + i * allocator->blockSize;
        int status = allocator->buffer == allocator->cursor ? 0 : currentPointer < allocator->cursor;
        if(status) {
            for(size_t j = 0; j < allocator->freeStack.size; j++) {
                if(allocator->freeStack.buffer[j] == currentPointer) {
                    status = 0;
                    break;
                }
            }
        }
        printf("%zu : %p : %s\n", i, currentPointer, status ? "busy" : "free");
    }
    printf("Free stack size: %zu\n", allocator->freeStack.size);
}
#endif
