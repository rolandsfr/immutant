#pragma once

#include <stdlib.h>
#include <string.h>

#include "ast_stmt.h"
#include "value_t.h"

typedef struct EnvEntry {
	char* name;
	Value value;
	struct EnvEntry* next; // for chaining
						   // enum MutabilityType mutability;
						   // enum PurityType purity;
} EnvEntry;

typedef enum EnvPurity {
	ENV_PURE,
	ENV_IMPURE,
	ENV_PURITY_UNSET,
	ENV_INHERIT
} EnvPurity;

typedef struct Env {
	EnvPurity purity;
	struct Env* parent;
	EnvEntry* entries; // linked list or hash table
} Env;

Env* env_new(Env* parent, EnvPurity purity);

void env_define(Env* env, const char* name, Value value,
				enum MutabilityType mutability);

void env_define_var(Env* env, const char* name, Value value,
					enum MutabilityType mutability);

void env_define_fn(Env* env, const char* name, Value value,
				   enum PurityType purity);

Value* env_get(Env* env, const char* name);

EnvEntry env_get_entry(Env* env, const char* name);

EnvEntry* env_get_direct_entry(Env* env, const char* name);

int env_set(Env* env, const char* name, Value value);

void env_free(Env* env);
