//
//  auto-internal.h
//  autoptrs-in-c
//

#ifndef auto_internal_h
#define auto_internal_h

// Define this if you wish to track the deallocation of your pointers.
//#define DEBUG_OWNED_POINTERS

#include "auto.h"

// See auto.c for function information.
void append_to_owned(ownership_manager_t* manager, void* ptr);

#endif /* auto_internal_h */
