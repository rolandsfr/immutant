#include "print.h"
#include "time.h"

#include "converters/t_converters.h"

typedef struct Env Env;

void define_natives(Env* env)
{
	define_native_time(env);
	define_native_print(env);
	define_native_converters(env);
}
