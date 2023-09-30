#!/usr/bin/env python
#
# Script to run Python test scripts.
#
# Copyright (C) 2011-2023, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

import glob
import os
import sys
import unittest


if __name__ == "__main__":
  print(f"Using Python version {sys.version!s}")

  test_loader = unittest.TestLoader()
  test_runner = unittest.TextTestRunner(verbosity=2)

  test_scripts = test_loader.discover("tests", pattern="*.py")

  test_profile = ".pybde"
  input_glob = "*"

  ignore_list = set()

  ignore_file_path = f"tests/input/{test_profile}/ignore"
  if os.path.isfile(ignore_file_path):
    with open(ignore_file_path, "r", encoding="utf-8") as file_object:
      ignore_list = set([line.strip() for line in file_object.readlines()])

  source_file = None

  for test_set in glob.glob("tests/input/*"):
    test_set = test_set.rsplit('/', maxsplit=1)[-1]
    if not test_set or test_set[0] == '.' or test_set in ignore_list:
      continue

    source_files = glob.glob(f"tests/input/{test_set:s}/{input_glob:s}")
    if source_files:
      source_file = source_files[0]
      break

  setattr(unittest, "source", source_file)

  test_results = test_runner.run(test_scripts)
  if not test_results.wasSuccessful():
    sys.exit(1)
