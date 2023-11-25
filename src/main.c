#include "Zeb.h"

#include <stdio.h>

Zeb_define(a, 2, 2);

int main(void) {
    Zeb* test = Zeb_create(2, 5);

    char* a = Zeb_alloc(test);
    char* b = Zeb_alloc(test);

    Zeb_free(test, a);
    Zeb_free(test, b);

    Zeb_alloc(test);

    Zeb_print(test);
    
    return 0;
}