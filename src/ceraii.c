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
#include "ceraii.h"
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif


#if defined(CERAII_GCC_COMPILER)
#define ERAII_THREAD_LOCAL __thread       // gcc
#elif defined(CERAII_CLANG_COMPILER)
#define ERAII_THREAD_LOCAL _Thread_local  // clang
#elif defined(CERAII_MICROSOFT_COMPILER)
#define ERAII_THREAD_LOCAL __declspec( thread ) //msvc
#else
#define ERAII_THREAD_LOCAL _Thread_local  // default
#endif


/* Possible values of AsockReturnCode */
#define ASOCK_SUCCESS        0
#define ASOCK_FAILURE        666

#define SYS_LOG_ERROR(...)

struct asock_vector;
typedef struct asock_vector asock_vector_t;


static asock_vector_t* asock_vector_create(size_t item_size, size_t capacity);
static void asock_vector_destroy(asock_vector_t*);

static size_t asock_vector_size (const asock_vector_t*);

static int    asock_vector_push      (asock_vector_t*, const void *item);
static void* asock_vector_at (asock_vector_t*, size_t index);



#define MAX(a,b) ((a) > (b) ? (a) : b)

struct asock_vector {
    size_t m_size;
    void  *m_data;
    size_t m_capacity;
    size_t m_item_size;
};


static int reallocate(asock_vector_t *vector, size_t new_capacity)
{
    assert(vector);
    assert(new_capacity > vector->m_capacity);

    size_t new_size = new_capacity * vector->m_item_size;
    vector->m_data = realloc(vector->m_data, new_size);
    if (vector->m_data == NULL)
        return -1;
    return 0;
}

/* ------------ Constructors & Destructors ----------------------------- */

static asock_vector_t* asock_vector_create(size_t item_size, size_t capacity)
{
    asock_vector_t *vector = malloc(sizeof(asock_vector_t));
    if (vector == NULL) {
        SYS_LOG_ERROR("Failed to allocate memory for asock vector");
        return NULL;
    }

    size_t init_size = MAX(item_size * capacity, 1);
    vector->m_data = malloc(init_size);
    if (vector->m_data == NULL) {
        SYS_LOG_ERROR("Failed to allocate memory for asock vector inern. buffer");
        free(vector);
        return NULL;
    }

    vector->m_size      = 0;
    vector->m_capacity  = capacity;
    vector->m_item_size = item_size;

    return vector;
}


static void asock_vector_destroy(asock_vector_t* vector)
{
    assert(vector);
    free(vector->m_data);
    free(vector);
}




/* ----------- Nonmodifying functions --------------------------------- */

size_t asock_vector_size(const asock_vector_t* vector)
{
    assert(vector);
    return vector->m_size;
}



/* ----------- Modifying functions ------------------------------------- */

static int asock_vector_push (asock_vector_t* vector, const void* item)
{
    assert(vector);
    assert(item);

    if (vector->m_size == vector->m_capacity) {
        if (reallocate(vector, vector->m_capacity * 2) == -1)
            return ASOCK_FAILURE;
        vector->m_capacity = vector->m_capacity * 2;
    }

    ptrdiff_t deviation = vector->m_size * vector->m_item_size;
    memcpy(vector->m_data + deviation, item, vector->m_item_size);

    ++(vector->m_size);

    return ASOCK_SUCCESS;
}


static void *asock_vector_at(asock_vector_t *vector, size_t index)
{
    if (index >= vector->m_size)
        return NULL;

    return vector->m_data + index * vector->m_item_size;
}

/* ----------------------------------------------------------------------- */

static ERAII_THREAD_LOCAL  struct stack_return_item stack_items[CERAII_ENV_STACK_SIZE];
static ERAII_THREAD_LOCAL  struct stack_return_item return_caller;
static ERAII_THREAD_LOCAL  int raii_objects_counter = 0;

static void stack_items_vector_destructor(int status, void *stack_items_vector)
{
    (void)status;
    if (stack_items_vector != NULL) {
        asock_vector_destroy(stack_items_vector);
    }
}

static ERAII_THREAD_LOCAL  asock_vector_t * stack_items_vector= NULL;

struct stack_return_item* get_stack_item(int index)
{
    if (index < CERAII_ENV_STACK_SIZE)
        return &stack_items[index];

    if (stack_items_vector == NULL) {
        stack_items_vector = asock_vector_create(sizeof(struct stack_return_item), CERAII_ALLOC_ENV_STACK_INITIAL_CAPACITY);
        if (stack_items_vector == NULL) {
            abort();
        }
        on_exit(stack_items_vector_destructor, stack_items_vector);
    }

    while (index >= CERAII_ENV_STACK_SIZE + asock_vector_size(stack_items_vector)) {
        struct stack_return_item dummy;
        memset(&dummy, 0, sizeof(dummy));
        int status = asock_vector_push(stack_items_vector, &dummy);
        if (status == ASOCK_FAILURE) {
            abort();
        }
    }

    return (struct stack_return_item*)asock_vector_at(stack_items_vector, index - CERAII_ENV_STACK_SIZE);
}

struct stack_return_item* get_return_caller(void)
{
    return &return_caller;
}

int* get_raii_objects_counter(void)
{
    return &raii_objects_counter;
}





#ifdef __cplusplus
}
#endif
