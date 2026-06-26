#!/bin/sh
# Script that synchronizes the local test data

TESTS_INPUT_DIRECTORY="tests/input"
TEST_SET="public"
TEST_FILES="bdetogo.raw"

mkdir -p "${TESTS_INPUT_DIRECTORY}/${TEST_SET}"

for TEST_FILE in ${TEST_FILES}
do
	URL="https://raw.githubusercontent.com/log2timeline/dfvfs/refs/heads/main/test_data/${TEST_FILE}"

	curl -L -o "${TESTS_INPUT_DIRECTORY}/${TEST_SET}/${TEST_FILE}" ${URL}
done

cat > test_data_options << EOT
# libyal test data options
password=bde-TEST
EOT

mkdir -p "${TESTS_INPUT_DIRECTORY}/.libbde/${TEST_SET}";
cp test_data_options "${TESTS_INPUT_DIRECTORY}/.libbde/${TEST_SET}/${TEST_FILE}.password";

mkdir -p "${TESTS_INPUT_DIRECTORY}/.pybde/${TEST_SET}";
cp test_data_options "${TESTS_INPUT_DIRECTORY}/.pybde/${TEST_SET}/${TEST_FILE}.password";

mkdir -p "${TESTS_INPUT_DIRECTORY}/.bdeinfo/${TEST_SET}";
cp test_data_options "${TESTS_INPUT_DIRECTORY}/.bdeinfo/${TEST_SET}/${TEST_FILE}.password";

rm -f test_data_options
