#!/bin/bash

source ~/env.sh

rm -rf build
mkdir build

touch build/depend
touch build/depend_date

make -C src -j${nproc}
