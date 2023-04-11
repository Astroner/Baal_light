#define BAAL_IMPLEMENTATION
#include "./Baal.h"

#include <assert.h>
#include <stdio.h>

void createsStaticAllocator() {
    Baal_createStatic(baal, sizeof(int), 10);
    
    assert(baal->blockSize == sizeof(int));
    assert(baal->blocksNumber == 10);
    assert(baal->cursor == baal->buffer);
    assert(baal->freeStack.size == 0);
}

void createsAndDestroysDynamicAllocator() {
    Baal* baal = Baal_create(sizeof(int), 10);

    assert(baal->blockSize == sizeof(int));
    assert(baal->blocksNumber == 10);
    assert(baal->cursor == baal->buffer);
    assert(baal->freeStack.size == 0);

    Baal_destroy(baal);
}

void movesCursorCorrectly() {
    Baal_createStatic(baal, sizeof(int), 10);

    int* num1 = Baal_alloc(baal);
    int* num2 = Baal_alloc(baal);
    int* num3 = Baal_alloc(baal);

    assert(baal->cursor == (char*)num3 + sizeof(int));
    
    Baal_free(baal, num3);
    Baal_free(baal, num2);
    Baal_free(baal, num1);

    assert(baal->cursor == baal->buffer);
}

void freeStackWorksCorrectly() {
    Baal_createStatic(baal, sizeof(int), 10);

    int* num1 = Baal_alloc(baal);
    int* num2 = Baal_alloc(baal);
    int* num3 = Baal_alloc(baal);

    Baal_free(baal, num1);
    assert(baal->freeStack.size == 1);
    assert(baal->freeStack.buffer[0] == num1);

    int* num4 = Baal_alloc(baal);
    assert(num4 == num1);
    assert(baal->freeStack.size == 0);

    Baal_free(baal, num3);
    Baal_free(baal, num2);

    assert(baal->freeStack.size == 0);
    
    Baal_free(baal, num4);
    assert(baal->cursor == baal->buffer);
    assert(baal->freeStack.size == 0);
}

void correctlyHandlesEndOfTheBlocks() {
    Baal_createStatic(baal, sizeof(int), 4);
    Baal_alloc(baal);
    Baal_alloc(baal);
    Baal_alloc(baal);
    Baal_alloc(baal);
    int* last = Baal_alloc(baal);

    assert(last == NULL);
}

void clearsMemory() {
    Baal_createStatic(baal, sizeof(int), 4);

    int* first = Baal_alloc(baal);
    Baal_alloc(baal);
    Baal_alloc(baal);
    Baal_alloc(baal);

    Baal_free(baal, first);
    Baal_clear(baal);

    assert(baal->cursor == baal->buffer);
    assert(baal->freeStack.size == 0);
}

#define TEST(funcName)\
    printf("%s - ", #funcName);\
    funcName();\
    printf("PASS\n");\

int main(void) {
    printf("\nLaunching test cases:\n");

    TEST(createsStaticAllocator);
    TEST(createsAndDestroysDynamicAllocator);
    TEST(movesCursorCorrectly);
    TEST(freeStackWorksCorrectly);
    TEST(correctlyHandlesEndOfTheBlocks);
    TEST(clearsMemory);

    printf("\n");
    return 0;
}