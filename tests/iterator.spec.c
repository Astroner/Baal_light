#include "tests-new.h"

#include "tests.h"

#include "../Zeb.h"

DESCRIBE(iterator) {
    IT("correctly counts entities") {
        Zeb_define(test, 1, 10);

        Zeb_alloc(test);
        char* a = Zeb_alloc(test);
        Zeb_alloc(test);
        Zeb_alloc(test);

        Zeb_free(test, a);

        int free = 0;
        int busy = 0;

        ZebIterator iter;
        ZebIterator_init(&iter, test);

        while(ZebIterator_next(&iter)) {
            if(iter.isFree) {
                free++;
            } else {
                busy++;
            }
        }

        EXPECT(free) TO_BE(7)
        EXPECT(busy) TO_BE(3)
    }

    IT("correctly handles frees") {
        Zeb_define(test, sizeof(int), 5);

        int* a = Zeb_alloc(test);
        *a = 5;

        int* b = Zeb_alloc(test);
        *b = 7;

        int* c = Zeb_alloc(test);
        *c = 11;

        int* d = Zeb_alloc(test);
        *d = 13;

        int* e = Zeb_alloc(test);
        *e = 17;

        Zeb_free(test, c);
        Zeb_free(test, d);

        int result = 1;
        ZEB_ITERATE(test, info, int*, num) {
            if(info.isFree) continue;

            result *= *num;
        }

        EXPECT(result) TO_BE(595);
    }
}