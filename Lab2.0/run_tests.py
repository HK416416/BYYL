#!/usr/bin/env python3
import os
import subprocess
import sys

def run_tests():
    test_dir = "Test"
    parser_path = "Code/parser"
    
    if not os.path.exists(parser_path):
        print(f"错误: 解析器未找到: {parser_path}")
        sys.exit(1)
    
    if not os.path.exists(test_dir):
        print(f"错误: 测试目录未找到: {test_dir}")
        sys.exit(1)
    
    # 获取所有 .cmm 文件
    cmm_files = []
    for root, dirs, files in os.walk(test_dir):
        for file in files:
            if file.endswith(".cmm"):
                cmm_files.append(os.path.join(root, file))
    
    cmm_files.sort()
    print(f"找到 {len(cmm_files)} 个测试文件")
    
    results = []
    
    for test_file in cmm_files:
        print(f"\n测试: {test_file}")
        try:
            # 运行解析器
            result = subprocess.run(
                [parser_path, test_file],
                capture_output=True,
                text=True,
                timeout=5
            )
            
            output = result.stdout.strip()
            error = result.stderr.strip()
            return_code = result.returncode
            
            # 记录结果
            test_result = {
                "file": test_file,
                "output": output,
                "error": error,
                "return_code": return_code,
                "has_output": bool(output),
                "lines": output.count('\n') + 1 if output else 0
            }
            
            results.append(test_result)
            
            # 打印简要结果
            if output:
                print(f"  输出 ({test_result['lines']} 行):")
                for line in output.split('\n')[:3]:  # 只显示前3行
                    print(f"    {line}")
                if test_result['lines'] > 3:
                    print(f"    ... 还有 {test_result['lines'] - 3} 行")
            else:
                print("  无输出 (语法正确)")
                
            if error:
                print(f"  标准错误: {error}")
                
        except subprocess.TimeoutExpired:
            print(f"  超时")
            results.append({
                "file": test_file,
                "output": "",
                "error": "Timeout",
                "return_code": -1,
                "has_output": False,
                "lines": 0
            })
        except Exception as e:
            print(f"  错误: {e}")
            results.append({
                "file": test_file,
                "output": "",
                "error": str(e),
                "return_code": -1,
                "has_output": False,
                "lines": 0
            })
    
    # 汇总统计
    print("\n" + "="*60)
    print("测试结果汇总")
    print("="*60)
    
    total_tests = len(results)
    tests_with_output = sum(1 for r in results if r["has_output"])
    tests_without_output = total_tests - tests_with_output
    
    print(f"总测试数: {total_tests}")
    print(f"有输出 (检测到错误): {tests_with_output}")
    print(f"无输出 (语法正确): {tests_without_output}")
    
    # 按错误类型分类
    error_files = [r for r in results if r["has_output"]]
    print(f"\n有错误的文件 ({len(error_files)} 个):")
    for r in error_files:
        print(f"  {r['file']}")
    
    # 保存详细结果到文件
    with open("test_results.txt", "w") as f:
        f.write("测试结果详情\n")
        f.write("="*60 + "\n")
        for r in results:
            f.write(f"\n文件: {r['file']}\n")
            f.write(f"返回码: {r['return_code']}\n")
            if r['output']:
                f.write("输出:\n")
                f.write(r['output'])
                f.write("\n")
            else:
                f.write("输出: 无\n")
            if r['error']:
                f.write(f"标准错误: {r['error']}\n")
            f.write("-"*40 + "\n")
    
    print(f"\n详细结果已保存到 test_results.txt")
    
    return results

if __name__ == "__main__":
    run_tests()