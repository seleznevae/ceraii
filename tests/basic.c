#include <stdio.h>
#include <string.h>
#include "ceraii.h"
#include <stdlib.h>
#include <assert.h>




int marker1 = 0;
int marker2 = 0;
int marker3 = 0;

int base_return_10()
{
    assert(marker1 == 0);
    DO_AT_EXIT(marker1 = 1;);

    /* Imitating some work */
    int i = 0;
    for (i = 0; i < 10; ++i)
        ;

    RETURN(i);
}

int base_multiple_actions_return_10()
{
    assert(marker1 == 0);
    assert(marker2 == 0);
    assert(marker3 == 0);
    DO_AT_EXIT(
        marker1 = 1;
        marker2 = 2;
        marker3 = 3;
    );

    /* Imitating some work */
    int i = 0;
    for (i = 0; i < 10; ++i)
        ;

    RETURN(i);
}

/*
 *  Checking that DO_AT_EXIT is executed after
 *  evaluation of statement in RETURN
 */
int base_modified_return_10()
{
    assert(marker1 == 0);

    int *i = (int*)malloc(sizeof(i));
    DO_AT_EXIT(
        marker1 = 1;
        free(i);
    );

    /* Imitating some work */
    for (*i = 0; *i < 100; ++(*i))
        ;

    RETURN(*i / 10);
}

int base_multiple_return_10()
{
    assert(marker1 == 0);
    assert(marker2 == 0);
    assert(marker3 == 0);

    DO_AT_EXIT(marker1 = 1;);
    /* Imitating some work */
    int i = 0;
    for (i = 0; i < 10; ++i)
        ;

    DO_AT_EXIT(marker2 = 2;);
    /* Imitating some work */
    for (i = 0; i < 10; ++i)
        ;

    DO_AT_EXIT(marker3 = 3;);
    /* Imitating some work */
    for (i = 0; i < 10; ++i)
        ;

    RETURN(i);
}

int base_without_macros_return_10()
{
    assert(marker1 == 0);

    /* Imitating some work */
    int i = 0;
    for (i = 0; i < 10; ++i)
        ;

    RETURN(i);
}


int base_if_return_10(int branch)
{
    assert(marker1 == 0);
    assert(marker2 == 0);
    assert(marker3 == 0);

    DO_AT_EXIT(marker1 = 1;);
    /* Imitating some work */
    int i = 0;
    for (i = 0; i < 10; ++i)
        ;

    if (branch) {
        DO_AT_EXIT(marker2 = 2;);
        /* Imitating some work */
        for (i = 0; i < 10; ++i)
            ;
    } else {
        DO_AT_EXIT(marker3 = 3;);
        /* Imitating some work */
        for (i = 0; i < 10; ++i)
            ;
    }

    RETURN(i);
}

int base_switch_return_10(int switch_value)
{
    assert(marker1 == 0);
    assert(marker2 == 0);
    assert(marker3 == 0);

    int i = 0;
    switch (switch_value) {
        case 1:
            DO_AT_EXIT(marker1 = 1;);
            /* Imitating some work */
            for (i = 0; i < 10; ++i)
                ;
            break;
        case 2:
            DO_AT_EXIT(marker2 = 2;);
            /* Imitating some work */
            for (i = 0; i < 10; ++i)
                ;
            break;

        case 3:
            DO_AT_EXIT(marker3 = 3;);
            /* Imitating some work */
            for (i = 0; i < 10; ++i)
                ;
            break;
    }

    RETURN(i);
}




int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    int result = 0;

    /*
     *  Base check
     */
    marker1 = 0;
    result = base_return_10();
    assert(result == 10);
    assert(marker1 == 1);

    /*
     *  Base check that DO_AT_EXIT is executed after evaluation of statement in RETURN
     */
#if (defined(CERAII_GCC_COMPILER) || defined(CERAII_CLANG_COMPILER))
    marker1 = 0;
    result = base_modified_return_10();
    assert(result == 10);
    assert(marker1 == 1);
#endif

    /*
     * Base check with multiple actions in one DO_AT_EXIT macro
     */
    marker1 = 0;
    marker2 = 0;
    marker3 = 0;
    result = base_multiple_actions_return_10();
    assert(result == 10);
    assert(marker1 == 1);
    assert(marker2 == 2);
    assert(marker3 == 3);


    /*
     * Base check with multiple macros
     */
    marker1 = 0;
    marker2 = 0;
    marker3 = 0;
    result = base_multiple_return_10();
    assert(result == 10);
    assert(marker1 == 1);
    assert(marker2 == 2);
    assert(marker3 == 3);


    /*
     * Base check with RETURN but without DO... macros
     */
    marker1 = 0;
    marker2 = 0;
    marker3 = 0;
    result = base_without_macros_return_10();
    assert(result == 10);
    assert(marker1 == 0);
    assert(marker2 == 0);
    assert(marker3 == 0);


    /*
     * Base check with conditionals
     */
    marker1 = 0;
    marker2 = 0;
    marker3 = 0;
    result = base_if_return_10(1);
    assert(result == 10);
    assert(marker1 == 1);
    assert(marker2 == 2);
    assert(marker3 == 0);

    marker1 = 0;
    marker2 = 0;
    marker3 = 0;
    result = base_if_return_10(0);
    assert(result == 10);
    assert(marker1 == 1);
    assert(marker2 == 0);
    assert(marker3 == 3);


    /*
     * Base check with switch
     */
    marker1 = 0;
    marker2 = 0;
    marker3 = 0;
    result = base_switch_return_10(0);
    assert(result == 0);
    assert(marker1 == 0);
    assert(marker2 == 0);
    assert(marker3 == 0);

    marker1 = 0;
    marker2 = 0;
    marker3 = 0;
    result = base_switch_return_10(1);
    assert(result == 10);
    assert(marker1 == 1);
    assert(marker2 == 0);
    assert(marker3 == 0);

    marker1 = 0;
    marker2 = 0;
    marker3 = 0;
    result = base_switch_return_10(2);
    assert(result == 10);
    assert(marker1 == 0);
    assert(marker2 == 2);
    assert(marker3 == 0);

    marker1 = 0;
    marker2 = 0;
    marker3 = 0;
    result = base_switch_return_10(3);
    assert(result == 10);
    assert(marker1 == 0);
    assert(marker2 == 0);
    assert(marker3 == 3);

    return 0;
}

