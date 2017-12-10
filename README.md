# ceraii (C Explicit RAII)
[![Build Status](https://travis-ci.org/seleznevae/ceraii.svg?branch=master)](https://travis-ci.org/seleznevae/ceraii)
[![Build status](https://ci.appveyor.com/api/projects/status/9lm1crskqmui1uvw?svg=true)](https://ci.appveyor.com/project/seleznevae/ceraii)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Coverage Status](https://coveralls.io/repos/github/seleznevae/ceraii/badge.svg?branch=master)](https://coveralls.io/github/seleznevae/ceraii?branch=master)

In software projects written in C there's always a lot of boiler plate code for releasing allocated resources. 
There are two main ways to deal with these problems in different programming languages:
1. _Finally_ blocks 
2. RAII
3. Deferred functions

The goal of **CERAII** is primarily to implement deferred functions as they seems to be closer to C style programming. With deferred functions it should be obvious for anybody who reads the code how resources are released. 

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

# Differences with **"defer"** in golang

In **go** a deferred function's arguments are evaluated when the defer statement is evaluated. Therefore in **go** the deferred call will print "0" after the function returns:
```go
func a() {
    i := 0
    defer fmt.Println(i)
    i++
    return
}
```
Expressions in **DO_AT_EXIT** are evaluated at the moment of **RETURN**. Therefore this function will print "1" after the function returns:
```C
void not_like_go()
{
    volatile int i = 0;
    DO_AT_EXIT(  printf("%d\n", i);   );
    
    i++;
    RETURN();
}
```

# Macros usage

**DO_AT_EXIT** and **RETURN** can be used freely in conditional clauses, switch statements, recursive functions:

```C
#include <stdio.h>
#include "ceraii.h"

int factorial(int value)
{
    printf(" Factorial(%d) begin\n", value);
    DO_AT_EXIT(printf(" Factorial(%d) end\n", value););

    printf("   Factorial body\n");
    if (value == 0 || value == 1)
        RETURN(1);

    RETURN(value * factorial(value - 1));
}

int conditional(int value)
{
    if (value) {
        DO_AT_EXIT(printf(" Condition was true\n"););
    } else {
        DO_AT_EXIT(printf(" Condition was false\n"););
    }

    printf("   Conditional function body\n");
    RETURN(1);
}

int switch_case(int value)
{
    switch (value) {
        case 1: DO_AT_EXIT(printf(" Value = 1\n");); break;
        case 2: DO_AT_EXIT(printf(" Value = 2\n");); break;
        default: DO_AT_EXIT(printf(" Value is default\n");); break;
    }

    printf("   Switch function body\n");
    RETURN(1);
}

int main()
{
    printf("===== Example of recursive function with CERAII =====\n");
    factorial(3);

    printf("\n===== Example of CERAII macros in conditional statements =====\n");
    conditional(1);

    printf("\n===== Example of CERAII macros in switch statements =====\n");
    switch_case(3);
    switch_case(1);

    RETURN(0);
}
```
Output is:
```
===== Example of recursive function with CERAII =====
 Factorial(3) begin
   Factorial body
 Factorial(2) begin
   Factorial body
 Factorial(1) begin
   Factorial body
 Factorial(1) end
 Factorial(2) end
 Factorial(3) end

===== Example of CERAII macros in conditional statements =====
   Conditional function body
 Condition was true

===== Example of CERAII macros in switch statements =====
   Switch function body
 Value is default
   Switch function body
 Value = 1
```
# Installation

To use **DO_AT_SCOPE_EXIT** and **RETURN** macro include _ceraii.h_ in your source code. Also it necessary to compile _ceraii.c_ and link with other object files during linkage process.

# Pitfalls
**CERAII** implementation is based on C long jumps. Therefore you should keep in mind:
1. Variables used inside **DO_AT_SCOPE_EXIT** macro shouldn't be changed from this macro usage till **RETURN** macro. If they are changed they should be declared volatile, otherwise their values will be indeterminate at the moment of execution before returning from the function. Also if when **DO_AT_SCOPE_EXIT** was called, a VLA (Variable-length array) or another variably-modified type variable was in scope and control left that scope, then it will be undefined behavior. See http://en.cppreference.com/w/c/program/setjmp for details. Examples of correct usage:
```C
/* No need to declare pi volatile. Because it is not modified after DO_AT_EXIT */
int *pi = (int*)malloc(sizeof(int));
DO_AT_EXIT(free(pi));

/* Some code that doesn't modify pi. */
*pi = 3;
*pi += *pi;
...

/* ps should be defined as volatile because lately it would be changed */
volatile short *ps = (short*)malloc(sizeof(short)); 
DO_AT_EXIT(free(ps));

free(ps);
*ps = (short*)malloc(sizeof(short));

RETURN();
```

# Defining custom macros

**DO_AT_EXIT** is so verbose on purpose so that it clear what it does. However if your prefer more fashionable names (like "defer") you can wrap **CERAII** and redefined them. Moreover for the most frequent cases of resource releasing it is preferred to define your own macros based on DO_AT_EXIT. 
Here are some examples:
```C
#include <malloc.h>
#include "ceraii.h"

/* Automatically free pointers allocated with malloc, calloc, realloc at exit*/
#define FREE_AT_EXIT(pointer) \
    DO_AT_EXIT(free(pointer);)
    
/* Automatically unlock pthread mutex at exit*/
#define UNLOCK_AT_EXIT(mutex) \
    DO_AT_EXIT(pthread_mutex_unlock(mutex);)
    
/* Automatically close file at exit*/
#define CLOSE_AT_EXIT(file_p) \
    DO_AT_EXIT(fclose(file_p);)
    
int func()
{
    int *pi = (int*)malloc(sizeof(int));
    FREE_AT_EXIT(pi);  
    
    if (pi == NULL) 
        RETURN(1);
   
    if (some_condition) {
        /* --- */
        RETURN(2);
    }

    RETURN(0);
}
```
After that it will be much easier to maintain code. For example if it is needed to log all invocations of free at the end of the functions, all you need is to add _printf_ to the _FREE_AT_EXIT_ definition:
```C
#define FREE_AT_EXIT(pointer) \
    DO_AT_EXIT(\
        printf("Pointer %p is being freed\n", pointer);
        free(pointer);\
    )\
```

