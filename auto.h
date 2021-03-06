//
//  auto.h
//  autoptrs-in-c
//

#ifndef auto_h
#define auto_h

// stddef.h for `size_t`.
#include <stddef.h>

// The structure representing an ownership manager.
typedef struct {
    size_t length;
    size_t capacity;
    void** pointers;
    void* return_value;
    const char* function_name;
} ownership_manager_t;

// Information about the workings of these functions can be found in the C file.
void setup_owned_pointers(ownership_manager_t* manager);
void free_owned_pointers(ownership_manager_t* manager);
void* allocate_owned_pointer(size_t length, ownership_manager_t* manager);
void* reallocate_owned_pointer(void* ptr, size_t length, ownership_manager_t* manager);
void move_ownership(ownership_manager_t* dest, ownership_manager_t* src, void* pointer);
void append_to_owned(ownership_manager_t* manager, void* ptr);
void auto_failure(ownership_manager_t* manager, const char* msg);

// These macros define the start and end of auto functions. Any pointer `alloc`ed within the space will be automatically managed.
#define auto_begin() ownership_manager_t __OWNED_POINTERS; (__OWNED_POINTERS.function_name = __FUNCTION__, setup_owned_pointers(&__OWNED_POINTERS))
#define auto_end() free_owned_pointers(&__OWNED_POINTERS)

// This macro is the core of the auto function. You wrap the function body in this macro to enable auto functio features. An auto function cannot return a value.
#define auto_func(...) {\
    auto_begin();\
    __VA_ARGS__\
    auto_end();\
}

// This macro is similar to the above one except it allows for returning values.
#define auto_func_ret(ret_type, ...) {\
    auto_begin();\
    __VA_ARGS__\
    auto_end();\
    return (ret_type)__OWNED_POINTERS.return_value;\
}

// Use this macro rather than `malloc` to enable auto functions.
#define alloc(n) allocate_owned_pointer(n, &__OWNED_POINTERS)

// Use this macro rather than `realloc` to enable auto functions.
#define ralloc(ptr, n) reallocate_owned_pointer(ptr, n, &__OWNED_POINTERS)

// Wrap the function signature in this macro to make it a borrowing function.
#define borrowing_auto_func(func_name, ...) func_name(ownership_manager_t* __PREVIOUS_OWNED_POINTERS, ## __VA_ARGS__)

// Use this macro to define a borrowing function without wrapping it in `borrowing_auto_func`
#define borrowed_args(...) ownership_manager_t* __PREVIOUS_OWNED_POINTERS, ## __VA_ARGS__

// Use this macro to call a function that can borrow with the specified name and arguments.
#define call_borrowing_func(func_name, ...) func_name(&__OWNED_POINTERS, ## __VA_ARGS__)

// Use this macro to call a borrowing function without wrapped it in `call_borrowing_func`
#define borrow_args(...) &__OWNED_POINTERS, ## __VA_ARGS__

// This macro (semicolon-terminated) will borrow a pointer declared in the auto function caller.
#define borrow(ptr) move_ownership(&__OWNED_POINTERS, __PREVIOUS_OWNED_POINTERS, ptr)

// This macro (semicolon-terminated) will return ownership of a pointer declared in the auto function caller back to the caller.
#define unborrow(ptr) move_ownership(__PREVIOUS_OWNED_POINTERS, &__OWNED_POINTERS, ptr)

// This macro will remove any auto function features from affecting this pointer.
#define noauto(ptr) move_ownership(NULL, &__OWNED_POINTERS, ptr)

// This macro will (re)enable auto function features for this pointer.
#define makeauto(ptr) append_to_owned(&__OWNED_POINTERS, ptr)

#define auto_return(value) if (sizeof(value) > 8) { auto_failure(&__OWNED_POINTERS, "Auto functions cannot return a value larger than 64 bits."); } else { __OWNED_POINTERS.return_value = (void*)(value); }

#endif /* auto_h */
