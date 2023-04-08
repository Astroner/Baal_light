#define BAAL_IMPLEMENTATION
#include "Baal.h"

Baal_createStatic(numbers, sizeof(int), 10);

int main(void) {
    Baal_alloc(numbers);

    Baal_print(numbers);

    Baal_clear(numbers);

    return 0;
}