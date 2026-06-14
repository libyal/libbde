#!/bin/sh
# Script to run tests
#
# Version: 20260609

if test -f ${PWD}/libbde/.libs/libbde.1.dylib && test -f ./pybde/.libs/pybde.so
then
	install_name_tool -change /usr/local/lib/libbde.1.dylib ${PWD}/libbde/.libs/libbde.1.dylib ./pybde/.libs/pybde.so
fi

make check-build > /dev/null

make check $@
RESULT=$?

if test ${RESULT} -ne 0
then
	find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

