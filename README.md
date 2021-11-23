<br/>
<p align="center">
    <img src="https://github.com/Jaysmito101/tovie/blob/master/recources/ReadmeHeader.png?raw=true" border="0"></
</p>

<br/>
<p align="center">
  <img alt="GitHub repo size" src="https://img.shields.io/github/repo-size/Jaysmito101/tovie?style=for-the-badge">
  <img alt="Lines of code" src="https://img.shields.io/tokei/lines/github/Jaysmito101/tovie?style=for-the-badge">
  <img alt="GitHub commit activity" src="https://img.shields.io/github/commit-activity/w/Jaysmito101/tovie?style=for-the-badge">
    <br>
    <img alt="GitHub Workflow Status" src="https://img.shields.io/github/workflow/status/Jaysmito101/tovie/Tovie%20Build?style=for-the-badge">
    <img alt="Maintenance" src="https://img.shields.io/maintenance/yes/2021?style=for-the-badge">
    <a href="https://patreon.com/jaysmito101"><img src="https://img.shields.io/endpoint.svg?url=https%3A%2F%2Fshieldsio-patreon.vercel.app%2Fapi%3Fusername%3Djaysmito101%26type%3Dpledges&style=for-the-badge" alt="Support me on Patreon" /></a>
</p>
<br/>


# Tovie Lang

An Advanced Programming Language (Compiler + Interpreter + Transpiler ).

# Docs : 
### https://github.com/Jaysmito101/tovie/wiki

# What can it do?

* All Arithmetic operations
* Dynamic Memory Allocation
* Call Native Functions By Loading DLL or SO
* String management
* Library System
* While, Do, For loops
* If statements
* User Input, ...

# Todo

* A standard library in tovie
* A standard library implemented in c
* Add more examples

## Usage

    Usage: C:\Users\Jaysmito Mukherjee\Desktop\tovie\bin\tovie.exe [options] <input>
    
    Options:
      -h, --help ............ prints this message and exits
      -v, --version ......... prints tovie's version and exits
      -r, --run ............. runs <input> instead of compiling (`-f` will specify the backend; will also compile if `-o` is given)
      -d, --debug ........... prints useful debug information
      -o, --output <path> ... specifies the output-file
      -f, --format <format> . specifies the output-format (bytecode,table,c,python; default: bytecode)
      -i, --include <path> .. specifies include directories (can be used multiple times)
      -I, --input <format> .. specifies the input-format (source,bytecode; default: extension)



## Examples :

### 1. Hello World

    proc_0
	    "Hello" 32 "World!"
	    12
	    puts
    proc_end

### 2. English Like Hello World

    Lets start with proc_0 as the main function. Then i push 2 into the stack to call procedure 2.
    Lastly I end this proc by end_proc .
    As far as proc_2 is concerned, first i push the string "Hello!" of size 6 in the stack. 
    Then just puts in on screen! Lastly proc_end .

### 3. Fibonacii Sequence

    proc_0
        1 call
        0 1
        inputi for
            print
            +
            32 prints pop // PRINTING THE SPACE
        for_end
    proc_end

    proc_1
        "Enter" 32 "N" 32 ":" 32
        10 puts
    proc_end

### 4. Even/Odd Checker

    proc_0
    	"Enter" 32 "a" 32 "Number" 32 ":" 32
    	17
    	puts
    	inputi
    	2 % 0 == if
    		"EVEN"
    		4 puts
    		6 proceed
    	end_if
    	"ODD"
    	3 puts
    proc_end

### 5. Print 1 to N For Loop

    proc_0
        "Enter" 32 "N:" 8 puts
        0
        inputi for
            1 +
            println
        for_end
    proc_end

### 6. Print 1 to N While Loop

    proc_0
    	1 call
    	2 call
    proc_end
    
    
    // PROC FOR TAKING INPUT
    
    proc_1
    	"Enter" 32 "N:" 32
    	9 puts
    	inputi
    proc_end
    
    // MAIN PRINTING PROC
    
    proc_2
    	memset_0
    	0 0
    	true
    	while
    		pop
    		1 +
    		println
    		memget_0 -
    	end_while
    proc_end

# How to install and use tovie?

Requirements:
- git (optional)
- make
- g++

You have to download this repository, you go to *tags* and download the latest version or use `git clone`:
```
$ git clone https://github.com/Jaysmito101/tovie
$ cd tovie
```

To build **tovie** just run `make`:
```
$ make
```

Your built `tovie` executable (or `tovie.exe` on windows) is in `bin`, run `tovie -h` to print a help message.

> *Have fun using Tovie!*

# Support

Any support would be highly appretiated!

For example you could add a feature and contribute via pull requests or you could even report any issues with the program!

And the best thing you could do to support this project is spread word about this so that more people who might be interested in this may use this!

Please considering tweeting about this! 


<img alt="Visitors" src="https://visitor-badge.glitch.me/badge?page_id=Jaysmito101.tovie&left_color=gray&right_color=green&style=for-the-badge">
