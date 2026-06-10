# Compiler
CC := gcc
AR := ar
LDFLAGS :=

# Quiet by default; run `make V=1` to see full compiler commands
ifeq ($(V),1)
  Q :=
  quiet = @true
else
  Q := @
  quiet = @printf '%s\n'
endif

# Parallel builds (override with MAKEFLAGS=-j1 for single-threaded)
NPROC := $(shell sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4)
MAKEFLAGS += -j$(NPROC)

INCLUDES := $(shell find core/src -type d)
CFLAGS := -std=c11 -Wall -Wextra -MMD -MP \
	$(foreach dir,$(INCLUDES),-I$(dir)) -Icore/include

# Directories
BUILD_DIR := build
CORE_BUILD := $(BUILD_DIR)/core
CLI_BUILD := $(BUILD_DIR)/interpreter
DEP_DIR := $(BUILD_DIR)/deps

# Core sources (exclude test-only code)
CORE_SRC := $(shell find core/src -name '*.c' ! -name '*.test.c' ! -path 'core/src/test_support/*')
CORE_OBJS := $(CORE_SRC:core/src/%.c=$(CORE_BUILD)/%.o)
CORE_DEPS := $(CORE_OBJS:.o=.d)
LIBCORE := $(BUILD_DIR)/libcore.a

# CLI sources and objects
CLI_SRC := $(shell find interpreter -name "*.c")
CLI_OBJ := $(CLI_SRC:interpreter/%.c=$(CLI_BUILD)/%.o)
CLI_DEPS := $(CLI_OBJ:.o=.d)
BIN := $(BUILD_DIR)/immutant

# Default target
all: $(BIN)
	$(quiet) "→ $(BIN)"

# Build core objects (preserve folder structure)
$(CORE_BUILD)/%.o: core/src/%.c
	$(Q)mkdir -p $(dir $@)
	$(quiet) "  CC  $<"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

# Build core static library
$(LIBCORE): $(CORE_OBJS)
	$(Q)mkdir -p $(dir $@)
	$(quiet) "  AR  $@"
	$(Q)$(AR) rcs $@ $^

# Build CLI objects
$(CLI_BUILD)/%.o: interpreter/%.c
	$(Q)mkdir -p $(dir $@)
	$(quiet) "  CC  $<"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

# Link CLI binary
$(BIN): $(CLI_OBJ) $(LIBCORE)
	$(quiet) "  LD  $@"
	$(Q)$(CC) $^ -o $@ $(LDFLAGS)

# Pull in header dependencies when present
-include $(CORE_DEPS) $(CLI_DEPS)

# Run CLI
run: $(BIN)
	@$(BIN)

# Parser static library for tests (see core/parser_lib.mk)
include core/parser_lib.mk

# Run Ceedling tests in core/ (parser integration tests link tests_build/lib/libparser.a)
test: parser-lib
	@cd core && ceedling test

# Clean build artifacts
clean: parser-lib-clean
	rm -rf $(BUILD_DIR)

.PHONY: all run test clean parser-lib parser-lib-clean
