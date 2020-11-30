//
//  auto.h
//  autoptrs-in-c
//

#ifndef auto_h
#define auto_h

// stddef.h for size_t.
#include <stddef.h>

// The structure representing an ownership manager.
typedef struct {
    size_t length;
    size_t capacity;
    void** pointers;
    const char* function_name;
} ownership_manager_t;

// Information about the workings of these functions can be found in the C file.
void setup_owned_pointers(ownership_manager_t* manager);
void free_owned_pointers(ownership_manager_t* manager);
void* allocate_owned_pointer(size_t length, ownership_manager_t* manager);
void move_ownership(ownership_manager_t* dest, ownership_manager_t* src, void* pointer);

// This macro is the core of the auto function. You wrap the function body in this macro to enable auto functio features. An auto function cannot return a value.
#define auto_func(body) {\
    ownership_manager_t __OWNED_POINTERS;\
    __OWNED_POINTERS.function_name = __FUNCTION__;\
    setup_owned_pointers(&__OWNED_POINTERS);\
    body\
    free_owned_pointers(&__OWNED_POINTERS);\
}

// Use this function rather than malloc to enable auto functions.
#define alloc(n) allocate_owned_pointer(n, &__OWNED_POINTERS)

// Wrap the function signature in this macro to make it a borrowing function.
#define borrowing_auto_func(func_name, ...) func_name(ownership_manager_t* __PREVIOUS_OWNED_POINTERS, ## __VA_ARGS__)

// Use this macro to call a function that can borrow with the specified name and arguments.
#define call_borrowing_func(func_name, ...) func_name(&__OWNED_POINTERS, ## __VA_ARGS__)

// This macro (semicolon-terminated) will borrow a pointer declared in the auto function caller.
#define borrow(ptr) move_ownership(&__OWNED_POINTERS, __PREVIOUS_OWNED_POINTERS, ptr)

// This macro (semicolon-terminated) will return ownership of a pointer declared in the auto function caller back to the caller.
#define unborrow(ptr) move_ownership(__PREVIOUS_OWNED_POINTERS, &__OWNED_POINTERS, ptr)

#endif /* auto_h */
