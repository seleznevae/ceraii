#include "eraii.h"


#define ERAII_THREAD_LOCAL _Thread_local

static ERAII_THREAD_LOCAL  struct stack_return_item stack_items[ERAII_ENV_STACK_SIZE];
static ERAII_THREAD_LOCAL  struct stack_return_item return_caller;
static ERAII_THREAD_LOCAL  int raii_objects_counter = 0;

struct stack_return_item* get_stack_item(int index)
{
    return &stack_items[index];
}

struct stack_return_item* get_return_caller()
{
    return &return_caller;
}

int* get_raii_objects_counter()
{
    return &raii_objects_counter;
}
