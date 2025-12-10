#include "make_runtime_err.h"

#include "runtime_err.h"

RuntimeError make_runtime_error(RuntimeErrorType type, const char* message)
{
	return (RuntimeError){
		.type = type,
		.message = message,
	};
}
