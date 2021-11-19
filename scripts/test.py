import os
import sys
import subprocess

def is_windows():
    return sys.platform == 'win32'

def get_absolute_path(relative_path):
    return os.path.abspath(relative_path)

FILES = [
    os.path.join('examples', "helloworld.tovie"),
    os.path.join('examples', "helloworld2.tovie"),
    os.path.join('examples', "fibonacii.tovie"),
    os.path.join('examples', "fizzbuzz.tovie"),
    os.path.join('examples', "1to10.tovie")
]

def path_join(a, b):
    if is_windows():
        return a + "\\" + b
    else:
        return a + "/" + b

BIN_PATH =  path_join("bin", "tovie")

def test_op(op = "com"):
    global FILES
    for file in FILES:
        print()
        print(BIN_PATH + " " + op + " " + file)
        retcode = 0
        if is_windows():
            retcode = subprocess.call(BIN_PATH + " " + op + " " + file)
        else:
            retcode = subprocess.call([BIN_PATH, op, file])
        if retcode != 0:
            print("Test Failed")
            exit(-1)

def test():
    print("\n\n")
    print("Testing compiler ...")
    test_op("com")

    print("\n\n")
    print("Testing interpreter ...")
    test_op("sis")

    print("\n\n")
    print("Testing python transpiler ...")
    test_op("t2p")

    print("\n\n")
    print("Testing c transpiler ...")
    test_op("t2c")

if __name__ == "__main__":
    print("Starting tests ...")
    test()
    print("Testing done.")