#!/bin/sh
# Script to run tests
#
# Version: 20260602

if test -f ${PWD}/libbde/.libs/libbde.1.dylib && test -f ./pybde/.libs/pybde.so
then
	install_name_tool -change /usr/local/lib/libbde.1.dylib ${PWD}/libbde/.libs/libbde.1.dylib ./pybde/.libs/pybde.so
fi

make check $@
RESULT=$?

if test ${RESULT} -ne 0 && test -f tests/test-suite.log
then
	cat tests/test-suite.log
fi
exit ${RESULT}

