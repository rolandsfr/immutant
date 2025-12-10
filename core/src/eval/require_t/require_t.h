typedef struct Value Value;
typedef struct RuntimeError RuntimeError;

int require_number(Value value, RuntimeError* err);
int require_string(Value value, RuntimeError* err);
int require_bool(Value value, RuntimeError* err);

int require_numbers(Value value1, Value value2, RuntimeError* err);

int check_strings(Value value1, Value value2, RuntimeError* err);
int check_numbers(Value value1, Value value2, RuntimeError* err);
