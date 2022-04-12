#!/bin/bash -e

find . ! \( -name '*.sh' -or -name '..'  -or -name '.' -or -name '.gitignore' \) | xargs rm -rf

cmake .. && make -j8
# make clean 
# make -j8