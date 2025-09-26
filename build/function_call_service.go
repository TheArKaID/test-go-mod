package function_call_service

/*
#include <stdlib.h>
#include "function_call_service.h"
*/
import "C"
import "crypto/rand"
import "encoding/hex"
import "errors"
import "strings"
import "unsafe"
import "github.com/dunglas/frankenphp"

func init() {
	frankenphp.RegisterExtension(unsafe.Pointer(&C.function_call_service_module_entry))
}


var args []any
var out string
var err error
var l int64 = 7
var v int64


func goStr(s *C.zend_string) string {
	if s == nil {
		return ""
	}
	return frankenphp.GoString(unsafe.Pointer(s))
}
func (s *FunctionCallService) strToHex(param string) string {
	dst := make([]byte, hex.EncodedLen(len(param)))
	hex.Encode(dst, []byte(param))
	return strings.ToUpper(string(dst))
}
func (s *FunctionCallService) paddWhiteSpaces(param string) (string, error) {
	parts := strings.SplitN(param, ";", 2)
	if len(parts) != 2 {
		return "", errors.New("invalid input, expected 'data;len'")
	}
	data := parts[0]

	// simple atoi (no strconv to keep the example self-contained)
	raw := parts[1]
	target := 0
	for i := 0; i < len(raw); i++ {
		if raw[i] < '0' || raw[i] > '9' {
			return "", errors.New("length must be an integer")
		}
		target = target*10 + int(raw[i]-'0')
	}
	if target < 0 {
		target = 0
	}

	// pad with spaces
	for len(data) < target {
		data += " "
	}
	return data, nil
}
type FunctionCallService struct {
}

//export registerGoObject
func registerGoObject(obj interface{}) C.uintptr_t {
	handle := cgo.NewHandle(obj)
	return C.uintptr_t(handle)
}

//export getGoObject
func getGoObject(handle C.uintptr_t) interface{} {
	h := cgo.Handle(handle)
	return h.Value()
}

//export removeGoObject
func removeGoObject(handle C.uintptr_t) {
	h := cgo.Handle(handle)
	h.Delete()
}
//export create_FunctionCallService_object
func create_FunctionCallService_object() C.uintptr_t {
	obj := &FunctionCallService{}
	return registerGoObject(obj)
}
func (s *FunctionCallService) GetAllFunctions() unsafe.Pointer {
	// List what we actually expose here (you can add more as you port additional PHP methods)
	methods := []any{
		"getAllFunctions",
		"RandomString",
		"core",
		"strtohex",
		"paddWhiteSpaces",
	}
	return frankenphp.PHPPackedArray(methods)
}

func (s *FunctionCallService) RandomString(length int64) unsafe.Pointer {
	// Default behavior like PHP signature default = 7
	n := int(length)
	if n <= 0 {
		n = 7
	}
	const alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
	// Secure-ish random generation
	b := make([]byte, n)
	_, _ = rand.Read(b)
	for i := 0; i < n; i++ {
		b[i] = alphabet[int(b[i])%len(alphabet)]
	}
	return frankenphp.PHPString(string(b), false)
}

func (s *FunctionCallService) Core(rname *C.zend_string, p1 *C.zend_string, p2 *C.zend_string, paramis *C.zval) unsafe.Pointer {
	name := strings.ToLower(goStr(rname))
	arg1 := goStr(p1)
	arg2 := goStr(p2)

	// Convert optional array params (packed) if provided; many of your call paths don't need this
	var args []any
	if paramis != nil {
		args = frankenphp.GoPackedArray(unsafe.Pointer(paramis), false)
	}

	// Dispatch to internal methods we expose in this port
	var out string
	var err error

	switch name {
	case "strtohex":
		out = s.strToHex(arg1)
	case "paddwhitespaces":
		out, err = s.paddWhiteSpaces(arg1)
	case "randomstring":
		// allow param1 as length if provided
		if arg1 != "" {
			// best-effort parse
			var l int64 = 7
			for i := 0; i < len(arg1); i++ {
				if arg1[i] < '0' || arg1[i] > '9' {
					l = 7
					break
				}
			}
			// simple atoi (no strconv to keep this tight)
			if arg1 != "" {
				var v int64
				for i := 0; i < len(arg1); i++ {
					v = v*10 + int64(arg1[i]-'0')
				}
				if v > 0 {
					l = v
				}
			}
			return s.RandomString(l)
		}
		return s.RandomString(7)
	default:
		err = errors.New("unknown function: " + name)
	}

	if err != nil {
		return frankenphp.PHPString("ERROR: "+err.Error(), false)
	}
	return frankenphp.PHPString(out, false)
}

//export getAllFunctions_wrapper
func getAllFunctions_wrapper(handle C.uintptr_t) unsafe.Pointer {
	obj := getGoObject(handle)
	if obj == nil {
		return nil
	}
	structObj := obj.(*FunctionCallService)
	return structObj.GetAllFunctions()
}

//export RandomString_wrapper
func RandomString_wrapper(handle C.uintptr_t, length int64) unsafe.Pointer {
	obj := getGoObject(handle)
	if obj == nil {
		return nil
	}
	structObj := obj.(*FunctionCallService)
	return structObj.RandomString(length)
}

//export core_wrapper
func core_wrapper(handle C.uintptr_t, rname *C.zend_string, param1 *C.zend_string, param2 *C.zend_string, paramis *C.zval) unsafe.Pointer {
	obj := getGoObject(handle)
	if obj == nil {
		return nil
	}
	structObj := obj.(*FunctionCallService)
	return structObj.Core(rname, param1, param2, paramis)
}

