#!/bin/bash

##
# Copyright IBM Corporation 2016
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##

# This script builds the corresponding Swift IBM DB Package in a
# Docker OS X container (Travis CI) and tests it.

# If any commands fail, we want the shell script to exit immediately.
set -e

# Install dependencies
brew install wget cppunit > /dev/null

# Project location
cd ${TRAVIS_BUILD_DIR}

# Install ODBC CLI
sudo ./setup/install.sh > /dev/null

# Environment variables
. ./setup/env.sh

# Build the project and test it
make
sudo make install
make clean
make test
./test
valgrind --leak-check=full --show-reachable=yes --undef-value-errors=no --error-exitcode=-1 ./test
make clean
