#!/usr/bin/env bash
source_dir=`pwd`

echo $source_dir

rm -rf build

mkdir build/ && cd build

cmake ..
make


