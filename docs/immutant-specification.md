# Immutant Programming Language - Getting Started Guide

## Introduction

Immutant is an interpreted programming language designed with immutability semantics at its core. The primary goal of the language is to help programmers write more predictable code by minimizing the risk of side effects caused by data mutability. This document provides a comprehensive specification of the language features and syntax.

## Table of Contents

1. [Data Types](#data-types)
2. [Variables](#variables)
3. [Operators and Expressions](#operators-and-expressions)
4. [Functions](#functions)
5. [Control Flow](#control-flow)
6. [Keywords](#keywords)
7. [Formal Grammar](#formal-grammar)

---

## Data Types

The Immutant language defines only primitive data types, including numbers, strings, and boolean values. This is subject to change and is likely for composite data structures, such as arrays and objects, to be introduced at a later stage.

### Strings

A string is any sequence of characters enclosed in double quotes. Strings can contain any characters, including special symbols like `@`, `^`, `&`, etc. String literals can contain language keywords such as `mut` without issues.

Examples of valid strings:

```
"", "someString", "unicode characters", " ", "\""
```

#### String Escaping

The quote character inside a string must be escaped with a backslash (`\`) to prevent it from being interpreted as the end of the string.

Example:

```
"They call themselves \"the wolves\""
```

This results in the text:

```
They call themselves "the wolves"
```

The backslash character itself must also be escaped:

```
"There is something mysterious about the \"\\\" character."
```

Results in:

```
There is something mysterious about the "\" character.
```

### Numbers

A number is any value from the set of real numbers. It's important to note that irrational numbers are implemented according to the IEEE 754 standard, so they are not truly irrational but rather approximated values. Decimal fractions are separated with a period (`.`).

Examples of numeric values:

```
102, -9, -0.3, 421.5632
```

The language grammar allows using a leading period before a number to denote a decimal fraction of zero. For example, `.34` is identical to `0.34`.

### Boolean Values

Boolean values in Immutant follow two-valued logic. True values are denoted by `true`, and false values by `false` keyword.

---

## Variables

Immutant distinguishes between 2 types of variables: those that do not rely on data mutation (immutable), in other words, constants, and those that do (mutable).

### Variable Declaration and Initialization

Variable declaration and initialization can occur in one step, or they can be performed separately. However, keep in mind that a variable must be declared before its initialization or any other use in the program; otherwise, a `RUNTIME_ERR-2` exception is thrown.

Variables are declared in the following form:

```
<mutability> <identifier>;
```

Variable declaration begins with one of 2 keywords `<mutability>` - `mut` and `imut` - which determines the variable's mutability property.

Variable declaration ends with an arbitrary identifier `<identifier>` - the variable name. An identifier can contain letters, digits, and the underscore character (`_`), but cannot start with a digit. Additionally, an identifier cannot match any of the language's reserved keywords, such as `mut`, `imut`, `if`, `else`, etc. (see the full list of keywords in the Keywords section).

Variable initialization must follow one of the following forms:

```
<identifier> = <expression>;
```

or

```
<mutability> <identifier> = <expression>;
```

Second notation declares a variable, so can only be done for the same variable.

To initialize a variable (i.e., assign it a value), one must use the assignment operator symbol (`=`), followed by an expression `<expression>` that returns a value to be assigned to the variable.

### Variable Mutability

Variable mutability determines whether a variable's value can be changed after its initialization. In Immutant, there are two types of variables: **mutable** and **immutable**, declared using the keywords `mut` and `imut` respectively.

#### Mutable Variables (`mut`)

A mutable variable is one whose value can be changed after its initialization. Such variables are declared with the keyword `mut`.

Any variable declared with `mut` can be assigned a new value at any point in program execution after its initialization. This allows programmers to create more dynamic programs where data can change depending on program logic and user input. However, it is recommended to use mutable variables only when solutions with immutable variables are too complex or inefficient.

#### Immutable Variables (`imut`)

Immutable variables in their essence are constants and during their lifetime can hold only one specific value. Such variables are declared with keyword `imut`.

Immutable variables must not be re-initialized, such behavior will throw error `RUNTIME_ERR-11`:

```
imut x = 10;
x = 20; // error
```

### Data Type Conversion

There are cases when it becomes necessary to convert a value of one data type to another. For example, if some arithmetic operator, such as addition, is applied to a string and a number, it is necessary to convert one of the operands to the other's data type to perform this operation.

- **`toString(value)`** - Returns the `value` converted to string data type. The conversion logic transforms the given value into a string using its base representation. For example, the number `42` is converted to the string `"42"`, and a boolean value is converted to the string `"true"` or `"false"` respectively.

- **`toNumber(value)`** - Returns the numeric representation of the value. If a string contains a valid numeric value, it is converted to the corresponding number. For example, the string `"123.45"` is converted to the number `123.45`. If the string does not contain a valid numeric value, exception `RUNTIME_ERR-12` is thrown. The true value `true` is converted to number `1`, and `false` to number `0`. A string is considered a valid numeric value if it contains only a number or decimal number. If there are whitespace characters around the number, they are ignored. For example, strings `"  42  "` and `"-3.14"` are valid numeric values, but strings `"123abc"` and `"hello"` are not valid.

- **`toBoolean(value)`** - Converts the given operand value to a boolean value. Values `0`, `""` (empty string), and `false` are converted to false, but everything else to true.

- **`typeOf(value)`** - Returns the data type of the given operand as a string. Accepts a value of any data type as an argument and returns one of the following string values: `"number"`, `"string"`, `"boolean"`, `"function"`.

### Variable Scope

Variable scope determines which parts of the program have access to a specific variable. In Immutant, variable scope is determined by lexical scope and purity type of any of the parent scopes.

Example of a global variable declaration and its use in a function:

```
imut globalVar = 10;

fn printGlobalVar() {
  print(globalVar);
}

printGlobalVar(); // Outputs: 10
```

Variables declared outside any function body are accessible throughout the program code, including functions, though, some restrictions apply to pure functions.

Example of a local variable declaration and its use in a function:

```
fn doMagic() {
  imut localVar = 5;
  if(true) {
    mut anotherLocalVar = localVar + 10;
  }
}
doMagic();
```

Variables that are attempted to be used outside their declaration scope throw error `RUNTIME_ERR-2`. By its nature, this is using undeclared variables. See the example below:

```
fn setLocalVar() {
  imut localVar = 5;
}
setLocalVar();
print(localVar); // Error RUNTIME_ERR-2
```

---

## Operators and Expressions

### Expressions

Immutant language defines following types of expressions:

- `logical` expression return boolean value
- `arithmetic` expression return number value
- `string`, `number` and `boolean` expressions return themselves
- `variable` return variable value
- `assignment` returns assignment target

### Operators

Operators are divided into unary and binary operators. A unary operator is applied to only one operand, while binary operators are applied to 2 operands. For example, the negation operator that inverts a boolean value is applied to one operand, but the addition operator is applied to two numbers, producing their sum.

It's worth noting that if an operator is binary, it is written between both operands. Hereafter in the text, the left operand is defined as the operand of such a binary operator that is located to the left of the operator, and the right one is correspondingly to the right of it. For example, in the expression `23 + 45`, the number `23` is the left operand, `45` is the right operand, and `+` is the binary sum operator.

#### Arithmetic Operators

Applied to numbers and return a numeric value:

- `+` addition, returns the sum of two numbers
- `-` subtraction, returns the first number's value minus the second number's value
- `*` multiplication, returns the product of two numbers
- `/` division, returns the left operand divided by the right operand. If the right operand is zero, error `RUNTIME_ERR-5` is thrown
- `%` modulus, returns the left operand modulo the right operand value

#### Arithmetic Comparison Operators

Applied to numbers and return a boolean value:

- `>` greater than, returns true if the left operand value is greater than the right operand value
- `<` less than, returns true if the left operand value is less than the right operand value
- `>=` greater than or equal to, returns true if the left operand value is greater than or equal to the right operand value
- `<=` less than or equal to, returns true if the left operand value is less than or equal to the right operand value

#### Equality Operators

Applied to operands of any data type and return a boolean value:

- `==` equals, returns true if both operands' values are equal. Always returns false if both operands have different data types
- `!=` not equal to, returns true if both operands' values are not equal, including if their data types are different

#### Logical Operators

Applied to boolean values and return a boolean value:

- `!` negation, inverts the boolean value. This is a unary operator
- `&&` conjunction (AND), returns true if both operands' values are true
- `||` disjunction (OR), returns true if at least one of the operands' values is true

### Operator Precedence

Each operator has its own priority, that is, the order in which sub-expressions with multiple operators are executed. For example, the multiplication operator has higher priority than the addition operator, so the expression `2 + 3 * 4` is evaluated as `2 + (3 * 4)`, not `(2 + 3) * 4`.

Below are operators listed in order of their precedence, from highest to lowest. Operators with equal precedence are mentioned together in one list item:

1. `!` (unary negation operator)
2. `*`, `/`, `%` (arithmetic multiplication, division, and modulus operators)
3. `+`, `-` (arithmetic addition and subtraction operators)
4. `>`, `<`, `>=`, `<=` (arithmetic comparison operators)
5. `==`, `!=` (equality operators)
6. `&&` (conjunction)
7. `||` (disjunction)

It's critical to note that the execution order of different operators can be artificially raised using parentheses. Expressions in parentheses are executed first, regardless of the precedence of the operators contained within them.

For clarity, here's an example with a compound expression and its evaluation order:

```
3 + 4 % 2 == 3 || true && false
```

Alternatively, it can be rewritten with parentheses that illustrate the evaluation order:

```
((3 + (4 % 2)) == 3) || (true && false)
```

### Evaluation Order

Evaluation happens left to right, first evaluating higher precedence expressions.

### Short-Circuit Evaluation

For interpreter optimization purposes, the language specification also includes a short-circuit evaluation mechanism that allows avoiding the evaluation of unnecessary sub-expressions in logical expressions with disjunction.

By definition, a logical expression with disjunction is true if at least one of its operands is true. Since expressions in Immutant are evaluated from left to right, it is sufficient to evaluate only the left operand if it is true.

For example, the expression `true || (someFunction() + 5 > 10)` evaluates to true, and the interpreter will not evaluate the right operand.

---

## Functions

Functions are fully or partially isolated program code that can be called and used with different parameters, in different places, and at different times. Functions themselves are part of the program code, however, in order for them to be executed, they need to be called.

Functions are defined in the following form:

```
<purity> fn <identifier>(<parameters>) {
    <statements>
}
```

`<purity>` denotes the function's property to change data outside its context. In computer science, "impure" functions are those that change external states or depend on them. Conversely, "pure" functions are those that do not affect external states and depend only on their parameters. Using pure functions in source code is recommended because it makes the program more predictable and easier to understand, as it's known that pure functions can only call other pure functions or expressions, which means they never change external state. Thus, the `pure` keyword is used to define a pure function, and `impure` for an impure function.

### Pure and Impure Functions

If a function is defined as pure, its argument values are passed by value. This means that if an argument is a variable, changing its value within the function will change the variable's value only within the function's scope, not in the surrounding context. In other words, any variable passed as an argument to a pure function is re-declared and initialized in the function's internal context.

Additionally, it is forbidden to call impure functions within the body of pure functions.

Conversely, impure function arguments are passed as references to their original values. This means that if an argument is a variable, changing its value within the function will also change the variable's value in the external context. Within impure function bodies, it is allowed to call both pure and impure functions.

Using the `pure` keyword is not mandatory, as user-defined functions in Immutant are considered pure by default.

### Function Definition and Invocation

```
fn <identifier>(<parameters>) {
    <statements>
}
```

In the example above, the function is defined as pure because none of the purity property keywords are used. The keyword `fn` indicates that a function definition follows. `<identifier>` contains the function name, which is a unique identifier in memory. The same rules apply to function names as mentioned in the Variables section about variable identifiers.

After the function name come parentheses containing the parameter list `<parameters>`. Parameters are variables in the function's internal context that are passed to the function when it's called, and whose values the function uses in its operation. The parameter list is a comma-separated sequence of parameters. If the function has no parameters, the parentheses are empty.

The function body `<statements>` is enclosed in curly braces and contains statements and expressions that are executed when the function is called. The function body must contain at least one statement or expression. If the function is pure, its body must not call impure functions or perform other actions that change external state.

Example of a pure function that calculates the sum of two numbers:

```
pure fn add(a, b) {
    return a + b;
}
```

Example of an impure function that adds a new number to a global variable:

```
mut num = 4;
impure fn addGlobal(newNum) {
    num = num + newNum;
}
```

A function call is an expression containing the function name followed by parentheses with an argument list. Arguments are values passed to the function when it's called, and whose values the function uses in its operation. The argument list is a comma-separated sequence of arguments. If the function has no arguments, the parentheses are empty. Arguments are passed to the function sequentially, in the same order as they are defined in the function's parameter list.

Examples of function calls:

```
addGlobal(5);
imut sum = add(3, 7);
```

### Built-in Functions

The Immutant language has the following built-in functions defined:

- **`print(value)`** - Outputs the given operand's value to the console. Accepts a value of any data type as an argument and outputs its representation as a string to the console.

- **`time()`** - Returns the current time in milliseconds since January 1, 1970, 00:00:00 UTC.

- **`toString(value)`, `toNumber(value)`, `toBoolean(value)`, `typeOf(value)`** - Data type conversion functions described in the Data Type Conversion section.

- **`input()`** - Reads user input from standard input as a string. The function waits until the user enters data and presses the Enter key, then returns the entered value as a string.

---

## Control Flow

Control flow is a mechanism that determines the execution sequence of a program. Immutant defines the following control flow mechanisms: `if-else` branching and `while` loop.

### The if-else Statement

The `if-else` statement allows executing different parts of code depending on condition fulfillment. The syntax is as follows:

```
if (<condition>) {
    <statements>
} else {
    <statements>
}
```

`<condition>` is a logical expression that returns a boolean value. If the expression's value is true, the statements `<statements>` inside the first curly braces are executed. If the expression's value is false, the statements `<statements>` inside the second curly braces after the keyword `else` are executed.

```
if (<condition1>) {
    <statements1>
} else {
    <statements3>
}
```

Additionally, the `else` statement is not mandatory and can be omitted if there's no need to execute code when none of the conditions are met.

Example program that outputs an appropriate message depending on a number's value:

```
imut num = 10;

if (num > 0) {
    print("Number is greater than zero");
} else if (num < 0) {
    print("Number is less than zero");
} else {
    print("Number is zero");
}
```

### Loops

The `while` loop allows executing a code section repeatedly while a condition is met. The syntax is as follows:

```
while (<condition>) {
    <statements>
}
```

`<condition>` is a logical expression that returns a boolean value. If the expression's value is true, the statements `<statements>` inside the curly braces are executed. After the statements are executed, the condition is checked again, and if it's still true, the statements are executed again. This process continues until the condition becomes false. If the condition is initially false, the loop body is not executed at all.

Cyclic operation can be demonstrated with Fibonacci number sequence generation:

```
// Demonstrates variable usage in a loop
mut prev = 0;
mut curr = 1;
mut count = 0;
imut n = 10;

while (count < n) {
    imut val = curr;
    print(val);
    imut next = prev + curr;
    prev = curr;
    curr = next;
    count = count + 1;
}
```

---

## Keywords

The Immutant language defines the following keywords that are reserved for language syntax and cannot be assigned another meaning:

- `imut` - used to declare immutable variables
- `mut` - used to declare mutable variables
- `fn` - used to define functions
- `pure` - used to define pure functions
- `impure` - used to define impure functions
- `if` - used to define conditional statements
- `else` - used to define conditional statement alternatives
- `while` - used to define loops
- `return` - used to return a value from a function
- `true` - boolean true value
- `false` - boolean false value

---

## Formal Grammar

The formal grammar of the Immutant language, described in BNF form, defines the language specification - what can expand into what else. Terminals are denoted with uppercase letters, and non-terminals with lowercase letters.

```
PROGRAM:
  program             -> statement* EOF

STATEMENTS:
  statement           -> var_declaration
                      | function_declaration
                      | if_statement
                      | while_statement
                      | return_statement
                      | block_statement
                      | expression_statement

  var_declaration     -> mutability IDENTIFIER
                         ( "=" expression )? ";"

  mutability          -> "mut" | "imut"

  function_declaration -> purity? "fn" IDENTIFIER "("
                          parameters? ")" block_statement

  purity              -> "pure" | "impure"

  parameters          -> IDENTIFIER ( "," IDENTIFIER )*

  if_statement        -> "if" "(" expression ")"
                         block_statement
                         ( "else" block_statement )?

  while_statement     -> "while" "(" expression ")"
                         block_statement

  return_statement    -> "return" expression? ";"

  block_statement     -> "{" statement* "}"

  expression_statement -> expression ";"

EXPRESSIONS:
  expression          -> assignment

  assignment          -> logical_or
                         ( "=" logical_or )?

  logical_or          -> logical_and
                         ( "||" logical_and )*

  logical_and         -> equality
                         ( "&&" equality )*

  equality            -> comparison
                         ( ( "!=" | "==" ) comparison )*

  comparison          -> additive
                         ( ( ">" | ">=" | "<" | "<=" )
                           additive )*

  additive            -> multiplicative
                         ( ( "-" | "+" ) multiplicative )*

  multiplicative      -> unary
                         ( ( "/" | "*" | "%" ) unary )*

  unary               -> ( "!" ) unary
                      | call

  call                -> primary ( "(" arguments? ")" )*

  arguments           -> expression ( "," expression )*

  primary             -> NUMBER
                      | STRING
                      | "true"
                      | "false"
                      | IDENTIFIER
                      | "(" expression ")"
```

### Notation Legend

- `|` denotes alternative (or)
- `*` denotes zero or more times (Kleene star)
- `?` denotes zero or one time (optionality)
- `( )` denotes a group

---

## Quick Start Example

Here's a simple example to get you started with Immutant:

```
// Calculate factorial using a while loop
imut n = 5;
mut result = 1;
mut i = 1;

while (i <= n) {
    result = result * i;
    i = i + 1;
}

print("Factorial of " + toString(n) + " is: " + toString(result));
```

---

## Common Runtime Errors

- `RUNTIME_ERR-2` - Using an undeclared variable or accessing a variable outside its scope
- `RUNTIME_ERR-5` - Division by zero
- `RUNTIME_ERR-8` - Function-related errors (e.g., incorrect number of arguments)
- `RUNTIME_ERR-11` - Attempting to reassign an immutable variable
- `RUNTIME_ERR-12` - Invalid type conversion (e.g., converting non-numeric string to number)
- `RUNTIME_ERR-13` - Calling an impure function from a pure function context

---

## Running Immutant Programs

### REPL Mode

Run the interpreter without arguments to enter interactive REPL mode:

```bash
./immutant
```

### File Execution

Execute a source file:

```bash
./immutant source.imt
```

---

## Additional Resources

For more examples, see the `examples/` directory in the project repository:

- `fibonacci.imut` - Fibonacci sequence generation
- `math_utils.imut` - Mathematical utility functions
- `primes.imut` - Prime number calculations
- `temperature.imut` - Temperature conversion

---

_Immutant programming language specification was developed as part of a qualification work at the University of Latvia, Faculty of Exact Sciences and Technology._
