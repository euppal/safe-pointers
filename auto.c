//
//  auto.c
//  autoptrs-in-c
//

#include "auto-internal.h"
#include <stdlib.h>
#include <stdio.h>


// Allocates space for 4 pointers at the start to allow for growth based on capacity.
// If 0 bytes were allocated, (1) the first reallocation would be difficult and (2) you could not multiply the capacity to increase it.
void setup_owned_pointers(ownership_manager_t* manager) {
    manager->length = 0;
    manager->capacity = 4;
    manager->pointers = (void**)malloc(sizeof(void*) * 4);
}

// This function's ownership manager will first see if there is a NULL slot due to a borrowed pointer and insert it there. Otherwise, it will append it to the stored pointers array.
void append_to_owned(ownership_manager_t* manager, void* ptr) {
    for (size_t i = 0; i < manager->length; i++) {
        if (manager->pointers[i] == NULL) {
            manager->pointers[i] = ptr;
            return;
        }
    }
    const size_t new_length = manager->length + 1;
    if (new_length > manager->capacity) {
        manager->capacity *= 2;
        manager->pointers = (void**)realloc((void*)manager->pointers, sizeof(void*) * manager->capacity);
    }
    manager->pointers[manager->length++] = ptr;
}

// This function will `malloc` the appropriate size and then add it to the function's ownership manager.
void* allocate_owned_pointer(size_t length, ownership_manager_t* manager) {
    void* ptr = malloc(length);
    append_to_owned(manager, ptr);
    return ptr;
}

// This function will `realloc` the appropriate size and then re-add it to the function's ownership manager.
void* reallocate_owned_pointer(void* ptr, size_t length, ownership_manager_t* manager) {
    void* new_ptr = realloc(ptr, length);
    move_ownership(NULL, manager, ptr);
    append_to_owned(manager, new_ptr);
    return new_ptr;
}

// Called at the end of every auto function, this will free each pointer still owned by the function.
void free_owned_pointers(ownership_manager_t* manager) {
    #ifdef DEBUG_OWNED_POINTERS
    printf("Freeing pointers allocated in %s:\n", manager->function_name);
    #endif
    for (size_t i = 0; i < manager->length; i++) {
        #ifdef DEBUG_OWNED_POINTERS
        printf("[%zu] Freeing pointer (%p)\n", i + 1, manager->pointers[i]);
        #endif
        free(manager->pointers[i]);
    }
    free((void*)manager->pointers);
}

// This function will transfer ownership of a pointer from one function's ownership manager to another's/
void move_ownership(ownership_manager_t* dest, ownership_manager_t* src, void* pointer) {
    if (dest != NULL) {
        append_to_owned(dest, pointer);
    }
    for (size_t i = 0; i < src->length; i++) {
        if (src->pointers[i] == pointer) {
            src->pointers[i] = NULL;
        }
    }
}

// This function will exit the program upon call.
void auto_failure(ownership_manager_t* manager, const char* msg) {
    puts(msg);
    free_owned_pointers(manager);
    exit(1);
}
