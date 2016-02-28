#!/bin/bash

# this script should be executed from the build directory

# set path to test directory
Current_Dir=/${PWD#*/}
Test_Dir=${Current_Dir}/tests
Coverage_Dir=${Current_Dir}/coverage

if [ ${PWD##*/} != "build" ]; then
    echo The current running directory is not a 'build' directory
    exit
fi


if [ ! -d ${Test_Dir} ]; then
    echo "The directory ${Test_Dir} does not exist!"
    exit
fi

if [ ! -d ${Coverage_Dir} ]; then
    mkdir -p ${Coverage_Dir}
fi

lcov --zerocounters --directory ${Coverage_Dir}
lcov --capture --initial --directory . --output-file ${Coverage_Dir}/app

find ${Test_Dir} -type f -executable -exec {} \;

lcov --no-checksum --directory . --capture --output-file ${Coverage_Dir}/app.info
lcov --remove ${Coverage_Dir}/app.info 'tests/*' '/usr/*' --output-file ${Coverage_Dir}/app.info.cleaned
genhtml -o ${Coverage_Dir} ${Coverage_Dir}/app.info.cleaned
