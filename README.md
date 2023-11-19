# Hi There
This is STB-like library for block memory allocation.

# Include
As with any header only library, you need to include *Baal_light.h* library to your project and define **BAAL_IMPLEMENTATION** before **include** statement to include the implementation.

# Usage
## Create instance
First, create **Baal** instance:
 - **Baal_createStatic(name, blockSize, blocksNumber)** - **macro** - statically creates pointer to Baal instance in current context.
 - **Baal_create(blockSize, blocksNumber)** - **function** - returns pointer to new Baal instance. Use **Baal_destroy** to free memory. 

```c
#define BAAL_IMPLEMENTATION
#include "Baal_light.h"

// static way
Baal_createStatic(baalStatic, sizeof(int), 10);

// dynamic way
Baal* baalDynamic = Baal_create(sizeof(int), 10);
Baal_destroy(baalDynamic);
```
## Allocate
Use **Baal_alloc()** to allocate next available block and **Baal_free()** to free it. **Baal_alloc()** returns **NULL** if no free blocks left.
```c
#include <stdio.h>

#define BAAL_IMPLEMENTATION
#include "Baal_light.h"

typedef struct Obj {
    int index;
    int counter;
} Obj;

Baal_createStatic(objects, sizeof(Obj), 10);

int main(void) {
    Obj* obj = Baal_alloc(objects);

    obj->index = 2;
    obj->counter = 10;

    printf("Index: %d, Counter: %d\n", obj->index, obj->counter);

    Baal_free(objects, obj);

    return 0;
}
```
## Clear
Use **Baal_clear()** to fully clear the buffer.
> Note, that the function doesn't actually resets the memory, it just resets the pointers pool.
```c
#include <stdio.h>

#define BAAL_IMPLEMENTATION
#include "Baal_light.h"

typedef struct Obj {
    int index;
    int counter;
} Obj;

Baal_createStatic(objects, sizeof(Obj), 10);

int main(void) {
    Obj* obj = Baal_alloc(objects);

    obj->index = 2;
    obj->counter = 10;

    printf("Index: %d, Counter: %d\n", obj->index, obj->counter);

    Baal_clear(objects);

    return 0;
}
```

## Print
You can use **Baal_print()** function to print status of memory blocks:
```c
#define BAAL_IMPLEMENTATION
#include "Baal_light.h"

Baal_createStatic(numbers, sizeof(int), 10);

int main(void) {
    Baal_alloc(numbers);

    Baal_print(numbers);
    // Blocks:
    // 0 : 0x103174060 : busy
    // 1 : 0x103174064 : free
    // 2 : 0x103174068 : free
    // 3 : 0x10317406c : free
    // 4 : 0x103174070 : free
    // 5 : 0x103174074 : free
    // 6 : 0x103174078 : free
    // 7 : 0x10317407c : free
    // 8 : 0x103174080 : free
    // 9 : 0x103174084 : free
    // Free stack size: 0

    Baal_clear(numbers);

    return 0;
}
```