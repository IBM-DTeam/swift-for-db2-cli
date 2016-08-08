# Valid Connection String
VALID_CONN_STR = -DVALID_CONN_STR='"DRIVER={DB2};DATABASE=BLUDB;UID=dash6435;PWD=0NKUFZxcskVZ;HOSTNAME=dashdb-entry-yp-dal09-09.services.dal.bluemix.net;PORT=50000"'
# Invalid Connection String
INVALID_CONN_STR = -DINVALID_CONN_STR='"DRIVER={DB2};DATABASE=someDB;UID=someUID;PWD=somePWD;HOSTNAME=someHost;PORT=somePort"'

CC = g++
OBJECT_FLAGS = -c -Wall -Wextra
OBJECT_FLAGS_SO = $(OBJECT_FLAGS) -fPIC
COMPILE_FLAGS = -Wall -Wextra
SHARED_LIBRARY = -shared
SHARED_LIBRARY_VERSION = 1

# Project paths
INCLUDE = include
SRC = src
TEST_INCLUDE = tests/include
TEST_SRC = tests/src

# Includes and Libraries for different build environments
PRODUCTION_SEARCH_PATH = -Iinclude -I/usr/local/ibmdb/include
PRODUCTION_LIBRARIES = -L/usr/local/ibmdb/lib -L/usr/local/lib
TEST_SEARCH_PATH = $(PRODUCTION_SEARCH_PATH) -Itests/include -I/usr/local/include/ibmdb2
TEST_LIBRARIES = $(PRODUCTION_LIBRARIES)

# Links for different build environments
PRODUCTION_LINKS = -ldb2
TEST_LINKS = $(PRODUCTION_LINKS) -lcppunit

# Shared Library
ibmdb2 : handles.o connect.o disconnect.o
	$(CC) $(COMPILE_FLAGS) $(SHARED_LIBRARY) -o lib$@.so.$(SHARED_LIBRARY_VERSION) $? $(PRODUCTION_LIBRARIES) $(PRODUCTION_LINKS)

# Copy Shared Library and Includes
install : ibmdb2
	sudo cp lib$?.so.$(SHARED_LIBRARY_VERSION) /usr/local/lib
	sudo mkdir -p /usr/local/include/ibmdb2
	sudo cp -rf include/* /usr/local/include/ibmdb2/

# Compile tests
test : handles.o test_handles.o connect.o test_connect.o test_main.o
	$(CC) $(COMPILE_FLAGS) -o $@ $? $(TEST_LIBRARIES) $(TEST_LINKS)

# Main files
handles.o : $(INCLUDE)/handles.h $(SRC)/handles.c
	$(CC) $(OBJECT_FLAGS_SO) $(PRODUCTION_SEARCH_PATH) $(SRC)/handles.c

connect.o : handles.o $(INCLUDE)/connect.h $(SRC)/connect.c
	$(CC) $(OBJECT_FLAGS_SO) $(PRODUCTION_SEARCH_PATH) $(SRC)/connect.c

disconnect.o : handles.o $(INCLUDE)/disconnect.h $(SRC)/disconnect.c
	$(CC) $(OBJECT_FLAGS_SO) $(PRODUCTION_SEARCH_PATH) $(SRC)/disconnect.c

# Test files
test_main.o : test_handles.o test_connect.o $(TEST_INCLUDE)/test_main.hpp $(TEST_SRC)/test_main.cpp
	$(CC) $(OBJECT_FLAGS) $(TEST_SEARCH_PATH) $(TEST_SRC)/test_main.cpp

test_connect.o : test_handles.o connect.o $(TEST_INCLUDE)/test_connect.hpp $(TEST_SRC)/test_connect.cpp
	$(CC) $(OBJECT_FLAGS) $(TEST_SEARCH_PATH) $(VALID_CONN_STR) $(INVALID_CONN_STR) $(TEST_SRC)/test_connect.cpp

test_handles.o : handles.o $(TEST_INCLUDE)/test_handles.hpp $(TEST_SRC)/test_handles.cpp
	$(CC) $(OBJECT_FLAGS) $(TEST_SEARCH_PATH) $(TEST_SRC)/test_handles.cpp

clean :
	rm -rf *.o
	rm -rf *.i
	rm -rf *.s
	rm -rf test
	rm -rf ibmdb2
	rm -rf core
	rm -rf lib*.so.*
