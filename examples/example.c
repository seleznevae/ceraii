#include <stdio.h>
#include "ceraii.h"

//int factorial(int value)
//{
//    printf(" Factorial(%d) begin\n", value);
//    DO_AT_EXIT(printf(" Factorial(%d) end\n", value););

//    printf("   Factorial body\n");
//    if (value == 0 || value == 1)
//        RETURN(1);

//    RETURN(value * factorial(value - 1));
//}

//int conditional(int value)
//{
//    if (value) {
//        DO_AT_EXIT(printf(" Condition was true\n"););
//    } else {
//        DO_AT_EXIT(printf(" Condition was false\n"););
//    }

//    printf("   Conditional function body\n");
//    RETURN(1);
//}

//int switch_case(int value)
//{
//    switch (value) {
//        case 1: DO_AT_EXIT(printf(" Value = 1\n");); break;
//        case 2: DO_AT_EXIT(printf(" Value = 2\n");); break;
//        default: DO_AT_EXIT(printf(" Value is default\n");); break;
//    }

//    printf("   Switch function body\n");
//    RETURN(1);
//}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;


//    printf("===== Example of recursive function with CERAII =====\n");
//    factorial(3);

//    printf("\n===== Example of CERAII macros in conditional statements =====\n");
//    conditional(1);

//    printf("\n===== Example of CERAII macros in switch statements =====\n");
//    switch_case(3);
//    switch_case(1);

    RETURN(0);
}


