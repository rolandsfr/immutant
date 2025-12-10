#pragma once

typedef enum RuntimeErrorType RuntimeErrorType;
typedef struct RuntimeError RuntimeError;

RuntimeError make_runtime_error(RuntimeErrorType type, const char* message);
