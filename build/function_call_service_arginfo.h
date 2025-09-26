/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 4ed7612714f70b1f4e959b4f8a61015b26b4cc14 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_FunctionCallService___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_FunctionCallService_getAllFunctions, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_FunctionCallService_RandomString, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, length, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_FunctionCallService_core, 0, 4, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, rname, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, param1, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, param2, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, paramis, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_METHOD(FunctionCallService, __construct);
ZEND_METHOD(FunctionCallService, getAllFunctions);
ZEND_METHOD(FunctionCallService, RandomString);
ZEND_METHOD(FunctionCallService, core);

static const zend_function_entry class_FunctionCallService_methods[] = {
	ZEND_ME(FunctionCallService, __construct, arginfo_class_FunctionCallService___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(FunctionCallService, getAllFunctions, arginfo_class_FunctionCallService_getAllFunctions, ZEND_ACC_PUBLIC)
	ZEND_ME(FunctionCallService, RandomString, arginfo_class_FunctionCallService_RandomString, ZEND_ACC_PUBLIC)
	ZEND_ME(FunctionCallService, core, arginfo_class_FunctionCallService_core, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_FunctionCallService(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "FunctionCallService", class_FunctionCallService_methods);
	class_entry = zend_register_internal_class(&ce);

	return class_entry;
}
