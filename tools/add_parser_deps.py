#!/usr/bin/env python3

import pathlib
import subprocess
import re
import logging
import argparse

# ---------------- CONFIG ----------------
SRC_DIR = pathlib.Path("core/src/parser/parse_expr")
TEST_DIR = pathlib.Path("core/tests/unit/parsers")

# List of source files (base names) to ignore for checking test files
IGNORE_SRC_FILES = [
    "parse_asgn.c",  
    "parse_block_stmt.c",
    "parse_call.c",
    "parse_unary.c",
    "parse_primary.c",
]

# ----------------------------------------

INCLUDE_RE = re.compile(r'#include\s+"([^"]+)"')


def setup_logging(verbose: bool):
    level = logging.DEBUG if verbose else logging.INFO
    logging.basicConfig(level=level, format="%(levelname)s: %(message)s")


def get_all_headers() -> list[str]:
    headers = [f.name for f in SRC_DIR.glob("*.h")]
    logging.debug("Found headers: %s", headers)
    return headers


def test_file_for(src_file: pathlib.Path) -> pathlib.Path:
    return TEST_DIR / f"{src_file.stem}.test.c"


def include_already_present(test_text: str, header: str) -> bool:
    for match in INCLUDE_RE.finditer(test_text):
        if match.group(1) == header:
            return True
    return False


def add_includes(test_file: pathlib.Path, headers: list[str]) -> bool:
    logging.info("Checking %s for parser includes", test_file)
    text = test_file.read_text(encoding="utf-8")
    lines = text.splitlines()
    insert_idx = 0
    for i, line in enumerate(lines):
        if line.startswith("#include"):
            insert_idx = i + 1

    changed = False
    for header in headers:
        if not include_already_present(text, header):
            logging.info("  Adding #include \"%s\"", header)
            lines.insert(insert_idx, f'#include "{header}"')
            insert_idx += 1
            changed = True
        else:
            logging.debug("  Already included: %s", header)

    if changed:
        test_file.write_text("\n".join(lines) + "\n", encoding="utf-8")
    return changed


def clang_format(path: pathlib.Path):
    logging.info("Running clang-format on %s", path)
    subprocess.run(["clang-format", "-i", str(path)], check=True)


def main():
    parser = argparse.ArgumentParser(
        description="Add parser header includes to test files (recursive dependencies)"
    )
    parser.add_argument("-v", "--verbose", action="store_true", help="Enable debug logging")
    args = parser.parse_args()
    setup_logging(args.verbose)

    logging.info("Starting parser include update")
    logging.info("Source dir: %s", SRC_DIR)
    logging.info("Test dir:   %s", TEST_DIR)

    all_headers = get_all_headers()
    modified_tests = set()

    for src_file in SRC_DIR.glob("*.c"):
        if src_file.name in IGNORE_SRC_FILES:
            logging.info("Skipping check for test file of %s (ignored)", src_file)
            continue

        test_file = test_file_for(src_file)
        if not test_file.exists():
            logging.warning("Missing test file: %s", test_file)
            continue

        changed = add_includes(test_file, all_headers)
        if changed:
            modified_tests.add(test_file)

    for test_file in modified_tests:
        clang_format(test_file)

    logging.info("Done. Modified %d test files.", len(modified_tests))


if __name__ == "__main__":
    main()
