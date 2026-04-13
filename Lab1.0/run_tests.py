#!/usr/bin/env python3
import os
import subprocess
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__)))
PARSER = os.path.join(ROOT, 'Code', 'parser')
INPUT_DIR = os.path.join(ROOT, 'Tests', 'inputs')
EXPECT_DIR = os.path.join(ROOT, 'Tests', 'expects')
RESULT_FILE = os.path.join(ROOT, 'test_results.txt')


def normalize_lines_for_compare(lines):
    """针对题主要求：如果某行包含冒号，仅比较冒号之前的部分；否则比较整行。
    同时去掉两端空白。返回处理后的行列表。"""
    out = []
    for l in lines:
        s = l.rstrip('\n').strip()
        if ':' in s:
            left = s.split(':', 1)[0].strip()
            out.append(left)
        else:
            out.append(s)
    return out


def run_parser_on_file(parser, input_path):
    # parser expects the input filename as argument (see Makefile test target)
    if not os.path.isfile(parser) or not os.access(parser, os.X_OK):
        return False, '', f'ERROR: parser executable not found or not executable: {parser}'

    try:
        p = subprocess.run([parser, input_path], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=10)
        out = p.stdout.decode('utf-8', errors='replace')
        return True, out, None
    except subprocess.TimeoutExpired:
        return False, '', 'ERROR: parser timed out'
    except Exception as e:
        return False, '', f'ERROR: running parser failed: {e}'


def load_expect(expect_path):
    try:
        with open(expect_path, 'r', encoding='utf-8') as f:
            return f.read()
    except Exception as e:
        return None


def main():
    inputs = sorted([f for f in os.listdir(INPUT_DIR) if f.endswith('.cmm')])
    if not inputs:
        print('No input files found in', INPUT_DIR)
        return 2

    results = []
    out_lines = []
    out_lines.append('Test run summary')
    out_lines.append('=================')

    for inp in inputs:
        inp_path = os.path.join(INPUT_DIR, inp)
        base = os.path.splitext(inp)[0]
        expect_name = base + '.exp'
        expect_path = os.path.join(EXPECT_DIR, expect_name)

        ok, out, err = run_parser_on_file(PARSER, inp_path)
        if not ok:
            results.append((inp, 'ERROR', err))
            out_lines.append(f'{inp}: ERROR running parser: {err}')
            continue

        expect_text = load_expect(expect_path)
        if expect_text is None:
            results.append((inp, 'ERROR', f'Expect file missing: {expect_name}'))
            out_lines.append(f'{inp}: ERROR expect file missing: {expect_name}')
            continue

        # 比较：按行比较，但规则为“错误输出只要对比冒号前的即可”
        actual_lines = out.splitlines()
        expect_lines = expect_text.splitlines()

        actual_norm = normalize_lines_for_compare(actual_lines)
        expect_norm = normalize_lines_for_compare(expect_lines)

        # 比较两者（简单按行比较，长度也必须相同）
        passed = (len(actual_norm) == len(expect_norm)) and all(a == b for a, b in zip(actual_norm, expect_norm))

        if passed:
            results.append((inp, 'PASS', None))
            out_lines.append(f'{inp}: PASS')
        else:
            # 生成简要差异（只展示前几处不同）
            diffs = []
            max_lines = max(len(actual_norm), len(expect_norm))
            for i in range(max_lines):
                a = actual_norm[i] if i < len(actual_norm) else '<no line>'
                b = expect_norm[i] if i < len(expect_norm) else '<no line>'
                if a != b:
                    diffs.append((i+1, a, b))
                if len(diffs) >= 8:
                    break

            results.append((inp, 'FAIL', diffs))
            out_lines.append(f'{inp}: FAIL')
            out_lines.append('  Differences (line#, actual, expected)')
            for d in diffs:
                out_lines.append(f'    {d[0]}: {d[1]!r}  !=  {d[2]!r}')

        # Optional: write the raw actual output to a *.out file next to results
        out_file = os.path.join(INPUT_DIR, base + '.out')
        try:
            with open(out_file, 'w', encoding='utf-8') as of:
                of.write(out)
        except Exception:
            pass

    # 汇总统计
    total = len(results)
    passed = sum(1 for r in results if r[1] == 'PASS')
    failed = sum(1 for r in results if r[1] == 'FAIL')
    errored = sum(1 for r in results if r[1] == 'ERROR')

    out_lines.append('')
    out_lines.append(f'Total: {total}, Passed: {passed}, Failed: {failed}, Error: {errored}')

    try:
        with open(RESULT_FILE, 'w', encoding='utf-8') as rf:
            rf.write('\n'.join(out_lines))
    except Exception as e:
        print('Failed to write result file:', e)

    # 打印简洁的终端输出
    for line in out_lines:
        print(line)

    # 非零退出码当有失败或错误
    if failed > 0 or errored > 0:
        return 1
    return 0


if __name__ == '__main__':
    sys.exit(main())
