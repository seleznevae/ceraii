# ceraii (C Explicit RAII) (WIP - Work in progress)
[![Build Status](https://travis-ci.org/seleznevae/ceraii.svg?branch=master)](https://travis-ci.org/seleznevae/ceraii)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Coverage Status](https://coveralls.io/repos/github/seleznevae/ceraii/badge.svg?branch=master)](https://coveralls.io/github/seleznevae/ceraii?branch=master)

In software projects written in C there's always a lot of boiler plate code for releasing allocated resources. 
There are two main ways to deal with these problems in different programmin languages:
1. _Finally_ blocks 
2. RAII

The goal of **CERAII** is in some sense to combine these two methods: releasing code should be near code that allocates resources and it should be explicit for anybody who reads the code how resources are released. 
Example of code with CERAII:

```
#include <malloc.h>
#include "ceraii.h"

void func(const char *str)
{
    char *str_copy = (char*)malloc(strlen(str) + 1);
    DO_AT_EXIT(free(str_copy));
    
    if (str_copy == NULL) 
        RETURN();

    /* some code with multiple returns */
    
    if (some_condition) {
        /* --- */
        RETURN();
    } else if (another_condition) {
        /* --- */
        RETURN();
    }

    RETURN();
}
```
No matter how many returns you have in your code, statements in  DO_AT_EXIT macro will be automatically done before exiting the function.
