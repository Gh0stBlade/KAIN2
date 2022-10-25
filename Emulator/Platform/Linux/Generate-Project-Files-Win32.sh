#!/bin/sh
dir=${PWD%/*}
dir=${dir%/*}
dir=${dir%/*}
cd ${dir}
cmake -DCMAKE_BUILD_TYPE=Release . -B${dir}/Build_Linux/
cd ${dir}/Build_Linux/
cmake --build .