@echo off
g++ -fPIC -shared -std=c++11 sort.cpp -o sort.dll
pause