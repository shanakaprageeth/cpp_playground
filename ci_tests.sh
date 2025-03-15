#!/bin/bash
# author Shanaka Prageeth
# details CI script

DEBIAN_FRONTEND=noninteractive
PROGRAM_NAME="$(basename $0)"
BASEDIR=$(dirname $(realpath "$0"))
set -e

if ! command -v cpplint &> /dev/null; then
    pip3 install cpplint
fi

if ! command -v cppcheck &> /dev/null; then
    sudo apt-get install -y cppcheck
fi

cd $BASEDIR
cpplint --recursive src/ include/ tests/examples/
cppcheck --enable=all --suppress=missingInclude --suppress=missingIncludeSystem src/ include/ tests/ examples/
cppcheck --addon=cpplint_config/misra.json  src include examples

cd $BASEDIR
mkdir -p $BASEDIR/build
cd $BASEDIR/build
cmake ../
cmake --build .
cd $BASEDIR/build
./bin/tests/test_runner
