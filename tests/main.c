#include <stdio.h>
#include <string.h>
#include "ceraii.h"
#include <stdlib.h>
#include <assert.h>



int factorial(int value)
{
    DO_AT_EXIT(
        printf("Returning from factorial invoked with value %d\n", value);
    );
    if (value == 0 || value == 1) {
        RETURN(1);
    }

    int ret = value * factorial(value - 1);
    printf("Factorial body invoked with value %d\n", value);
    RETURN(ret);
}



//char *copy_string(const char *str)
//{
//    char *result = malloc(strlen(str) + 1);
//    DO_AT_EXIT(
//        free(result);
//    );

//    if (result == NULL) {
//        RETURN(NULL);
//    }

//    strcpy(result, str);

//    if (1) { /* if something wrong */
//        RETURN(NULL);
//    } else { /*if everything is ok */
//        char *tmp = result;
//        result = NULL;
//        RETURN(tmp);
//    }
//}



int empty_function()
{
   printf("Empty function body\n");
   RETURN(1);
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    int a = 3;
    DO_AT_EXIT(
        printf("Hello, world!\n");
    );
    if (0) {
        DO_AT_EXIT(
            printf("Hello, world2!\n");
        );
    } else {
        DO_AT_EXIT(
            printf("Hello, world3!\n");
        );
    }


//    factorial(3);
//    printf("--------------------\n");
//    empty_function();
//    printf("--------------------\n");
//    factorial(5);
//    printf("--------------------\n");

    printf("return address %p\n", __builtin_return_address(0));
    printf("frame address %p\n", __builtin_frame_address(0));
    printf("sizeof(jmp_buf) %d\n", (int)sizeof(jmp_buf));

    RETURN(a);
//    return a;
}


