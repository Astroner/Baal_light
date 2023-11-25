# Hi There
This is STB-like library for fixed-size block memory allocation.

# Include
As with any header only library, you need to include *Zeb.h* library to your project and define **ZEB_IMPLEMENTATION** before **include** statement to include the implementation.

# Create instance
## Macros
**Zeb_define(NAME, BLOCK_SIZE, BLOCKS_NUMBER)**
 - **NAME** - **Zeb** instance name
 - **BLOCK_SIZE** - size of a single block
 - **BLOCKS_NUMBER** - total number of blocks

This macro expands into several variable definitions and at the end it will define pointer to **Zeb** instance with provided **NAME**

```c
#define ZEB_IMPLEMENTATION
#include "zeb.h"

Zeb_define(globalAllocator, 10, 10);

int main(void) {
    Zeb_define(localAllocator, 10, 10);

    return 0;
}
```
**Zeb_defineStatic(NAME, BLOCK_SIZE, BLOCKS_NUMBER)** does the same thing with **Zeb_define(NAME, BLOCK_SIZE, BLOCKS_NUMBER)** but adds modifier **static** to variable definitions.

## Functions
**Zeb* Zeb_create(size_t blockSize, size_t blocksNumber)**
 - **returns** - **Zeb** instance
 - **blockSize** - size of a single block
 - **blocksNumber** - maximum number of blocks to be allocated

By default this function uses **malloc** to allocate required memory from heap, but it can be changed by defining **ZEB_STD_MALLOC** macro with desired function before including the implementation.

**void Zeb_destroy(Zeb*)** function destroys provided **Zeb** instance and frees allocated memory. Basically this function is just an alias for std **free()**, which can be replaced by defining **ZEB_STD_FREE** with desired function.

```c
#define ZEB_IMPLEMENTATION
#include "Zeb.h"

int main(void) {
    Zeb* zeb = Zeb_create(10, 20);

    // Do smth

    Zeb_destroy(zeb);

    return 0;
}
```

# Init instance
__Zeb* Zeb_init(Zeb* zeb, char* buffer, size_t bufferLength, size_t blockSize)__
 - **returns** - **NULLABLE** - pointer to provided zeb empty structure
 - **buffer** - allocator memory region
 - **bufferLength** - length of provided **buffer**
 - **blockSize** - single memory block size

Use this function if you want to initialize **Zeb** instance on a custom memory region.
If insufficient buffer is provided **Zeb_init()** returns **NULL**.
To get total size of required memory for specific block size and blocks number use macro **Zeb_getTotalMemorySize(blockSize, blocksNumber)**
> Note that block size cannot be less than **ZEB_BLOCK_MIN_SIZE** and even if you provide smaller size it will be rounded to min size with **Zeb_roundBlockSize(blockSize)** function.

```c
#define ZEB_IMPLEMENTATION
#include "Zeb.h"

#include <assert.h>

char memoryBuffer[1000];

int main(void) {
    Zeb allocator;
    size_t blockSize = 10;
    size_t desiredBlocksNumber = 20;
    size_t memoryRequired = Zeb_getTotalMemorySize(blockSize, desiredBlocksNumber);

    assert(memoryRequired <= sizeof(memoryBuffer));

    Zeb* zeb = Zeb_init(&allocator, memoryBuffer, memoryRequired, blockSize);

    return 0;
}
```

# Allocate
Use **Zeb_alloc()** to allocate next available block and **Zeb_free()** to free it. **Zeb_alloc()** returns **NULL** if no free blocks left.
```c
#include <stdio.h>

#define ZEB_IMPLEMENTATION
#include "Zeb.h"

typedef struct Obj {
    int index;
    int counter;
} Obj;

Zeb_defineStatic(objects, sizeof(Obj), 10);

int main(void) {
    Obj* obj = Zeb_alloc(objects);

    obj->index = 2;
    obj->counter = 10;

    printf("Index: %d, Counter: %d\n", obj->index, obj->counter);

    Zeb_free(objects, obj);

    return 0;
}
```
# Clear
Use **Zeb_clear()** to fully clear the buffer.
> Note, that the function doesn't actually resets the memory, it just resets the pointers pool.
```c
#include <stdio.h>

#define ZEB_IMPLEMENTATION
#include "Zeb.h"

typedef struct Obj {
    int index;
    int counter;
} Obj;

Zeb_defineStatic(objects, sizeof(Obj), 10);

int main(void) {
    Obj* obj = Zeb_alloc(objects);

    obj->index = 2;
    obj->counter = 10;

    printf("Index: %d, Counter: %d\n", obj->index, obj->counter);

    Zeb_clear(objects);

    return 0;
}
```

## Print
You can use **Zeb_print()** function to print status of memory blocks:
```c
#define ZEB_IMPLEMENTATION
#include "Zeb.h"

Zeb_defineStatic(numbers, sizeof(int), 10);

int main(void) {
    Zeb_alloc(numbers);

    Zeb_print(numbers);
    // [00] BUSY
    // [01] FREE
    // [02] FREE
    // [03] FREE
    // [04] FREE
    // [05] FREE
    // [06] FREE
    // [07] FREE
    // [08] FREE
    // [09] FREE

    return 0;
}
```