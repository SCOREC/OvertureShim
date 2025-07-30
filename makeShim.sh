#!/bin/bash

source ~/env.sh

if [ "$1" == "clean" ]; then
    make -C src clean
fi

make -C src -j${nproc}

