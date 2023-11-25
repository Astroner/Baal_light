#define ZEB_IMPLEMENTATION
#include "../../Zeb.h"

int main(void) {
    Zeb_define(test, 2, 4);

    char* p1 = Zeb_alloc(test);
    char* p2 = Zeb_alloc(test);
    Zeb_alloc(test);

    Zeb_free(test, p1);
    Zeb_print(test);

    Zeb_free(test, p2);
    Zeb_print(test);

    return 0;
}
