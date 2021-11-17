#!/bin/bash
shift
python scripts/compile.py TOVIE_LINUX $@
read -p "Press any key to continue... " -n1 -s
clear
