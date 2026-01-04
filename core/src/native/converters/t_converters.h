#pragma once

typedef struct Env Env;
typedef struct ValueBuffer ValueBuffer;
typedef struct Context Context;
typedef struct Value Value;

void define_native_converters(Env* env);
