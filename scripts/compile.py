import sys
import os

TARGET_NAME = "tovie"

def get_files_in_dir(dir):
    files = []
    for file in os.listdir(dir):
        if file.endswith(".cpp") or file.endswith(".c"):
            files.append(file)
        if(os.path.isdir(file)):
            files.extend(get_files_in_dir(file))
    return files

def compile_file(file):
    print("Compiling " + file + "...")
    os.system("g++ -c -std=c++11 -o ./bin/o/" + file.split(".")[0] + ".o ./src/" + file)
    

def compile_files(files):
    global TARGET_NAME
    files_str = ""
    for file in files:
        files_str += ("src/" + file) + " "
    os.system("g++ -std=c++11 -o ./bin/" + TARGET_NAME + ".exe " + files_str)
    

def main(args):
    if len(args) == 0:
        if(not os.path.isdir("./bin")):
            os.mkdir("./bin")
        compile_files(get_files_in_dir("./src"))
        print("Done!")

if __name__ == "__main__":
    main(sys.argv[1:])