#!/bin/sh
set -e

URL="http://public.dhe.ibm.com/ibmdl/export/pub/software/data/db2/drivers/odbc_cli/"
UNAME=`uname`
ARCH=`getconf LONG_BIT`
PROCESSOR=`uname -p`

if [ $UNAME = "Linux" ]; then
  if [ $ARCH = "64" ]; then
    if [ $PROCESSOR = "x86_64" ]; then
      DIST=linuxx64_odbc_cli.tar.gz
    elif [ $PROCESSOR = "ppc64" ]; then
      ENDIAN=`lscpu | grep Endian`
      if [ $ENDIAN = *"Little Endian"* ]; then
        DIST=ppc64le_odbc_cli.tar.gz
      else
        DIST=ppc64_odbc_cli.tar.gz
      fi
    elif [ $PROCESSOR = "s390x" ]; then
      DIST=s390x64_odbc_cli.tar.gz
    else
      echo "Processor not supported."
      exit 1
    fi
  elif [ $ARCH = "32" ]; then
    if [ $PROCESSOR = "ppc32" ]; then
      DIST=ppc32_odbc_cli.tar.gz
    elif [ $PROCESSOR = "s390" ]; then
      DIST=s390_odbc_cli.tar.gz
    else
      DIST=linuxia32_odbc_cli.tar.gz
    fi
  fi
elif [ $UNAME = "Darwin" ]; then
  if [ $ARCH = "64" ]; then
    DIST=macos64_odbc_cli.tar.gz
  else
    echo "Mac OS 32 bit not supported. Please use an x64 architecture."
    exit 1
  fi
else
  echo "Operating system not supported yet."
  exit 1
fi

rm -rf ibm_db.o
clang -c src/*.c -O3 -I/usr/local/include -I/usr/include
ar -rcs libibmdb.a *.o

sudo mkdir -p /usr/local/include
sudo mkdir -p /usr/local/lib

sudo cp -rfp include/*.h /usr/local/include/
sudo cp -rfp libibmdb.a /usr/local/lib/
rm libibmdb.a

rm -rf $DIST
wget $URL$DIST
tar -xvzf $DIST
sudo mkdir -p /usr/local/ibmdb

if [ $UNAME = "Linux" ]; then
  sudo cp -rfp clidriver/lib/* /usr/lib/
elif [ $UNAME = "Darwin" ]; then
  sudo cp -rfp clidriver/lib/* /usr/local/lib
fi

sudo cp -rfp clidriver/* /usr/local/ibmdb/
rm -rf clidriver

rm -rf ibm_db.o
rm -rf $DIST

######################### ENVIRONMENT VARIABLES ##############################
# Persistant environment variables for all other sessions

sudo mkdir -p /etc/profile.d

sudo echo "export PATH=/usr/local/ibmdb/bin:/usr/local/ibmdb/adm:/usr/local/ibmdb/lib:\$PATH" > /etc/profile.d/db2_swift.sh
sudo echo "export LD_LIBRARY_PATH=/usr/local/ibmdb/lib:\$LD_LIBRARY_PATH" >> /etc/profile.d/db2_swift.sh
sudo echo "export DYLD_LIBRARY_PATH=/usr/local/ibmdb/lib:/usr/local/ibmdb/lib/icc:\$DYLD_LIBRARY_PATH" >> /etc/profile.d/db2_swift.sh

sudo echo "export IBM_DB_DIR=/usr/local/ibmdb:\$IBM_DB_DIR" >> /etc/profile.d/db2_swift.sh
sudo echo "export IBM_DB_HOME=/usr/local/ibmdb:\$IBM_DB_HOME" >> /etc/profile.d/db2_swift.sh
sudo echo "export IBM_DB_LIB=/usr/local/ibmdb/lib:\$IBM_DB_LIB" >> /etc/profile.d/db2_swift.sh
sudo echo "export IBM_DB_INCLUDE=/usr/local/ibmdb/include:\$IBM_DB_INCLUDE" >> /etc/profile.d/db2_swift.sh
sudo echo "export DB2_HOME=/usr/local/ibmdb/include:\$DB2_HOME" >> /etc/profile.d/db2_swift.sh
sudo echo "export DB2LIB=/usr/local/ibmdb/lib:\$DB2LIB" >> /etc/profile.d/db2_swift.sh

if [ $UNAME = "Darwin" ]; then
  if ! grep -Fxq "if [ -d /etc/profile.d ]; then" /etc/profile ; then
    sudo echo "if [ -d /etc/profile.d ]; then" >> /etc/profile
    sudo echo "  for i in /etc/profile.d/*.sh; do" >> /etc/profile
    sudo echo "    if [ -r $i ]; then" >> /etc/profile
    sudo echo "      . $i" >> /etc/profile
    sudo echo "    fi" >> /etc/profile
    sudo echo "  done" >> /etc/profile
    sudo echo "  unset i" >> /etc/profile
    sudo echo "fi" >> /etc/profile
  fi
fi
