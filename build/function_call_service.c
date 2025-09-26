#include <php.h>
#include <Zend/zend_API.h>
#include <Zend/zend_hash.h>
#include <Zend/zend_types.h>
#include <stddef.h>

#include "function_call_service.h"
#include "function_call_service_arginfo.h"
#include "_cgo_export.h"

#define VALIDATE_GO_HANDLE(intern) \
    do { \
        if ((intern)->go_handle == 0) { \
            zend_throw_error(NULL, "Go object not found in registry"); \
            RETURN_THROWS(); \
        } \
    } while (0)

static zend_object_handlers object_handlers_function_call_service;

typedef struct {
    uintptr_t go_handle;
    zend_object std; /* This must be the last field in the structure: the property store starts at this offset */
} function_call_service_object;

static inline function_call_service_object *function_call_service_object_from_obj(zend_object *obj) {
    return (function_call_service_object*)((char*)(obj) - offsetof(function_call_service_object, std));
}

static zend_object *function_call_service_create_object(zend_class_entry *ce) {
    function_call_service_object *intern = ecalloc(1, sizeof(function_call_service_object) + zend_object_properties_size(ce));
    
    zend_object_std_init(&intern->std, ce);
    object_properties_init(&intern->std, ce);
    
    intern->std.handlers = &object_handlers_function_call_service;
    intern->go_handle = 0; /* will be set in __construct */

    return &intern->std;
}

static void function_call_service_free_object(zend_object *object) {
    function_call_service_object *intern = function_call_service_object_from_obj(object);

    if (intern->go_handle != 0) {
        removeGoObject(intern->go_handle);
    }
    
    zend_object_std_dtor(&intern->std);
}

void init_object_handlers() {
    memcpy(&object_handlers_function_call_service, &std_object_handlers, sizeof(zend_object_handlers));
    object_handlers_function_call_service.free_obj = function_call_service_free_object;
    object_handlers_function_call_service.clone_obj = NULL;
    object_handlers_function_call_service.offset = offsetof(function_call_service_object, std);
}

static zend_class_entry *FunctionCallService_ce = NULL;

PHP_METHOD(FunctionCallService, __construct) {
    ZEND_PARSE_PARAMETERS_NONE();

    function_call_service_object *intern = function_call_service_object_from_obj(Z_OBJ_P(ZEND_THIS));

    /* Constructor is called more than once, make it no-op */
    if (intern->go_handle != 0) {
        return;
    }

    intern->go_handle = create_FunctionCallService_object();
}


PHP_METHOD(FunctionCallService, getAllFunctions) {
    function_call_service_object *intern = function_call_service_object_from_obj(Z_OBJ_P(ZEND_THIS));
    
    VALIDATE_GO_HANDLE(intern);
    ZEND_PARSE_PARAMETERS_NONE();
    
    void* result = getAllFunctions_wrapper(intern->go_handle);
    if (result != NULL) {
        HashTable *ht = (HashTable*)result;
        RETURN_ARR(ht);
    } else {
        RETURN_NULL();
    }
}

PHP_METHOD(FunctionCallService, RandomString) {
    function_call_service_object *intern = function_call_service_object_from_obj(Z_OBJ_P(ZEND_THIS));
    
    VALIDATE_GO_HANDLE(intern);
    zend_long length = 0;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(length)
        ZEND_PARSE_PARAMETERS_END();
    
    zend_string* result = RandomString_wrapper(intern->go_handle, length);
    RETURN_STR(result);
}

PHP_METHOD(FunctionCallService, core) {
    function_call_service_object *intern = function_call_service_object_from_obj(Z_OBJ_P(ZEND_THIS));
    
    VALIDATE_GO_HANDLE(intern);
    zend_string *rname = NULL;
    zend_string *param1 = NULL;
    zend_bool param1_is_null = 0;
    zend_string *param2 = NULL;
    zend_bool param2_is_null = 0;
    zval *paramis = NULL;
    
    ZEND_PARSE_PARAMETERS_START(4, 4)
        Z_PARAM_STR(rname)
        Z_PARAM_STR_OR_NULL(param1, param1_is_null)
        Z_PARAM_STR_OR_NULL(param2, param2_is_null)
        Z_PARAM_ARRAY_OR_NULL(paramis)
        ZEND_PARSE_PARAMETERS_END();
    
    zend_string* result = core_wrapper(intern->go_handle, rname, param1_is_null ? NULL : param1, param2_is_null ? NULL : param2, paramis);
    RETURN_STR(result);
}

void register_all_classes() {
    init_object_handlers();
    FunctionCallService_ce = register_class_FunctionCallService();
    if (!FunctionCallService_ce) {
        php_error_docref(NULL, E_ERROR, "Failed to register class FunctionCallService");
        return;
    }
    FunctionCallService_ce->create_object = function_call_service_create_object;
}

PHP_MINIT_FUNCTION(function_call_service) {
    register_all_classes();
    return SUCCESS;
}

zend_module_entry function_call_service_module_entry = {STANDARD_MODULE_HEADER,
                                         "function_call_service",
                                         ext_functions,             /* Functions */
                                         PHP_MINIT(function_call_service),  /* MINIT */
                                         NULL,                      /* MSHUTDOWN */
                                         NULL,                      /* RINIT */
                                         NULL,                      /* RSHUTDOWN */
                                         NULL,                      /* MINFO */
                                         "1.0.0",                   /* Version */
                                         STANDARD_MODULE_PROPERTIES};

