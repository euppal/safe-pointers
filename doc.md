Safe Pointers operates with auto functions, i.e. functions whose pointer contents are managed and borrowable. To create an auto function, one simply wraps the funciton body in `auto_func(body)`. For returning values up to 64 bits, use `auto_func_ret(type, body)`.

Rather than `malloc`, `realloc` and `free`, with Safe Pointers you simply use the macros `alloc` and `ralloc`. The freeing is managed automatically (hence the name "auto" function). 

```c
// Normal auto function
void func1() auto_func({
    char* str = alloc(15);
    strcpy(str, "Hello, ");
    strcat(str, "world!");
})

// Value-returning auto function
int func2() auto_func_ret(int, {
    char* str = alloc(15);
    strcpy(str, "Hello, ");
    strcat(str, "world!");
    auto_return(42);
})
```

If you pass an `alloc`ed pointer to another non-auto function or a normal auto function, then when that function exits the pointer will still be valid. To enable "borrowing" of ownership from one function to another, ue the `borrow` and `unborrow` macros.

void borrowing(borrowed_args(char* str)) auto_func({
    borrow(str);
    strcpy(str, "Hello, ");
    strcat(str, "world!");
    printf("%s\n", str);
})

void normal() auto_func({
     char* str = (char*)alloc(14);
     my_borrowing_func(borrow_args(str));
})
```

As you can see, syntactically the only difference is the introduction of the `borrowed_args` and `borrow_args` keywords, used in the definition and the call respectively.

You can disable and (re)enable auto function features for pointers at any point within an  `auto_func` body using the `noauto` and `makeauto` macros. 
