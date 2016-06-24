# swift-for-db2-cli

This is the source repository for the ODBC CLI & Swift DB2 Driver installation package for the dependencies of [swift-for-db2](https://github.com/IBM-DTeam/swift-for-db2).

Package Info
---
* Platforms: x86_64, i386, i686, PowerPC, S390
* Operating Systems: Ubuntu 14.04, 15.10 or Mac OS X
* Dependencies: clang, unixodbc-dev, unzip, wget, tar

Installation
---

Both Mac OS and Ubuntu require libdispatch (https://github.com/apple/swift-corelibs-libdispatch/blob/experimental/foundation/INSTALL).

Ubuntu 14.04 and 15.10
```
sudo apt-get update
sudo apt-get install -y clang unixodbc-dev unzip wget tar
wget https://github.com/IBM-DTeam/swift-for-db2-cli/archive/master.zip && unzip master.zip && cd swift-for-db2-cli-master && sudo ./cli.sh && . env.sh && cd .. && rm -f master.zip && rm -rf swift-for-db2-cli-master
```

Mac OS X
```
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
brew install wget unixodbc
wget https://github.com/IBM-DTeam/swift-for-db2-cli/archive/master.zip && unzip master.zip && cd swift-for-db2-cli-master && sudo ./cli.sh && . env.sh && cd .. && rm -f master.zip && rm -rf swift-for-db2-cli-master
```

Contributors
---
* Gregory Wlodarek (gregorywlodarek@gmail.com)
* Angad Singh (angaduom@gmail.com)
* Nick Dujay (nickdujay@gmail.com)
* IBM

Licence
---
Copyright (c) 2016 IBM

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
