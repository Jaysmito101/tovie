#include "info.hpp"
#include "Operations.hpp"
#include "compiler.hpp"
#include "python_transpiler.hpp"
#include "c_transpiler.hpp"
#include "runtimelib.hpp"
#include "op_table.hpp"
#include "vm.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string.h>
#include <string>
#include <vector>


std::string read_file(std::string filePath) {
	std::ifstream	  file(filePath);
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

void write_file(std::string data, std::string filePath) {
	std::ofstream out(filePath);
	out << data;
	out.close();
}

void print_usage(std::string arg0 = "tovie") {
	std::cout << "Usage: " << arg0 << " [options] <input>" << std::endl
			  << std::endl
			  << "Options:" << std::endl
			  << "  -h, --help ............ prints this message and exits" << std::endl
			  << "  -v, --version ......... prints tovie's version and exits" << std::endl
			  << "  -r, --run ............. compile and run <input> (`-f` will specify the backend; will also compile if `-o` is given)" << std::endl
			  << "  -d, --debug ........... enable debug mode" << std::endl
			  << "  -o, --output <path> ... specifies the output-file" << std::endl
			  << "  -f, --format <format> . specifies the output-format (bytecode,table,c,python; default: bytecode)" << std::endl
			  << "  -i, --include <path> .. specifies include directories (can be used multiple times)" << std::endl
			  << "  -I, --input <format> .. specifies the input-format (source,bytecode; default: extension)" << std::endl;
}

static void save(std::vector<tovie::Operation> ops, std::string filePath) {
	std::ofstream fout(filePath, std::ios::out | std::ios::binary);
	fout.write((char*) &ops[0], ops.size() * sizeof(tovie::Operation));
	fout.close();
}

static std::vector<tovie::Operation> load(std::string filePath) {
	std::ifstream	file(filePath, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	std::vector<char>	   buffer(size);
	std::vector<tovie::Operation> ops;
	if (file.read(buffer.data(), size)) {
		int		   count = size / sizeof(tovie::Operation);
		tovie::Operation* opsp	 = (tovie::Operation*) buffer.data();
		for (int i = 0; i < count; i++) {
			ops.push_back(*opsp);
			opsp++;
		}
	}
	return ops;
}

typedef enum file_format {
	FORMAT_DEFAULT = 0,
	FORMAT_SOURCE,
	FORMAT_BYTECODE,
	FORMAT_TABLE,
	FORMAT_C,
	FORMAT_PYTHON
} file_format_t;

static const char* format_to_string(file_format format) {
	switch (format) {
		case FORMAT_SOURCE:
			return "source";
		case FORMAT_BYTECODE:
			return "bytecode";
		case FORMAT_TABLE:
			return "table";
		case FORMAT_C:
			return "c";
		case FORMAT_PYTHON:
			return "python";
		default:
			break;
	}
	return "";
}

int main(int argc, const char** argv) {
	std::string	  input_file;
	file_format_t input_format = FORMAT_DEFAULT;

	std::string	  output_file;
	file_format_t output_format = FORMAT_DEFAULT;

	bool run   = false;
	bool debug = false;

	std::vector<std::string> include_paths{ "." };

	for (int i = 1; i < argc; i++) {
		std::string arg = argv[i];
		if (arg[0] != '-') {
			if (input_file != "") {
				std::cout << "error: input-file already specified" << std::endl;
				print_usage(argv[0]);
				return 1;
			}
			input_file = arg;
		} else if (arg == "-h" || arg == "--help") {
			print_usage(argv[0]);
			return 0;
		} else if (arg == "-v" || arg == "--version") {
			std::cout << tovie::get_name() << std::endl;
			return 0;
		} else if (arg == "-r" || arg == "--run") {
			run = true;
		} else if (arg == "-d" || arg == "--debug") {
			debug = true;
		} else if (arg == "-o" || arg == "--output") {
			if (i + 1 == argc) {	// last argument
				std::cout << "error: <path> omitted" << std::endl;
				print_usage(argv[0]);
				return 1;
			}
			if (output_file != "") {
				std::cout << "error: output-file already specified" << std::endl;
				print_usage(argv[0]);
				return 1;
			}
			output_file = argv[++i];
		} else if (arg == "-i" || arg == "--include") {
			if (i + 1 == argc) {	// last argument
				std::cout << "error: <path> omitted" << std::endl;
				print_usage(argv[0]);
				return 1;
			}
			include_paths.push_back(argv[++i]);
		} else if (arg == "-f" || arg == "--format") {
			if (i + 1 == argc) {	// last argument
				std::cout << "error: <format> omitted" << std::endl;
				print_usage(argv[0]);
				return 1;
			}
			if (output_format != FORMAT_DEFAULT) {
				std::cout << "error: output-format already specified" << std::endl;
				print_usage(argv[0]);
				return 1;
			}
			std::string format = argv[++i];
			if (format == "bytecode") {
				output_format = FORMAT_BYTECODE;
			} else if (format == "table") {
				output_format = FORMAT_TABLE;
			} else if (format == "c") {
				output_format = FORMAT_C;
			} else if (format == "python") {
				output_format = FORMAT_PYTHON;
			} else {
				std::cout << "error: unknown format: " << format << std::endl;
				print_usage(argv[0]);
				return 1;
			}
		} else if (arg == "-I" || arg == "--input") {
			if (i + 1 == argc) {	// last argument
				std::cout << "error: <format> omitted" << std::endl;
				print_usage(argv[0]);
				return 1;
			}
			if (input_format != FORMAT_DEFAULT) {
				std::cout << "error: input-format already specified" << std::endl;
				print_usage(argv[0]);
				return 1;
			}
			std::string format = argv[++i];
			if (format == "bytecode") {
				output_format = FORMAT_BYTECODE;
			} else if (format == "source") {
				output_format = FORMAT_SOURCE;
			} else {
				std::cout << "error: unknown format: " << format << std::endl;
				print_usage(argv[0]);
				return 1;
			}
		} else {
			std::cout << "error: unknown option: " << arg << std::endl;
			print_usage(argv[0]);
			return 1;
		}
	}

	if (input_file == "") {
		std::cout << "error: input-file omitted" << std::endl;
		print_usage(argv[0]);
		return 1;
	}

	if (input_format == FORMAT_DEFAULT)
		input_format = input_file.substr(input_file.length() - 6) == ".tovie" ? FORMAT_SOURCE : FORMAT_BYTECODE;

	if (output_format == FORMAT_DEFAULT)
		output_format = FORMAT_BYTECODE;

	std::vector<tovie::Operation> input_ops;
	if (input_format == FORMAT_SOURCE) {
		input_ops = tovie::parse(read_file(input_file), include_paths, debug);
	} else if (input_format == FORMAT_BYTECODE) {
		input_ops = load(input_file);
	}

	if (run) {
		tovie::simulate(input_ops, debug);
		return 0;
	}

	switch (output_format) {
		case FORMAT_BYTECODE:
			if (output_file == "")
				output_file = "a.toviec";
			save(input_ops, output_file);
			break;
		case FORMAT_TABLE:
			if (output_file == "")
				output_file = "a.tovieopt";
			write_file(tovie::generate_op_table(input_ops), output_file);
			break;
		case FORMAT_C:
			if (output_file == "")
				output_file = "a.tovie.c";
			write_file(tovie::tovie2c(input_ops), output_file);
			break;
		case FORMAT_PYTHON:
			if (output_file == "")
				output_file = "a.tovie.py";
			write_file(tovie::tovie2py(input_ops), output_file);
			break;
		default:
			break;
	}

	std::cout << "compiled '" << input_file << "'(" << format_to_string(input_format) << ") -> '" << output_file << "'(" << format_to_string(output_format) << ")" << std::endl;
}

