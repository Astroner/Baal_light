#include <stdio.h>

#define ZEB_IMPLEMENTATION
#include "../../Zeb.h"

int main(void) {
    Zeb_define(test, 2, 10);

    printf("Block size is equal to MIN: %d\n", test->blockSize == ZEB_BLOCK_MIN_SIZE);

    return 0;
}
