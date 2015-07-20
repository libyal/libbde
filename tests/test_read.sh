#!/bin/bash
#
# Library read testing script
#
# Copyright (C) 2011-2015, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This software is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this software.  If not, see <http://www.gnu.org/licenses/>.
#

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

TEST_PREFIX="bde";
OPTION_SETS="password recovery_password";

list_contains()
{
	LIST=$1;
	SEARCH=$2;

	for LINE in ${LIST};
	do
		if test ${LINE} = ${SEARCH};
		then
			return ${EXIT_SUCCESS};
		fi
	done

	return ${EXIT_FAILURE};
}

test_read()
{ 
	TEST_SET=$1;
	INPUT_FILE=$2;
	OPTION_SET=$3;

	BASENAME=`basename ${INPUT_FILE}`;

	if test -z "${OPTION_SET}";
	then
		OPTIONS="";
	else
		OPTIONS=`cat "${TEST_SET}/${BASENAME}.${OPTION_SET}" | head -n 1 | sed 's/[\r\n]*$//'`;
	fi
	TMPDIR="tmp$$";

	rm -rf ${TMPDIR};
	mkdir ${TMPDIR};

	${TEST_RUNNER} ${TMPDIR} ${TEST_READ} ${OPTIONS} ${INPUT_FILE};

	RESULT=$?;

	rm -rf ${TMPDIR};

	return ${RESULT};
}

TEST_READ="./${TEST_PREFIX}_test_read";

if ! test -x "${TEST_READ}";
then
	TEST_READ="./${TEST_PREFIX}_test_read.exe";
fi

if ! test -x "${TEST_READ}";
then
	echo "Missing executable: ${TEST_READ}";

	exit ${EXIT_FAILURE};
fi

TEST_RUNNER="tests/test_runner.sh";

if ! test -x "${TEST_RUNNER}";
then
	TEST_RUNNER="./test_runner.sh";
fi

if ! test -x "${TEST_RUNNER}";
then
	echo "Missing test runner: ${TEST_RUNNER}";

	exit ${EXIT_FAILURE};
fi

if ! test -d "input";
then
	echo "No input directory found.";

	exit ${EXIT_IGNORE};
fi

OLDIFS=${IFS};
IFS="
";

RESULT=`ls input/* | tr ' ' '\n' | wc -l`;

if test ${RESULT} -eq 0;
then
	echo "No files or directories found in the input directory.";

	IFS=${OLDIFS};

	exit ${EXIT_IGNORE};
fi

TEST_PROFILE="input/.lib${TEST_PREFIX}";

if ! test -d "${TEST_PROFILE}";
then
	mkdir ${TEST_PROFILE};
fi

IGNORE_FILE="${TEST_PROFILE}/ignore";
IGNORE_LIST="";

if test -f "${IGNORE_FILE}";
then
	IGNORE_LIST=`cat ${IGNORE_FILE} | sed '/^#/d'`;
fi

for TESTDIR in input/*;
do
	if ! test -d "${TESTDIR}";
	then
		continue
	fi
	DIRNAME=`basename ${TESTDIR}`;

	if list_contains "${IGNORE_LIST}" "${DIRNAME}";
	then
		continue
	fi
	TEST_SET="${TEST_PROFILE}/${DIRNAME}";

	if ! test -d "${TEST_SET}";
	then
		mkdir "${TEST_SET}";
	fi

	if test -f "${TEST_SET}/files";
	then
		TEST_FILES=`cat ${TEST_SET}/files | sed "s?^?${TESTDIR}/?"`;
	else
		TEST_FILES=`ls ${TESTDIR}/*`;
	fi

	for TEST_FILE in ${TEST_FILES};
	do
		BASENAME=`basename ${TEST_FILE}`;

		for OPTION_SET in `echo ${OPTION_SETS} | tr ' ' '\n'`;
		do
			OPTION_FILE="${TEST_SET}/${BASENAME}.${OPTION_SET}";

			if ! test -f "${OPTION_FILE}";
			then
				continue
			fi
			echo "Testing read with option: ${OPTION_SET} and input: ${INPUT_FILE}";

			if test_read "${TEST_SET}" "${TEST_FILE}" "${OPTION_SET}";
			then
				RESULT=${EXIT_SUCCESS};
			else
				RESULT=${EXIT_FAILURE};
			fi
			echo "";

			if test ${RESULT} -ne ${EXIT_SUCCESS};
			then
				IFS=${OLDIFS};

				exit ${EXIT_FAILURE};
			fi
		done

		if test -z "${OPTION_SETS}";
		then
			echo "Testing read with input: ${INPUT_FILE}";

			if test_read "${TEST_SET}" "${TEST_FILE}" "";
			then
				RESULT=${EXIT_SUCCESS};
			else
				RESULT=${EXIT_FAILURE};
			fi
			echo "";

			if test ${RESULT} -ne ${EXIT_SUCCESS};
			then
				IFS=${OLDIFS};

				exit ${EXIT_FAILURE};
			fi
		fi
	done
done

IFS=${OLDIFS};

exit ${EXIT_SUCCESS};

