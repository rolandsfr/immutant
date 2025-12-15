#pragma once

#include <stdlib.h>
#include <string.h>

#include "ast_stmt.h"

typedef struct Value Value; // your interpreter's value type

typedef struct EnvEntry {
	char* name;
	Value* value;
	struct EnvEntry* next; // for chaining
	enum MutabilityType mutability;
} EnvEntry;

typedef struct Env {
	struct Env* parent;
	EnvEntry* entries; // linked list or hash table
} Env;

Env* env_new(Env* parent);

void env_define(Env* env, const char* name, Value* value,
				enum MutabilityType mutability);

Value* env_get(Env* env, const char* name);

int env_set(Env* env, const char* name, Value* value);

void env_free(Env* env);
