# Safe Pointers

Safe Pointers contains macros which wrap parts of functions and enable automatic deallocation of pointers after they go out of scope. Borrowing and "unborrowing" are supported, so pointers don't necessarily have to remain within a specific scope. You can also eject the pointer from any management with the `noauto` macro. Likewise, you can bring a pointer back under management with the `makeauto` macro.

This code will print "Hello, world" and not leak any memory.

```c
#include <stdio.h>
#include <string.h>
#include "auto.h"

void borrowing_auto_func(my_borrowing_func, char* str) auto_func({
    borrow(str);
    strcpy(str, "Hello, ");
    strcat(str, "world!");
    printf("%s\n", str);
})

void my_func() auto_func({
    char* str = (char*)alloc(14);
    call_borrowing_func(my_borrowing_func, str);
})

int main(int argc, const char * argv[]) {
    my_func();
    return 0;
}
```

This is licensed under the GPLv3. The GPLv3 is a free, copyleft license for software and other kinds of works.
