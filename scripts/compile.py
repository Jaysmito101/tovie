import sys
import os
import subprocess

TARGET_NAME = "tovie"
DEFINES = ["TOVIE"]

def get_files_in_dir(dir):
    files = []
    for file in os.listdir(dir):
        if file.endswith(".cpp") or file.endswith(".c"):
            files.append(os.path.join(dir, file))
        if(os.path.isdir(os.path.join(dir, file))):
            files.extend(get_files_in_dir(os.path.join(dir, file)))
    return files
    
def is_windows():
    return sys.platform == "win32"

def compile_files(files):
    global TARGET_NAME
    global DEFINES
    files_str = ""
    for file in files:
        files_str += (file) + " "
    def_str = ""
    for dfstr in DEFINES:
        def_str += " -D"  + dfstr + " "
    def_str = def_str[:-1]
    target_extension = ".exe" if is_windows() else ""
    extra = ""
    if(not is_windows()):
        extra = "-lstdc++ -Wl,--no-as-needed -ldl"
    print("g++ -std=c++11 " + extra + " -o ./bin/" + TARGET_NAME  + target_extension +  " " + files_str)
    retcode = os.system("g++ -I/include -std=c++11 " + extra + " -o ./bin/" + TARGET_NAME  + target_extension +  " " + files_str)
    if(retcode != 0):
        print("Compilation failed")
        exit(-1)
    

def main(args):
    global DEFINES
    DEFINES.extend(args)
    if(not os.path.isdir("./bin")):
        os.mkdir("./bin")
    compile_files(get_files_in_dir("./src"))
    print("Done!")

if __name__ == "__main__":
    main(sys.argv[1:])
