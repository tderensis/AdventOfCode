#!/bin/bash

mkdir -p build
cd build/
cmake ..
cmake --build . || exit 1
cd ../
