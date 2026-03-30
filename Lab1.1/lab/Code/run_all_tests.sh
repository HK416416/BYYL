#!/bin/bash

# 颜色
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# 结果文件
RESULT_FILE="test_results.txt"
> $RESULT_FILE

echo "========================================" | tee -a $RESULT_FILE
echo "Test started at: $(date)" | tee -a $RESULT_FILE
echo "========================================" | tee -a $RESULT_FILE
echo "" | tee -a $RESULT_FILE

# 测试目录（相对于 Code 目录）
TEST_BASE="../Test"

# 要测试的文件夹列表
FOLDERS="cases comment expr function literal public statement struct"

TOTAL=0
PASS=0
FAIL=0

# 先测试 test1.cmm 和 test2.cmm
echo "========== Single Test Files ==========" | tee -a $RESULT_FILE

for test_file in test1.cmm test2.cmm; do
    if [ -f "$TEST_BASE/$test_file" ]; then
        TOTAL=$((TOTAL + 1))
        echo "" | tee -a $RESULT_FILE
        echo ">>> Testing: $test_file" | tee -a $RESULT_FILE
        
        output=$(./parser "$TEST_BASE/$test_file" 2>&1)
        exit_code=$?
        
        if [ $exit_code -eq 0 ]; then
            echo -e "${GREEN}>>> Result: SUCCESS (exit: $exit_code)${NC}" | tee -a $RESULT_FILE
            PASS=$((PASS + 1))
        else
            echo -e "${RED}>>> Result: FAILED (exit: $exit_code)${NC}" | tee -a $RESULT_FILE
            FAIL=$((FAIL + 1))
        fi
        
        if [ -n "$output" ]; then
            echo ">>> Output:" | tee -a $RESULT_FILE
            echo "$output" | tee -a $RESULT_FILE
        fi
        echo "----------------------------------------" | tee -a $RESULT_FILE
    fi
done

# 测试各个文件夹
for folder in $FOLDERS; do
    TEST_DIR="$TEST_BASE/$folder"
    
    if [ ! -d "$TEST_DIR" ]; then
        echo -e "${YELLOW}Warning: $TEST_DIR not found, skipping...${NC}" | tee -a $RESULT_FILE
        continue
    fi
    
    echo "" | tee -a $RESULT_FILE
    echo "========== Folder: $folder ==========" | tee -a $RESULT_FILE
    
    # 遍历该文件夹下所有 .cmm 文件
    for cmm_file in "$TEST_DIR"/*.cmm; do
        if [ ! -f "$cmm_file" ]; then
            echo "No .cmm files found in $TEST_DIR" | tee -a $RESULT_FILE
            break
        fi
        
        TOTAL=$((TOTAL + 1))
        filename=$(basename "$cmm_file")
        
        echo "" | tee -a $RESULT_FILE
        echo ">>> Testing: $folder/$filename" | tee -a $RESULT_FILE
        
        output=$(./parser "$cmm_file" 2>&1)
        exit_code=$?
        
        # 判断结果
        if [ $exit_code -eq 0 ]; then
            # 检查是否有语法错误输出（stderr 中的 Error type）
            if echo "$output" | grep -q "Error type"; then
                echo -e "${YELLOW}>>> Result: SYNTAX ERROR (detected but exit 0)${NC}" | tee -a $RESULT_FILE
            else
                echo -e "${GREEN}>>> Result: SUCCESS${NC}" | tee -a $RESULT_FILE
                PASS=$((PASS + 1))
            fi
        else
            echo -e "${RED}>>> Result: FAILED (exit: $exit_code)${NC}" | tee -a $RESULT_FILE
            FAIL=$((FAIL + 1))
        fi
        
        # 记录输出（如果有）
        if [ -n "$output" ]; then
            echo ">>> Output:" | tee -a $RESULT_FILE
            echo "$output" | tee -a $RESULT_FILE
        fi
        
        echo "----------------------------------------" | tee -a $RESULT_FILE
    done
done

# 汇总
echo "" | tee -a $RESULT_FILE
echo "========================================" | tee -a $RESULT_FILE
echo "SUMMARY:" | tee -a $RESULT_FILE
echo "  Total tests: $TOTAL" | tee -a $RESULT_FILE
echo -e "  ${GREEN}Success: $PASS${NC}" | tee -a $RESULT_FILE
echo -e "  ${RED}Failed: $FAIL${NC}" | tee -a $RESULT_FILE
echo "========================================" | tee -a $RESULT_FILE
echo "Test completed at: $(date)" | tee -a $RESULT_FILE
echo "Results saved to: $RESULT_FILE"
