#pragma once

typedef struct Env Env;
typedef struct ValueBuffer ValueBuffer;
typedef struct Context Context;
typedef struct Value Value;

Value native_toString(ValueBuffer* arguments, Context* context);

void define_native_toString(Env* env);
