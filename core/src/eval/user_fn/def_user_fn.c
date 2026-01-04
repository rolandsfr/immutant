#include "def_user_fn.h"

#include "env.h"
#include "eval_user_fn.h"
#include "value_t.h"

void def_user_fn(FunDeclStmt* declaration, Env* env, Error* err)
{
	Value fn_value = create_user_fn(env, declaration);
	env_define_fn(env, declaration->name, fn_value, declaration->purity);
}
