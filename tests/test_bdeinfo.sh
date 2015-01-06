#!/bin/bash
#
# bdeinfo tool testing script
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

list_contains()
{
	LIST=$1;
	SEARCH=$2;

	for LINE in $LIST;
	do
		if test $LINE = $SEARCH;
		then
			return ${EXIT_SUCCESS};
		fi
	done

	return ${EXIT_FAILURE};
}

test_info()
{ 
	DIRNAME=$1;
	INPUT_FILE=$2;
	OPTIONS=$3;
	BASENAME=`basename ${INPUT_FILE}`;
	RESULT=${EXIT_FAILURE};

	rm -rf tmp;
	mkdir tmp;

	# Note that options should not contain spaces otherwise the test_runner
	# will fail parsing the arguments.
	${TEST_RUNNER} ${BDEINFO} ${OPTIONS} ${INPUT_FILE} | sed '1,2d' > tmp/${BASENAME}.log;

	RESULT=$?;

	if test -f "input/.bdeinfo/${DIRNAME}/${BASENAME}.log.gz";
	then
		zdiff "input/.bdeinfo/${DIRNAME}/${BASENAME}.log.gz" "tmp/${BASENAME}.log";

		RESULT=$?;
	else
		mv "tmp/${BASENAME}.log" "input/.bdeinfo/${DIRNAME}";

		gzip "input/.bdeinfo/${DIRNAME}/${BASENAME}.log";
	fi

	rm -rf tmp;

	return ${RESULT};
}

test_info_password()
{ 
	DIRNAME=$1;
	INPUT_FILE=$2;
	BASENAME=`basename ${INPUT_FILE}`;
	RESULT=${EXIT_FAILURE};
	PASSWORDFILE="input/.bdeinfo/${DIRNAME}/${BASENAME}.password";

	if test -f "${PASSWORDFILE}";
	then
		PASSWORD=`cat "${PASSWORDFILE}" | head -n 1 | sed 's/[\r\n]*$//'`;

		if test_info "${DIRNAME}" "${INPUT_FILE}" "-p${PASSWORD}";
		then
			RESULT=${EXIT_SUCCESS};
		fi
	fi

	echo -n "Testing bdeinfo with password of input: ${INPUT_FILE} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

test_info_recovery_password()
{ 
	DIRNAME=$1;
	INPUT_FILE=$2;
	BASENAME=`basename ${INPUT_FILE}`;
	RESULT=${EXIT_FAILURE};
	PASSWORDFILE="input/.bdeinfo/${DIRNAME}/${BASENAME}.recovery_password";

	if test -f "${PASSWORDFILE}";
	then
		PASSWORD=`cat "${PASSWORDFILE}" | head -n 1 | sed 's/[\r\n]*$//'`;

		if test_info "${DIRNAME}" "${INPUT_FILE}" "-r${PASSWORD}";
		then
			RESULT=${EXIT_SUCCESS};
		fi
	fi

	echo -n "Testing bdeinfo with recovery password of input: ${INPUT_FILE} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

BDEINFO="../bdetools/bdeinfo";

if ! test -x ${BDEINFO};
then
	BDEINFO="../bdetools/bdeinfo.exe";
fi

if ! test -x ${BDEINFO};
then
	echo "Missing executable: ${BDEINFO}";

	exit ${EXIT_FAILURE};
fi

TEST_RUNNER="tests/test_runner.sh";

if ! test -x ${TEST_RUNNER};
then
	TEST_RUNNER="./test_runner.sh";
fi

if ! test -x ${TEST_RUNNER};
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

	EXIT_RESULT=${EXIT_IGNORE};
else
	IGNORELIST="";

	if ! test -d "input/.bdeinfo";
	then
		mkdir "input/.bdeinfo";
	fi
	if test -f "input/.bdeinfo/ignore";
	then
		IGNORELIST=`cat input/.bdeinfo/ignore | sed '/^#/d'`;
	fi
	for TESTDIR in input/*;
	do
		if test -d "${TESTDIR}";
		then
			DIRNAME=`basename ${TESTDIR}`;

			if ! list_contains "${IGNORELIST}" "${DIRNAME}";
			then
				if ! test -d "input/.bdeinfo/${DIRNAME}";
				then
					mkdir "input/.bdeinfo/${DIRNAME}";
				fi
				if test -f "input/.bdeinfo/${DIRNAME}/files";
				then
					TESTFILES=`cat input/.bdeinfo/${DIRNAME}/files | sed "s?^?${TESTDIR}/?"`;
				else
					TESTFILES=`ls ${TESTDIR}/*`;
				fi
				for TESTFILE in ${TESTFILES};
				do
					BASENAME=`basename ${TESTFILE}`;

					if test -f "input/.bdeinfo/${DIRNAME}/${BASENAME}.password";
					then
						if ! test_info_password "${DIRNAME}" "${TESTFILE}";
						then
							exit ${EXIT_FAILURE};
						fi
					fi
					if test -f "input/.bdeinfo/${DIRNAME}/${BASENAME}.recovery_password";
					then
						if ! test_info_recovery_password "${DIRNAME}" "${TESTFILE}";
						then
							exit ${EXIT_FAILURE};
						fi
					fi
				done
			fi
		fi
	done

	EXIT_RESULT=${EXIT_SUCCESS};
fi

IFS=${OLDIFS};

exit ${EXIT_RESULT};

