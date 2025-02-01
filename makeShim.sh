#!/bin/bash

source ~/env.sh
cd ~/OvertureShim/src
if [ "$1" == "clean" ]; then
    make clean
fi
make -j${nproc}
cd ..