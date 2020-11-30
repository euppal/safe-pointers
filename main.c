//
//  main.c
//  autoptrs-in-c
//

// This example program uses auto functions to allocate a string and then another function borrows this string.
// With DEBUG_OWNED_POINTERS defined (see auto-internal.h), you will get this dump:
/*
 * Hello, world!
 * Freeing pointers allocated in my_borrowing_func:
 * [1] Freeing pointer (0x1007a7730 or whatever address the pointer is)
 * Freeing pointers allocated in my_func:
 * [1] Freeing pointer (0x0)
 */

#include <stdio.h>
#include <string.h>
#include "auto.h"

void my_borrowing_func(borrowed_args(char* str)) {
    auto_begin();
    borrow(str);
    strcpy(str, "Hello, ");
    str = ralloc(str, 15);
    strcat(str, "world!");
    printf("%s\n", str);
    auto_end();
}

void my_func() auto_func({
     char* str = (char*)alloc(8);
     my_borrowing_func(borrow_args(str));
})

int main(int argc, const char * argv[]) {
    my_func();
    return 0;
}
