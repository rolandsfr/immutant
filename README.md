# the Immutant programming language

YAPL (Yet Another Programming Language) with useful quirks of immutability.

## General structure

```
├── 📁 interpreter #  cli tool for using the interpreter
│   └──  main.c # entry point for interpreter, treat this lib as cli
├── 📁 docs #  formal and detailed documentation of the language and underlying interpreter arch
├── 📁 core # core lib for interpreter
│   ├── 📁 include # public headers
│   ├── 📁 src # source and private headers
│   │   └── 📁 <moduleName>
│   └── 📁 tests # unit tests
│   └── ⚙️ project.yml # ceedling test runner config
└── README.md
```

## Tests

The project is structured according to the Pitchfork layout which contains both the source for the module and unit tests for it within same folder in `src/`.

Test runner used in this project is [ceedling](https://www.throwtheswitch.org/ceedling).
Running tests requires having ceedling being installed via ruby `gem`.

To run all tests use `ceedling test` within the core/ lib or otherwise `make test` in the root.

## Static libs

The parser is compiled once into a static library and linked into the three parser test executables:

```
make parser-lib   # → core/tests_build/lib/libparser.a
make test         # builds the library, then runs Ceedling
```

Parser integration tests include `src/test_support/parser_api.h` only (not individual `parse_*.h` headers) so Ceedling does not recompile the recursive-descent sources. Linking is configured per-test in `core/project.yml` (`:flags: :test: :link: :parsers/`). See `core/parser_lib.mk` for the archived source list.

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

## Specification

The official language specification is located in the `docs/` folder. `paper/` dir contain source files for resulting paper that was developed specifically to comply with requirements of University of Latvia Quality project.

On the other hand, [immutant-specification.md](docs/immutant-specification.md) contains language specification in english.

Since the language is still quite immature, the specification is subject to change. Please refer to the informal doc [ideas.md](docs/ideas.md) for ideas and proposals for future language features.
