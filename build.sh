#!/bin/bash

mkdir -p build
cd build/
cmake ..
cmake --build . --clean-first || exit 1
cd ../
