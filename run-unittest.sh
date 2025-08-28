#!/bin/bash

find ./$1 -name '*.gcda' -exec rm {} \;
for filename in ./$1/*_test; do
    $filename
done
