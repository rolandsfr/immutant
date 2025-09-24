# the Immutant programming language
YAPL (Yet Another Programming language) with useful quirks of immutability.

## General structure

```
├── 📁 cli #  cli tool for using the interpreter
├── 📁 docs #  formal and detailed documentation of the 
├── 📁 interpreter 
│   ├── 📁 build
│   ├── 📁 include 
│   ├── ⚙️ project.yml # ceedling test runner config
│   ├── 📁 src 
│   └── 📁tests_build # artifacts of ceelding
└── README.md
```

## Running tests

This project uses [ceedling test runner for c projects]().
Running tests requires having ceedling being installed via ruby `gem`.

To run all tests use `ceedling test` within the interpreter project.

## Building