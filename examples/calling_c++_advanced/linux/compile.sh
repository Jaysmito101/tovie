#!/bin/bash
g++ -lstdc++ -fPIC -shared -std=c++11 sort.cpp -o sort.so
read -p "Press any key to continue... " -n1 -s
