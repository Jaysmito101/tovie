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
    <img alt="Maintenance" src="https://img.shields.io/maintenance/yes/2022?style=for-the-badge">
    <a href="https://patreon.com/jaysmito101"><img src="https://img.shields.io/endpoint.svg?url=https%3A%2F%2Fshieldsio-patreon.vercel.app%2Fapi%3Fusername%3Djaysmito101%26type%3Dpledges&style=for-the-badge" alt="Support me on Patreon" /></a>
</p>
<br/>


# Tovie Lang

An Advanced Programming Language (Compiler + Interpreter + Transpiler ).

<a href="https://github.com/Jaysmito101/dynamic-badges/"><img src="https://dynamic-badges.maxalpha.repl.co/star?user=Jaysmito101&repo=tovie&show=true"><img src="https://dynamic-badges.maxalpha.repl.co/star?image=true"></a>


# Docs : 
### For Help join our discord server : https://discord.gg/3h5S2cSbm8
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

    proc_main
	    "Hello World!"
	    puts
    proc_end

### 2. Factorial

    def<factorial 1>
    proc_factorial
	1 memset_0
	true while
		dup memget_0 * memset_0
		1 -
		dup > 0
	while_end
	memget_0
    proc_end
    
### 4. Using Arrays

    dec data:int[35]
    proc_main
	dec i:int
	0 >i
	1
	for_15
		dup >data[i]
		1 +
		<i 1 + >i
	for_end

	for_15
		<i 1 - >i
		<data[i] println
	for_end
    proc_end
 
 ### 5. Rule 110 (Proving Tovie is Turing Complete)
 
     dec board:int[32]
    def<COUNT 16>
    
    def<print_arr 2>
    proc_2
    	dec tmp:int
    	0 >tmp
    	32 for
    		<board[tmp]
    		if dup 1 == do
    			"#" puts
    		end_if
    		if 0 == do
    			"-" puts
    		end_if
    		<tmp 1 + >tmp
    	for_end
    proc_end
    
    def<load_arr 3>
    proc_3
    	0 >board[0]
    	0 >board[1]
    	0 >board[2]
    	0 >board[3]
    	0 >board[4]
    	1 >board[5]
    	0 >board[6]
    	1 >board[7]
    	0 >board[8]
    	0 >board[9]
    	0 >board[10]
    	0 >board[11]
    	0 >board[12]
    	0 >board[13]
    	0 >board[14]
    	0 >board[15]
    	0 >board[16]
    	0 >board[17]
    	0 >board[18]
    	0 >board[19]
    	0 >board[20]
    	1 >board[21]
    	0 >board[22]
    	1 >board[23]
    	0 >board[24]
    	0 >board[25]
    	0 >board[26]
    	0 >board[27]
    	0 >board[28]
    	0 >board[29]
    	0 >board[30]
    	0 >board[31]
    proc_end
    
    def<cycle_core 5>
    proc_5
    	dec t1:int
    	dec t2:int
    	dec t3:int
    	>t3 >t2 >t1
    	if <t1 0 == <t2 0 == && <t3 0 == && do
    		0 ret
    	end_if
    	if <t1 0 == <t2 0 == && <t3 1 == && do
    		1 ret
    	end_if
    	if <t1 0 == <t2 1 == && <t3 0 == && do
    		1 ret
    	end_if
    	if <t1 0 == <t2 1 == && <t3 1 == && do
    		1 ret
    	end_if
    	if <t1 1 == <t2 0 == && <t3 0 == && do
    		0 ret
    	end_if
    	if <t1 1 == <t2 0 == && <t3 1 == && do
    		1 ret
    	end_if
    	if <t1 1 == <t2 1 == && <t3 0 == && do
    		1 ret
    	end_if
    	if <t1 1 == <t2 1 == && <t3 1 == && do
    		0 ret
    	end_if
    proc_end
    
    
    def<do_cycle 4>
    proc_4
    	dec b0:int
    	dec b1:int
    	dec tmp2:int
    	dec tmp3:int
    	dec tmp4:int
    	dec tmp5:int
    	dec tmp:int
    	<board[0] >b0
    	<board[1] >b1
    	0 >tmp
    	32 for
    		<board[tmp]
    		if <tmp 30 < do
    			<board[tmp]
    			<tmp 1 + >tmp2
    			<board[tmp2]
    			<tmp 2 + >tmp2
    			<board[tmp2]
    			>tmp5
    			>tmp4
    			>tmp3
    		end_if
    		if <tmp 30 == do
    			<b0 >tmp5
    			<tmp 1 + >tmp2
    			<board[tmp2] >tmp4
    			<board[tmp] >tmp3
    		end_if
    		if <tmp 31 == do
    			<b0 >tmp5
    			<b1 >tmp4
    			<board[tmp] >tmp3
    		end_if
    		<tmp3 <tmp4 <tmp5 cycle_core call
    		>board[tmp]
    		<tmp 1 + >tmp
    	for_end
    proc_end
    
    proc_main
    	load_arr call
    	COUNT for
    		print_arr call
    		" " putsln
    		do_cycle call
    	for_end
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


<a href="https://dynamic-badges.maxalpha.repl.co/star?user=Jaysmito101&repo=tovie&id=1"><img src="https://dynamic-badges.maxalpha.repl.co/star?image=true"></a>
<a href="https://dynamic-badges.maxalpha.repl.co/star?user=Jaysmito101&repo=tovie&id=2"><img src="https://dynamic-badges.maxalpha.repl.co/star?image=true"></a>
<a href="https://dynamic-badges.maxalpha.repl.co/star?user=Jaysmito101&repo=tovie&id=3"><img src="https://dynamic-badges.maxalpha.repl.co/star?image=true"></a>
<a href="https://dynamic-badges.maxalpha.repl.co/star?user=Jaysmito101&repo=tovie&id=4"><img src="https://dynamic-badges.maxalpha.repl.co/star?image=true"></a>
<a href="https://dynamic-badges.maxalpha.repl.co/star?user=Jaysmito101&repo=tovie&id=5"><img src="https://dynamic-badges.maxalpha.repl.co/star?image=true"></a>


<img alt="Visitors" src="https://visitor-badge.glitch.me/badge?page_id=Jaysmito101.tovie&left_color=gray&right_color=green&style=for-the-badge">
