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
