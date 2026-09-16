#!/bin/sh
# Script to run tests
#
# Version: 20260714

if [ -f "${PWD}/libbde/.libs/libbde.1.dylib" ] && [ -f ./pybde/.libs/pybde.so ]
then
    install_name_tool -change /usr/local/lib/libbde.1.dylib "${PWD}/libbde/.libs/libbde.1.dylib" ./pybde/.libs/pybde.so
fi

make check-build > /dev/null

# shellcheck disable=SC2068
make check $@
RESULT=$?

if [ ${RESULT} -ne 0 ]
then
    find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

