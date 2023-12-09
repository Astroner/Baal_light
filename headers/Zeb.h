#if !defined(ZEB_H)
#define ZEB_H

#include <stddef.h>

#define ZEB_BLOCK_MIN_SIZE sizeof(void*)

#define Zeb_roundBlockSize(blockSize) (blockSize < ZEB_BLOCK_MIN_SIZE ? ZEB_BLOCK_MIN_SIZE : blockSize)
#define Zeb_getTotalMemorySize(blockSize, blocksNumber) (Zeb_roundBlockSize(blockSize) * blocksNumber)

typedef struct Zeb {
    size_t blockSize;
    size_t blocksNumber;
    char* buffer;
    char** cursor;
} Zeb;

#define Zeb_define(NAME, BLOCK_SIZE, BLOCKS_NUMBER)\
    char NAME##__buffer[Zeb_getTotalMemorySize(BLOCK_SIZE, BLOCKS_NUMBER)] = {\
        [0] = 0,\
        [1] = 0,\
        [2] = 0,\
        [3] = 0,\
        [4] = 0,\
        [5] = 0,\
        [6] = 0,\
        [7] = 0,\
        [8] = 0,\
    };\
    Zeb NAME##__data = {\
        .blockSize = Zeb_roundBlockSize(BLOCK_SIZE),\
        .blocksNumber = BLOCKS_NUMBER,\
        .buffer = NAME##__buffer,\
        .cursor = (char**)NAME##__buffer,\
    };\
    Zeb* NAME = &NAME##__data;\

#define Zeb_defineStatic(NAME, BLOCK_SIZE, BLOCKS_NUMBER)\
    static char NAME##__buffer[Zeb_getTotalMemorySize(BLOCK_SIZE, BLOCKS_NUMBER)] = {\
        [0] = 0,\
        [1] = 0,\
        [2] = 0,\
        [3] = 0,\
        [4] = 0,\
        [5] = 0,\
        [6] = 0,\
        [7] = 0,\
        [8] = 0,\
    };\
    static Zeb NAME##__data = {\
        .blockSize = Zeb_roundBlockSize(BLOCK_SIZE),\
        .blocksNumber = BLOCKS_NUMBER,\
        .buffer = NAME##__buffer,\
        .cursor = (char**)NAME##__buffer,\
    };\
    static Zeb* NAME = &NAME##__data;\

Zeb* Zeb_init(Zeb* zeb, char* buffer, size_t bufferLength, size_t blockSize);
Zeb* Zeb_create(size_t blockSize, size_t blocksNumber);
void Zeb_destroy(Zeb* zeb);

void* Zeb_alloc(Zeb* zeb);
void Zeb_free(Zeb* zeb, void* block);
void Zeb_clear(Zeb* zeb);

void Zeb_print(const Zeb* zeb);

#define ZEB_ITERATE(ZEB, INFO_NAME, TYPE, VARIABLE)\
    ZebIterator INFO_NAME;\
    ZebIterator_init(&INFO_NAME, ZEB);\
    TYPE VARIABLE;\
    while((VARIABLE = ZebIterator_next(&INFO_NAME)))

typedef struct ZebIterator {
    struct {
        const Zeb* zeb;
        const char* end;
    } __internal;

    size_t index;
    int isFree;
    void* current;
} ZebIterator;

void ZebIterator_init(ZebIterator* iterator, const Zeb* zeb);
void* ZebIterator_next(ZebIterator* iterator);
void ZebIterator_reset(ZebIterator* iterator);

#endif // ZEB_H
