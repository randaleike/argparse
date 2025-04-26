#!/bin/bash

for filename in ./$1/*_test; do
    $filename
done
