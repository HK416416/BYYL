#!/bin/bash
# run_all_tests.sh — 自动遍历所有 .cmm 测试文件，生成对应的 .ir 输出
# 用法: ./run_all_tests.sh [测试目录]

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
TEST_DIR="${1:-../Test}"

PARSER="${SCRIPT_DIR}/parser"

if [ ! -x "$PARSER" ]; then
    echo "Error: parser not found or not executable at $PARSER"
    echo "Please run 'make' first in $(dirname "$PARSER")"
    exit 1
fi

count=0
fail=0

echo "========================================"
echo "  Lab3.0 — Batch Test Runner"
echo "  Test dir: $TEST_DIR"
echo "========================================"
echo ""

for cmm in "$TEST_DIR"/*.cmm; do
    [ -f "$cmm" ] || { echo "No .cmm files found in $TEST_DIR"; exit 0; }
    base="${cmm%.cmm}"
    ir="${base}.ir"
    echo -n "[$(basename "$cmm")] → $(basename "$ir") ... "
    if "$PARSER" "$cmm" "$ir" >/dev/null 2>&1; then
        echo "OK"
        count=$((count + 1))
    else
        echo "FAIL (exit code $?)"
        fail=$((fail + 1))
        # Still produce .ir if it exists (e.g. error message)
    fi
done

echo ""
echo "========================================"
echo "  Done: $count passed, $fail failed"
echo "========================================"
