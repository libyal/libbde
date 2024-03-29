#!/bin/sh
# Script that synchronizes the local test data
#
# Version: 20231001

TEST_SET="public";
TEST_INPUT_DIRECTORY="tests/input";
TEST_FILES="bdetogo.raw";

mkdir -p "${TEST_INPUT_DIRECTORY}/${TEST_SET}";
mkdir -p "${TEST_INPUT_DIRECTORY}/.libbde/${TEST_SET}";
mkdir -p "${TEST_INPUT_DIRECTORY}/.pybde/${TEST_SET}";
mkdir -p "${TEST_INPUT_DIRECTORY}/.bdeinfo/${TEST_SET}";

cat > test_data_options << EOT
# libyal test data options
password=bde-TEST
EOT

for TEST_FILE in ${TEST_FILES};
do
	URL="https://github.com/log2timeline/dfvfs/blob/main/test_data/${TEST_FILE}?raw=true";

	curl -L -o "${TEST_INPUT_DIRECTORY}/${TEST_SET}/${TEST_FILE}" ${URL};

	cp test_data_options "${TEST_INPUT_DIRECTORY}/.libbde/${TEST_SET}/${TEST_FILE}.password";
	cp test_data_options "${TEST_INPUT_DIRECTORY}/.pybde/${TEST_SET}/${TEST_FILE}.password";
	cp test_data_options "${TEST_INPUT_DIRECTORY}/.bdeinfo/${TEST_SET}/${TEST_FILE}.password";
done

