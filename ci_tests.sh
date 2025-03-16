#!/bin/bash
# author Shanaka Prageeth
# details CI script

DEBIAN_FRONTEND=noninteractive
PROGRAM_NAME="$(basename $0)"
BASEDIR=$(dirname $(realpath "$0"))
set -e

function cleanup {
    echo "Cleaning up"
    rm -rf $BASEDIRbuild
}

function sast(){
    if ! command -v cpplint &> /dev/null; then
        pip3 install cpplint
    fi

    if ! command -v cppcheck &> /dev/null; then
        sudo apt-get install -y cppcheck
    fi
    echo "Running Static Analysis"
    cd $BASEDIR/
    cpplint --recursive $BASEDIR/src/ $BASEDIR/include/ $BASEDIR/tests/ $BASEDIR/examples/
    cppcheck --enable=all --suppress=missingInclude --suppress=missingIncludeSystem $BASEDIR/src/ $BASEDIR/include/ $BASEDIR/tests/ $BASEDIR/examples/
    cppcheck --addon=cpplint_config/misra.json $BASEDIR/src/ $BASEDIR/include/ $BASEDIR/tests/ $BASEDIR/examples/
}

function build_debug(){
    mkdir -p $BASEDIR/build
    cd $BASEDIR/build
    cmake -DCMAKE_BUILD_TYPE=Debug ../
    cmake --build .
}
function unit_tests(){
    build_debug
    cd $BASEDIR/build
    ./bin/tests/test_runner
}

sast
unit_tests
