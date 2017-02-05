#!/bin/bash
# Tests C library functions and types.
#
# Version: 20170115

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

TEST_PREFIX=`dirname ${PWD}`;
TEST_PREFIX=`basename ${TEST_PREFIX} | sed 's/^lib\([^-]*\).*$/\1/'`;

TEST_PROFILE="lib${TEST_PREFIX}";
LIBRARY_TESTS="aes_ccm_encrypted_key error io_handle key key_protector metadata metadata_entry notify sector_data stretch_key volume_master_key";
LIBRARY_TESTS_WITH_INPUT="support volume";
OPTION_SETS="password recovery_password";

TEST_TOOL_DIRECTORY=".";
INPUT_DIRECTORY="input";
INPUT_GLOB="*";

run_test()
{
	local TEST_NAME=$1;

	local TEST_DESCRIPTION="Testing: ${TEST_NAME}";
	local TEST_EXECUTABLE="${TEST_TOOL_DIRECTORY}/${TEST_PREFIX}_test_${TEST_NAME}";

	if ! test -x "${TEST_EXECUTABLE}";
	then
		TEST_EXECUTABLE="${TEST_EXECUTABLE}.exe";
	fi

	# TODO: add support for TEST_PROFILE and OPTION_SETS?
	run_test_with_arguments "${TEST_DESCRIPTION}" "${TEST_EXECUTABLE}";
	local RESULT=$?;

	return ${RESULT};
}

run_test_with_input()
{
	local TEST_NAME=$1;

	local TEST_DESCRIPTION="Testing: ${TEST_NAME}";
	local TEST_EXECUTABLE="${TEST_TOOL_DIRECTORY}/${TEST_PREFIX}_test_${TEST_NAME}";

	if ! test -x "${TEST_EXECUTABLE}";
	then
		TEST_EXECUTABLE="${TEST_EXECUTABLE}.exe";
	fi

	run_test_on_input_directory "${TEST_PROFILE}" "${TEST_DESCRIPTION}" "default" "${OPTION_SETS}" "${TEST_EXECUTABLE}" "${INPUT_DIRECTORY}" "${INPUT_GLOB}";
	local RESULT=$?;

	return ${RESULT};
}

if ! test -z ${SKIP_LIBRARY_TESTS};
then
	exit ${EXIT_IGNORE};
fi

TEST_RUNNER="tests/test_runner.sh";

if ! test -f "${TEST_RUNNER}";
then
	TEST_RUNNER="./test_runner.sh";
fi

if ! test -f "${TEST_RUNNER}";
then
	echo "Missing test runner: ${TEST_RUNNER}";

	exit ${EXIT_FAILURE};
fi

source ${TEST_RUNNER};

RESULT=${EXIT_IGNORE};

for TEST_NAME in ${LIBRARY_TESTS};
do
	run_test "${TEST_NAME}";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		break;
	fi
done

if test ${RESULT} -ne ${EXIT_SUCCESS} && test ${RESULT} -ne ${EXIT_IGNORE};
then
	exit ${RESULT};
fi

for TEST_NAME in ${LIBRARY_TESTS_WITH_INPUT};
do
	if test -d ${INPUT_DIRECTORY};
	then
		run_test_with_input "${TEST_NAME}";
		RESULT=$?;
	else
		run_test "${TEST_NAME}";
		RESULT=$?;
	fi

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		break;
	fi
done

exit ${RESULT};

