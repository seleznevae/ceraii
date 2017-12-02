#include <stdio.h>
#include <string.h>
#include "ceraii.h"
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

#define NUMBER_OF_ITERATIONS  1000000

int marker1[3] = {0};
int marker2[3] = {0};
int marker3[3] = {0};


int base_multiple_return_100(int thread_num)
{
    assert(marker1[thread_num] == 0);
    assert(marker2[thread_num] == 0);
    assert(marker3[thread_num] == 0);

    int sum = 0;

    DO_AT_EXIT(marker1[thread_num] = 1;);
    // Imitating some work
    int i = 0;
    for (i = 0; i < 100; ++i)
        sum += i;

    DO_AT_EXIT(marker2[thread_num] = 2;);
    // Imitating some work
    for (i = 0; i < 100; ++i)
        sum += i;

    DO_AT_EXIT(marker3[thread_num] = 3;);
    // Imitating some work
    for (i = 0; i < 100; ++i)
        sum += i;

    RETURN(i);
}

void* thread_1_func(void *arg)
{
    (void)arg;
    int thread_nuber = 0;
    size_t i = 0;
    for (i = 0; i < NUMBER_OF_ITERATIONS; ++i) {
        marker1[thread_nuber] = 0;
        marker2[thread_nuber] = 0;
        marker3[thread_nuber] = 0;
        int result = base_multiple_return_100(thread_nuber);
        assert(result == 100);
        assert(marker1[thread_nuber] = 1);
        assert(marker3[thread_nuber] = 2);
        assert(marker2[thread_nuber] = 3);
    }
    return NULL;
}

void* thread_2_func(void *arg)
{
    (void)arg;
    int thread_nuber = 1;
    size_t i = 0;
    for (i = 0; i < NUMBER_OF_ITERATIONS; ++i) {
        marker1[thread_nuber] = 0;
        marker2[thread_nuber] = 0;
        marker3[thread_nuber] = 0;
        int result = base_multiple_return_100(thread_nuber);
        assert(result == 100);
        assert(marker1[thread_nuber] = 1);
        assert(marker3[thread_nuber] = 2);
        assert(marker2[thread_nuber] = 3);
    }
    return NULL;
}

void* thread_3_func(void *arg)
{
    (void)arg;
    int thread_nuber = 2;
    size_t i = 0;
    for (i = 0; i < NUMBER_OF_ITERATIONS; ++i) {
        marker1[thread_nuber] = 0;
        marker2[thread_nuber] = 0;
        marker3[thread_nuber] = 0;
        int result = base_multiple_return_100(thread_nuber);
        assert(result == 100);
        assert(marker1[thread_nuber] = 1);
        assert(marker3[thread_nuber] = 2);
        assert(marker2[thread_nuber] = 3);
    }
    return NULL;
}


void* thread_func(void *arg)
{
    int thread_number = *(int*)arg;
    size_t i = 0;
    for (i = 0; i < NUMBER_OF_ITERATIONS; ++i) {
        marker1[thread_number] = 0;
        marker2[thread_number] = 0;
        marker3[thread_number] = 0;
        int result = base_multiple_return_100(thread_number);
        assert(result == 100);
        assert(marker1[thread_number] = 1);
        assert(marker3[thread_number] = 2);
        assert(marker2[thread_number] = 3);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    /*
     * Test threads that run independent functions with CERAII macros
     */
    pthread_t thread_1;
    pthread_t thread_2;
    pthread_t thread_3;

    int status = pthread_create(&thread_1, NULL, &thread_1_func, NULL);
    if (status != 0) {
        fprintf(stderr, "pthread_create failed\n");
        return 1;
    }

    status = pthread_create(&thread_2, NULL, &thread_2_func, NULL);
    if (status != 0) {
        fprintf(stderr, "pthread_create failed\n");
        pthread_join(thread_1, NULL);
        return 1;
    }

    status = pthread_create(&thread_3, NULL, &thread_3_func, NULL);
    if (status != 0) {
        fprintf(stderr, "pthread_create failed\n");
        pthread_join(thread_2, NULL);
        pthread_join(thread_1, NULL);
        return 1;
    }

    pthread_join(thread_3, NULL);
    pthread_join(thread_2, NULL);
    pthread_join(thread_1, NULL);


    /*
     * Test threads that run the same functions with CERAII macros
     */
    int thread1_number = 0;
    status = pthread_create(&thread_1, NULL, &thread_func, &thread1_number);
    if (status != 0) {
        fprintf(stderr, "pthread_create failed\n");
        return 1;
    }

    int thread2_number = 1;
    status = pthread_create(&thread_2, NULL, &thread_func, &thread2_number);
    if (status != 0) {
        fprintf(stderr, "pthread_create failed\n");
        pthread_join(thread_1, NULL);
        return 1;
    }

    int thread3_number = 2;
    status = pthread_create(&thread_3, NULL, &thread_func, &thread3_number);
    if (status != 0) {
        fprintf(stderr, "pthread_create failed\n");
        pthread_join(thread_2, NULL);
        pthread_join(thread_1, NULL);
        return 1;
    }

    pthread_join(thread_3, NULL);
    pthread_join(thread_2, NULL);
    pthread_join(thread_1, NULL);

    return 0;
}
