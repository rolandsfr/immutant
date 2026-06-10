# Test-only static library: full parser subsystem (compile once, link separately).
# Included from the repo root Makefile.

CORE_DIR ?= core
TEST_LIB_DIR := $(CORE_DIR)/tests_build/lib
PARSER_LIB := $(TEST_LIB_DIR)/libparser.a

INCLUDES := $(shell find $(CORE_DIR)/src -type d)
PARSER_CFLAGS := -std=c11 -Wall -Wextra -DTEST -MMD -MP \
	$(foreach dir,$(INCLUDES),-I$(dir)) -I$(CORE_DIR)/include

PARSER_SRCS := \
	$(CORE_DIR)/src/parser/parse_expr.c \
	$(CORE_DIR)/src/parser/parse_stmt.c \
	$(CORE_DIR)/src/parser/parser.c \
	$(CORE_DIR)/src/ast/expr/ast_make_expr.c \
	$(CORE_DIR)/src/ast/stmt/ast_make_stmt.c \
	$(CORE_DIR)/src/error/error.c \
	$(CORE_DIR)/src/error/error_report.c \
	$(CORE_DIR)/src/lexer/lexer.c \
	$(CORE_DIR)/src/resolve/resolve.c

PARSER_OBJS := $(PARSER_SRCS:$(CORE_DIR)/src/%.c=$(TEST_LIB_DIR)/%.o)
PARSER_DEPS := $(PARSER_OBJS:.o=.d)

.PHONY: parser-lib parser-lib-clean

parser-lib: $(PARSER_LIB)
	$(quiet) "→ $(PARSER_LIB)"

$(TEST_LIB_DIR)/%.o: $(CORE_DIR)/src/%.c
	$(Q)mkdir -p $(dir $@)
	$(quiet) "  CC  $<"
	$(Q)$(CC) $(PARSER_CFLAGS) -c $< -o $@

$(PARSER_LIB): $(PARSER_OBJS)
	$(Q)mkdir -p $(TEST_LIB_DIR)
	$(quiet) "  AR  $@"
	$(Q)$(AR) rcs $@ $^

-include $(PARSER_DEPS)
