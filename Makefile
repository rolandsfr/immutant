# Compiler
CC := gcc
AR := ar
CFLAGS := -std=c11 -Wall -Wextra -Icore/include -Icore/src
LDFLAGS :=

# Directories
BUILD_DIR := build
CORE_BUILD := $(BUILD_DIR)/core
CLI_BUILD := $(BUILD_DIR)/interpreter

# Core sources (exclude *.test.c)
CORE_SRC := $(filter-out %.test.c,$(wildcard core/src/**/*.c))
CORE_OBJS := $(patsubst core/src/%.c,$(CORE_BUILD)/%.o,$(CORE_SRC))
LIBCORE := $(BUILD_DIR)/libcore.a

# CLI sources and objects
CLI_SRC := interpreter/main.c
CLI_OBJ := $(CLI_BUILD)/main.o
BIN := $(BUILD_DIR)/immutant

# Default target
all: $(BIN)

# Build core objects
$(CORE_BUILD)/%.o: core/src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Build core static library
$(LIBCORE): $(CORE_OBJS)
	@mkdir -p $(dir $@)
	$(AR) rcs $@ $^

# Build CLI object
$(CLI_OBJ): $(CLI_SRC) $(LIBCORE)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Link CLI binary
$(BIN): $(CLI_OBJ) $(LIBCORE)
	$(CC) $^ -o $@ $(LDFLAGS)

# Run CLI
run: $(BIN)
	./$(BIN)

# Run Ceedling tests in core/
test:
	@cd core && ceedling test


# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean
