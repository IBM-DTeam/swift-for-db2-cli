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
# Docker ubuntu container (Travis CI) and tests it.

# If any commands fail, we want the shell script to exit immediately.
set -e

# Ubuntu 14.04
docker pull ubuntu:trusty

# Ubuntu 15.10
docker pull ubuntu:wily

# Ubuntu 16.04
docker pull ubuntu:xenial

# Ubuntu 16.10
docker pull ubuntu:yakkety

# Install dependencies
update="apt-get update > /dev/null"
upgrade="apt-get -y upgrade  > /dev/null"
install_dependencies="apt-get install -y wget make g++ gcc libcppunit-dev sudo libxml2 valgrind > /dev/null"
install_odbc_cli="cd /swift-for-db2-cli && ./setup/install.sh  > /dev/null && . ./setup/env.sh"

# Build the project and test it
build_and_test="make && make install && make clean && make test && ./test"

# Check for memory leaks
check_memory="valgrind ./test && make clean"

echo "********** TESTING Ubuntu 14.04 - TRUSTY **********"
docker run -v ${TRAVIS_BUILD_DIR}:/swift-for-db2-cli -i -t ubuntu:trusty /bin/bash -c "${update} && ${upgrade} && ${install_dependencies} && ${install_odbc_cli} && ${build_and_test} && ${check_memory}"

echo "********** TESTING Ubuntu 15.10 - WILY **********"
docker run -v ${TRAVIS_BUILD_DIR}:/swift-for-db2-cli -i -t ubuntu:wily /bin/bash -c "${update} && ${upgrade} && ${install_dependencies} && ${install_odbc_cli} && ${build_and_test} && ${check_memory}"

echo "********** TESTING Ubuntu 16.04 - XENIAL **********"
docker run -v ${TRAVIS_BUILD_DIR}:/swift-for-db2-cli -i -t ubuntu:xenial /bin/bash -c "${update} && ${upgrade} && ${install_dependencies} && ${install_odbc_cli} && ${build_and_test} && ${check_memory}"

echo "********** TESTING Ubuntu 16.10 - YAKKETY **********"
docker run -v ${TRAVIS_BUILD_DIR}:/swift-for-db2-cli -i -t ubuntu:yakkety /bin/bash -c "${update} && ${upgrade} && ${install_dependencies} && ${install_odbc_cli} && ${build_and_test} && ${check_memory}"
