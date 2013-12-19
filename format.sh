#!/bin/bash
#!format all .c .h file in this dir and all subdir
for FILE in $(find $(pwd) -type f -name '*.c' -o -name '*.cpp' -o -name '*.h')
do
    ./tools/astyle -p -U $FILE
    rm -f $FILE.orig
done
