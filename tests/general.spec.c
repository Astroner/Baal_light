#include "tests-new.h"

#define WITH_AFTER_EACH
#include "tests.h"

#include "../Zeb.h"

AFTER_EACH(general) {
    testPrint__reset();
}

DESCRIBE(general) {
    IT("works") {
        Zeb_define(test, 2, 10);

        EXPECT(test->blockSize) TO_BE(ZEB_BLOCK_MIN_SIZE)
    }

    IT("allocates") {
        Zeb_define(test, 2, 4);
    
        Zeb_alloc(test);
        Zeb_alloc(test);

        char* printed = testPrint__nextString();
        Zeb_print(test);

        EXPECT(printed) TO_BE_STRING(
            "[00] BUSY\n"
            "[01] BUSY\n"
            "[02] FREE\n"
            "[03] FREE\n"
        )
    }

    IT("deallocates") {
        Zeb_define(test, 2, 4);

        char* p1 = Zeb_alloc(test);
        char* p2 = Zeb_alloc(test);
        Zeb_alloc(test);

        Zeb_free(test, p1);

        char* firstFree = testPrint__nextString();
        Zeb_print(test);
        EXPECT(firstFree) TO_BE_STRING(
            "[00] FREE\n"
            "[01] BUSY\n"
            "[02] BUSY\n"
            "[03] FREE\n"
        );

        char* secondFree = testPrint__nextString();
        Zeb_free(test, p2);
        Zeb_print(test);
        EXPECT(secondFree) TO_BE_STRING(
            "[00] FREE\n"
            "[01] FREE\n"
            "[02] BUSY\n"
            "[03] FREE\n"
        )
    }
}
