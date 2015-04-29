#!/bin/bash
for i in src/*.c include/*.h
do
    if [[ "$i" != "src/data.c" ]]
    then
        clang-format -style=file "$i" >$$
        mv $$ "$i"
    fi
done
