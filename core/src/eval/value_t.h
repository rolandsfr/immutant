typedef enum ValueType { VAL_NUMBER, VAL_BOOL, VAL_STRING, VAL_NULL } ValueType;

typedef struct Value {
	ValueType type;
	union {
		long double number;
		int boolean;
		char* string;
	};
} Value;
