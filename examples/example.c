/*
CERAII

MIT License

Copyright (c) 2017 Seleznev Anton

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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

int not_like_go()
{
    volatile int i = 0;
    DO_AT_EXIT(printf("%d\n", i););
    i++;
    return (0);
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;


    printf("===== Example of recursive function with CERAII =====\n");
    factorial(3);

    printf("\n===== Example of CERAII macros in conditional statements =====\n");
    conditional(1);

    printf("\n===== Example of CERAII macros in switch statements =====\n");
    switch_case(3);
    switch_case(1);

    not_like_go();

    RETURN(0);
}


