#include <stdio.h>
#include "ceraii.h"

int factorial(int value)
{
    printf("factorial(%d) begin\n", value);
    DO_AT_EXIT(printf("factorial(%d) end\n", value););

    if (value == 0 || value == 1)
        RETURN(1);

    RETURN(value * factorial(value - 1));
}

int conditional(int value)
{
    if (value) {
        DO_AT_EXIT(printf("Condition is true\n"););
    } else {
        DO_AT_EXIT(printf("Condition is false\n"););
    }

    RETURN(1);
}

int switch_case(int value)
{
    switch (value) {
        case 1: DO_AT_EXIT(printf("Value = 1\n");); break;
        case 2: DO_AT_EXIT(printf("Value = 2\n");); break;
        default: DO_AT_EXIT(printf("Default value\n");); break;
    }

    RETURN(1);
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    factorial(3);
    printf("--------------------\n");
    conditional(1);
    conditional(0);
    printf("--------------------\n");
    switch_case(1);
    switch_case(2);
    switch_case(3);

    RETURN(0);
}


