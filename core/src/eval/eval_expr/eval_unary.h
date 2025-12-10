typedef struct Value Value;
typedef struct RuntimeError RuntimeError;
typedef struct UnaryExpr UnaryExpr;

Value eval_unary(struct UnaryExpr* expr, RuntimeError* err);
