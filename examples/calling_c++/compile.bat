@echo off
g++ -fPIC -shared -std=c++11 test.cpp -o test.dll
pause