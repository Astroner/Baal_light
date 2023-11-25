#define ZEB_IMPLEMENTATION
#include "../../Zeb.h"

int main(void) {
    Zeb_define(test, 2, 4);
    
    Zeb_alloc(test);
    Zeb_alloc(test);

    Zeb_print(test);

    return 0;
}
