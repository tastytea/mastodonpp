#!/bin/sh

project="$(realpath --relative-base=.. .)"
version="$(grep -Eo '[0-9]+.[0-9]+.[0-9]+$' CMakeLists.txt)"

if [[ -f Doxyfile ]]; then
    mkdir -p doc
    (doxygen -s -g - && cat Doxyfile &&
         echo "PROJECT_NAME = ${project}" &&
         echo "PROJECT_NUMBER = ${version}") | doxygen -
fi
