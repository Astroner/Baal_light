#include "tests-new.h"

#define MULTI_TEST
#include "tests.h"

CREATE_PRINTF_LIKE_FUNCTION(testPrint, 500)

#include "general.spec.c"
#include "iterator.spec.c"

#define ZEB_DEBUG
#define ZEB_STD_PRINT testPrint
#define ZEB_IMPLEMENTATION
#include "../Zeb.h"

RUN_TESTS(general, iterator)