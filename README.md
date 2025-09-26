# the Immutant programming language
YAPL (Yet Another Programming language) with useful quirks of immutability.

## General structure

```
├── 📁 interpreter #  cli tool for using the interpreter
│   └──  main.c # entry point for interpreter, treat this lib as cli
├── 📁 docs #  formal and detailed documentation of the 
├── 📁 core # exposes core parts of interpreter for use in the resulting interpeting tool
│   ├── 📁 include 
│   ├── 📁 src 
│   └── ⚙️ project.yml # ceedling test runner config
└── README.md
```

## Running tests

This project uses [ceedling test runner for c projects]().
Running tests requires having ceedling being installed via ruby `gem`.

To run all tests use `ceedling test` within the core/ lib or otherwise `make test` in the root.

## Building