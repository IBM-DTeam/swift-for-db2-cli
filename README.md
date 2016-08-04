![swift-for-db2-cli](https://ibm.app.box.com/representation/file_version_81797080661/image_2048/1.png?shared_name=b1f1yajuxqlip32zoe185bp27mhcc4pw)

**Swift SDK for DB2 CLI**

[![Build Status - Master](https://travis-ci.org/IBM-DTeam/swift-for-db2-cli.svg?branch=master)](https://travis-ci.org/IBM-DTeam/swift-for-db2-cli)
![Mac OS X](https://img.shields.io/badge/os-Mac%20OS%20X-green.svg?style=flat)
![Linux](https://img.shields.io/badge/os-linux-green.svg?style=flat)
![Apache 2](https://img.shields.io/badge/license-Apache2-blue.svg?style=flat)

## Summary

The Swift SDK for DB2 CLI allows you to connect to your IBM DB2 database or products based off IBM DB2 and execute queries in C/C++.

## Table of Contents
* [Summary](#summary)
* [Features](#features)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
* [Using Swift SDK for DB2 CLI](#using-swift-sdk-for-db2-cli)
* [Usage Examples](#usage-examples)
* [Contributing](#contributing)
* [License](#license)

## Features:

- [x] Connect to your database.
- [ ] Query and get results.
- [ ] Create Prepared Statements.
- [ ] Perform Transactions.
- [ ] Disconnect from your database.

## Prerequisites

1. For Linux, you will need to execute the following:

  `sudo apt-get update && sudo apt-get install wget g++ gcc libcppunit-dev`
  `sudo ./setup/install.sh`

2. For OS X, you will need to execute the following:
  `brew install wget cppunit`
  `sudo ./setup/install.sh`

## Installation

  `make`
  `sudo make install`

## Usage Examples
Visit the [Wiki](https://github.com/IBM-DTeam/swift-for-db2-cli/wiki) for examples on how to use the Swift SDK for DB2 CLI.

## Contributing
1. Clone this repository, `git clone https://github.com/IBM-DTeam/swift-for-db2-cli`
2. Build and run tests

  ### Notes
  * You can configure the variables VALID_CONN_STR and INVALID_CONN_STR in the makefile if you wish to. However, they are already set for you.

You can find info on contributing to Swift SDK for DB2 CLI in our [contributing guidelines](CONTRIBUTING.md).

## License
This library is licensed under Apache 2.0. Full license text is available in [LICENSE](LICENSE.txt).
