#!/bin/bash
g++ -lstdc++ -fPIC -shared -std=c++11 test.cpp -o test.so
read -p "Press any key to continue... " -n1 -s
