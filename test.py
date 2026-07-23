import subprocess
import os

BINARY_PATH = os.path.join(
    os.path.dirname(__file__), "cmake-build-debug",
    "interpreter_anna_mohil.exe" if os.name == "nt" else "interpreter_anna_mohil"
)

def run_interpreter(input_lines):
    process = subprocess.Popen(
        [BINARY_PATH],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    input_data = "\n".join(input_lines) + "\n"
    stdout, stderr = process.communicate(input_data)

    stdout_lines = [line for line in stdout.strip().split("\n") if line != ""]
    return stdout_lines, stderr.strip()

TEST_CASES = [
    {
        "name": "Basic arithmetic with operator precedence",
        "input": ["5 + 3 * 2"],
        "expected": ["11"],
    },
    {
        "name": "Built-in max",
        "input": ["max(5, 2)"],
        "expected": ["5"],
    },
    {
        "name": "Built-in min",
        "input": ["min(3, 4)"],
        "expected": ["3"],
    },
    {
        "name": "Built-in pow",
        "input": ["pow(2, 4)"],
        "expected": ["16"],
    },
    {
        "name": "Built-in abs",
        "input": ["abs(-7)"],
        "expected": ["7"],
    },
    {
        "name": "Nested / combined expressions",
        "input": ["max(min(3 * 2, 2), 2)"],
        "expected": ["2"],
    },
    {
        "name": "Variables: declare and reuse",
        "input": ["var a = max(min(3 * 2, 2), 2)", "a + 3"],
        "expected": ["5"],
    },
    {
        "name": "User-defined function",
        "input": ["def myfunc(a, b) { min(a, b) + max(a, b) }", "myfunc(3, 4)"],
        "expected": ["7"],
    },
    {
        "name": "Higher-order integral() with trapezoidal rule",
        "input": ["def line(x) { 2 * x }", "integral(line, 0, 3)"],
        "expected": ["9"],
    },
    {
        "name": "Higher-order sum() warm-up",
        "input": ["def sq(x) { x * x }", "sum(sq, 1, 3)"],
        "expected": ["14"],
    },
    {
        "name": "First-class functions",
        "input": ["def square(x) { x * x }", "var g = square", "g(4)"],
        "expected": ["16"],
    },
]


def run_all_tests():
    passed = 0
    failed = 0

    for case in TEST_CASES:
        output_lines, stderr = run_interpreter(case["input"])

        if stderr:
            print(f"Test failed for input:\n{case['input']}")
            print(f"Error:\n{stderr}")
            failed += 1
            continue

        if output_lines != case["expected"]:
            print(f"Test failed for input:\n{case['input']}")
            print(f"Expected:\n{case['expected']}")
            print(f"Got:\n{output_lines}")
            failed += 1
        else:
            print(f"Test passed for input:\n{case['input']}")
            passed += 1

    print()
    print(f"Results: {passed} passed, {failed} failed, {len(TEST_CASES)} total")
    return failed == 0


if __name__ == "__main__":
    success = run_all_tests()
    exit(0 if success else 1)