#!/bin/bash
for i in src/*.c include/*.h
do
    clang-format -style=file $i >$$
    mv $$ $i
done
