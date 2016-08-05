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

# Ubuntu 15.10
docker pull ubuntu:wily

# Install dependencies
update="apt-get update"
upgrade="apt-get upgrade"
install_dependencies="apt-get install -y wget make g++ gcc libcppunit-dev"
install_odbc_cli="cd /swift-for-db2-cli && ./setup/install.sh"

# Build the project and test it
build_and_test="make && make install && make clean && make test && ./test"

docker run -v ${TRAVIS_BUILD_DIR}:/swift-for-db2-cli -i -t ubuntu:wily /bin/bash -c "${update} && ${upgrade} && ${install_dependencies} && ${install_odbc_cli} && ${build_and_test}"
