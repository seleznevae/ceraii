#ifndef ERAII_H
#define ERAII_H

#include <setjmp.h>
#include <string.h>


#ifdef __cplusplus
extern "C"
{
#endif


/**
 * Configuration parameters
 */

#ifdef  ERAII_CONF_ENV_STACK_SIZE
#define ERAII_ENV_STACK_SIZE ERAII_CONF_ENV_STACK_SIZE
#else
#define ERAII_ENV_STACK_SIZE 200
#endif


/**
 * Definitions of main structures
 */

struct signature
{
    void *return_address_;
    void *frame_address_;
};

struct stack_return_item
{
    struct signature signature_;
    int index_;
    jmp_buf env_;
};

struct stack_return_item* get_stack_item(int index);
struct stack_return_item* get_return_caller();
int* get_raii_objects_counter();


/**
 * Utility macros
 */

#define STR_2_CAT_(arg1, arg2) \
    arg1##arg2
#define STR_2_CAT(arg1, arg2) \
    STR_2_CAT_(arg1, arg2)

#define UNIQUE_RAII_NAME_(prefix) \
    STR_2_CAT(prefix,__COUNTER__)
#define UNIQUE_RAII_NAME(prefix) \
    UNIQUE_RAII_NAME_(prefix)

/**
 *  Implementation specific macros
 */

#define CERAII_TYPE_OF(arg) __typeof__(arg)

/**
 * Main helper macros
 */

#define ARE_EQUAL_SIGNATURES_(sig1,sig2) \
    (((sig1).return_address_ == (sig2).return_address_) \
    && ((sig1).frame_address_ == (sig2).frame_address_))

#define ARE_EQUAL_SIGNATURES(sig1,sig2) \
    ARE_EQUAL_SIGNATURES_(sig1, sig2)

#define FILL_SIGNATURE_(signature) \
    signature.return_address_ = __builtin_return_address(0); \
    signature.frame_address_ = __builtin_frame_address(0)

#define FILL_SIGNATURE(signature) \
    FILL_SIGNATURE_(signature)


#define GO_TO_PREVIOS_LABEL_OR_RETURN_TO_CALLER(item) \
    do { \
        if (item.index_ == 0 || !ARE_EQUAL_SIGNATURES(item.signature_, get_stack_item(item.index_ -1)->signature_)) { \
            longjmp(get_return_caller()->env_, 1); \
        }\
        else { \
            longjmp(get_stack_item(item.index_ -1)->env_, 1); \
        }\
    } while(0)


/**
 * Main macros
 */

#define DO_AT_EXIT_(actions, index_name) \
do {\
    int index_name = (*get_raii_objects_counter())++; \
    FILL_SIGNATURE((*get_stack_item(index_name)).signature_); \
    (*get_stack_item(index_name)).index_ = index_name; \
    if (setjmp(get_stack_item(index_name)->env_)) {\
        actions \
        (*get_raii_objects_counter())--; \
        GO_TO_PREVIOS_LABEL_OR_RETURN_TO_CALLER((*get_stack_item(*get_raii_objects_counter())));\
    }\
} while (0)

#define DO_AT_EXIT(actions) \
    DO_AT_EXIT_(actions, UNIQUE_RAII_NAME(index_eraii))


#define DO_AT_SCOPE_EXIT_(actions, tmp_var_name, destructor_name) \
    void destructor_name(void *arg) { \
        (void)arg; \
        actions \
    } \
    int tmp_var_name __attribute__ ((__cleanup__(destructor_name))) = 1;

#define DO_AT_SCOPE_EXIT(actions) \
    DO_AT_SCOPE_EXIT_(actions, UNIQUE_RAII_NAME(tmp_var),  UNIQUE_RAII_NAME(destructor_func))



#define RETURN_(return_value, index_name, return_name) \
    do { \
        CERAII_TYPE_OF(return_value) return_name = (return_value); \
        int index_name = (*get_raii_objects_counter())++; \
        FILL_SIGNATURE((*get_stack_item(index_name)).signature_); \
        (*get_stack_item(index_name)).index_ = index_name; \
        if (setjmp(get_stack_item(index_name)->env_)) {\
            return return_name; \
        } \
        memcpy(get_return_caller(), get_stack_item(index_name), sizeof(struct stack_return_item)); \
        (*get_raii_objects_counter())--; \
        GO_TO_PREVIOS_LABEL_OR_RETURN_TO_CALLER((*get_stack_item(*get_raii_objects_counter())));\
    } while(0)

#define RETURN(return_value) \
    RETURN_(return_value, UNIQUE_RAII_NAME(index_eraii), UNIQUE_RAII_NAME(return_eraii))





#ifdef __cplusplus
}
#endif


#endif // ERAII_H
