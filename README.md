# the Immutant programming language

YAPL (Yet Another Programming language) with useful quirks of immutability.

## General structure

```
├── 📁 interpreter #  cli tool for using the interpreter
│   └──  main.c # entry point for interpreter, treat this lib as cli
├── 📁 docs #  formal and detailed documentation of the language and underlying interpreter arch
├── 📁 core # exposes core parts of interpreter for use in the resulting interpeting tool
│   ├── 📁 include # public header
│   ├── 📁 src # source
│   │   └── 📁 <moduleName>
│   │       ├── 📁 <module.c>
│   │       └── 📁 <module.test.c>
│   └── ⚙️ project.yml # ceedling test runner config
└── README.md
```

## Tests

The project is structured according to the Pitchfork layout which contains both the source for the module and unit tests for it within same folder in `src/`.

Test runner used in this project is [ceedling](https://www.throwtheswitch.org/ceedling).
Running tests requires having ceedling being installed via ruby `gem`.

To run all tests use `ceedling test` within the core/ lib or otherwise `make test` in the root.

## LSP Context
This project uses ceedling plugin `compile_commands_json_db` that generates `compile_commands.json` file for better LSP support (including ceedling vendor files too). It will be generated automatically when running tests in `core/tests_build/artifacts/compile_commands.json`. For some Unix based systems you may need symlink it to the root of the project for LSP to pick it up:

```
ln -s core/tests_build/artifacts/compile_commands.json compile_commands.json
```


## Building

To build the binary, make sure you have GNU GCC compiler installed and available in your PATH.

```
make
```

Running the command above in the root will create a binary `build/immutant` that is the interpreter itself.
