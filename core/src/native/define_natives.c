#include "time.h"

typedef struct Env Env;

void define_natives(Env* env) { define_native_time(env); }
