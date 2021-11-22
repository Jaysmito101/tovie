import os
import sys
import subprocess

def is_windows():
    return sys.platform == 'win32'

FILES = [
    os.path.join('examples', "helloworld.tovie"),
    os.path.join('examples', "helloworld2.tovie"),
    os.path.join('examples', "fibonacii.tovie"),
    os.path.join('examples', "fizzbuzz.tovie"),
    os.path.join('examples', "1to10.tovie")
]

BIN_PATH =  os.path.join("bin", "tovie")

def test_op(op = "com"):
    global FILES
    for file in FILES:
        print()
        print(BIN_PATH + " " + op + " " + file)
        retcode = 0
        if is_windows():
            retcode = subprocess.call(BIN_PATH + ".exe " + op + " " + file)
        else:
            retcode = subprocess.call(["./" + BIN_PATH, *op.split(" "), file])
        if retcode != 0:
            print("Test Failed")
            exit(-1)

def test():
    print("\n\n")
    print("Testing compiler ...")
    test_op("")

    print("\n\n")
    print("Testing interpreter ...")
    test_op("-r")

    print("\n\n")
    print("Testing python transpiler ...")
    test_op("-f python")

    print("\n\n")
    print("Testing c transpiler ...")
    test_op("-f c")

if __name__ == "__main__":
    print("Starting tests ...")
    test()
    print("Testing done.")
