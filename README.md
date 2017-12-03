# ceraii (C Explicit RAII) (WIP - Work in progress)
[![Build Status](https://travis-ci.org/seleznevae/ceraii.svg?branch=master)](https://travis-ci.org/seleznevae/ceraii)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Coverage Status](https://coveralls.io/repos/github/seleznevae/ceraii/badge.svg?branch=master)](https://coveralls.io/github/seleznevae/ceraii?branch=master)

In software projects written in C there's always a lot of boiler plate code for releasing allocated resources. 
There are two main ways to deal with these problems in different programming languages:
1. _Finally_ blocks 
2. RAII

The goal of **CERAII** is in some sense to combine these two methods: releasing code should be near code that allocates resources and it should be explicit for anybody who reads the code how resources are released. 
Example of code with CERAII:

```C
#include <malloc.h>
#include "ceraii.h"

int func()
{
    int *pi = (int*)malloc(sizeof(int));
    DO_AT_EXIT(free(pi));  /* Explicit declaration of what should be done before exit from the function */
    
    if (pi == NULL) 
        RETURN(1);

    /* some code with multiple returns */
    
    if (some_condition) {
        /* --- */
        RETURN(2);
    } else if (another_condition) {
        /* --- */
        RETURN(3);
    }

    RETURN(0);
}
```
No matter how many returns you have in your code, statements in  **DO_AT_EXIT** macro will be automatically done before returning from the function. All you need is 2 macros: 
- **DO_AT_EXIT** to declare actions before return from the function
- **RETURN** macros instead of **return** key word 

For the most frequent cases of resource releasing it is preferred to define your own macros based on DO_AT_EXIT. For the case above:
```C
#include <malloc.h>
#include "ceraii.h"

#define FREE_AT_EXIT(pointer) \
    DO_AT_EXIT(free(pointer);)
    
int func()
{
    int *pi = (int*)malloc(sizeof(int));
    FREE_AT_EXIT(pi);  
    
    if (pi == NULL) 
        RETURN(1);

    /* some code with multiple returns */
    
    if (some_condition) {
        /* --- */
        RETURN(2);
    } else if (another_condition) {
        /* --- */
        RETURN(3);
    }

    RETURN(0);
}
```
After that it will be much easier to maintain code. For example if it is needed to log all envocations of free at the end of the functions, all you need is add _printf_ to the _FREE_AT_EXIT_ definition:
```C
#define FREE_AT_EXIT(pointer) \
    DO_AT_EXIT(\
        printf("Pointer %p is being freed\n", pointer);
        free(pointer);\
    )\
```
Some other common macros that can be defined based **DO_AT_EXIT** macro:

```C
/* Automatically free pointers allocated with malloc, calloc, realloc at exit*/
#define FREE_AT_EXIT(pointer) \
    DO_AT_EXIT(free(pointer);)
    
/* Automatically unlock pthread mutex at exit*/
#define UNLOCK_AT_EXIT(mutex) \
    DO_AT_EXIT(pthread_mutex_unlock(mutex);)
    
/* Automatically close file at exit*/
#define CLOSE_AT_EXIT(file_p) \
    DO_AT_EXIT(fclose(file_p);)

```
