VALID_CONN_STR = -DVALID_CONN_STR='"DRIVER={DB2};DATABASE=BLUDB;HOSTNAME=awh-yp-small03.services.dal.bluemix.net;PORT=50000;PROTOCOL=TCPIP;UID=dash104953;PWD=Exd8nmHNmSHT"'
INVALID_CONN_STR = -DINVALID_CONN_STR='"DRIVER={DB2};DATABASE=someDB;UID=someUID;PWD=somePWD;HOSTNAME=someHost;PORT=somePort"'
VALID_SELECT_QUERY_STR = -DVALID_SELECT_QUERY_STR='"SELECT * COL1 FROM MYTABLE WHERE COL1 = 1"'
INVALID_SELECT_QUERY_STR = -DINVALID_SELECT_QUERY_STR='"SELECT * COL1 FROM MYTABLE WHERE COL1 = 4"'
VALID_INSERT_QUERY_STR = -DVALID_SELECT_QUERY_STR='"INSERT INTO MYTABLE VALUES(3, 'hey')"'
INVALID_INSERT_QUERY_STR = -DINVALID_SELECT_QUERY_STR='"INSERT INTO MYTABLENOTEXIST VALUES(3, 'hey')"'
VALID_CREATE_QUERY_STR = -DVALID_SELECT_QUERY_STR='" CREATE TABLE "MYTABLE2" ( "COL1" INT, "COL2" VARCHAR(5) )"'
INVALID_DROP_QUERY_STR = -DVALID_SELECT_QUERY_STR='" DROP TABLE MYTABLENOTEXIST"'

CC = g++
OBJECT_FLAGS = -c -Wall -Wextra -g
OBJECT_FLAGS_SO = $(OBJECT_FLAGS) -fPIC
COMPILE_FLAGS = -Wall -Wextra -g
SHARED_LIBRARY = -shared
SHARED_LIBRARY_VERSION = 1.0

# Project paths
INCLUDE = include
SRC = src
TEST_INCLUDE = tests/include
TEST_SRC = tests/src

# Includes and Libraries for different build environments
PRODUCTION_SEARCH_PATH = -Iinclude -I/usr/local/ibmdb/include
PRODUCTION_LIBRARIES = -L/usr/local/ibmdb/lib
TEST_SEARCH_PATH = $(PRODUCTION_SEARCH_PATH) -Itests/include -I/usr/local/include/ibmdb2
TEST_LIBRARIES = $(PRODUCTION_LIBRARIES)

# Links for different build environments
PRODUCTION_LINKS = -ldb2
TEST_LINKS = $(PRODUCTION_LINKS) -lcppunit

OS := $(shell uname)
ifeq ($(OS), Darwin)
    EXTENSION = $(SHARED_LIBRARY_VERSION).dylib
else ifeq ($(OS), Linux)
    EXTENSION = so.$(SHARED_LIBRARY_VERSION)
else
    @echo "OS not supported, proceed with caution."
endif

# Shared Library
ibmdb2 : database.o error.o handle.o connect.o disconnect.o query.o
	$(CC) $(COMPILE_FLAGS) $(SHARED_LIBRARY) -o lib$@.$(EXTENSION) $? $(PRODUCTION_LIBRARIES) $(PRODUCTION_LINKS)

# Copy Shared Library and Includes
install : ibmdb2
	sudo cp lib$?.so.$(SHARED_LIBRARY_VERSION) /usr/local/lib
	sudo mkdir -p /usr/local/include/ibmdb2
	sudo cp -rf include/* /usr/local/include/ibmdb2/

# Compile tests
test : database.o error.o handle.o connect.o disconnect.o query.o test_database.o test_connect.o test_disconnect.o test_handle.o test_error.o test_main.o
	$(CC) $(COMPILE_FLAGS) -o $@ $? $(TEST_LIBRARIES) $(TEST_LINKS)

# Main files
database.o : $(INCLUDE)/database.h $(SRC)/database.c $(INCLUDE)/handle.h $(INCLUDE)/error.h $(INCLUDE)/type.h
	$(CC) $(OBJECT_FLAGS_SO) $(PRODUCTION_SEARCH_PATH) $(SRC)/database.c

error.o : $(INCLUDE)/error.h $(SRC)/error.c $(INCLUDE)/type.h $(INCLUDE)/handle.h
	$(CC) $(OBJECT_FLAGS_SO) $(PRODUCTION_SEARCH_PATH) -DVERSION=$(SHARED_LIBRARY_VERSION) $(SRC)/error.c

handle.o : $(INCLUDE)/handle.h $(SRC)/handle.c $(INCLUDE)/type.h
	$(CC) $(OBJECT_FLAGS_SO) $(PRODUCTION_SEARCH_PATH) $(SRC)/handle.c

connect.o : $(INCLUDE)/connect.h $(SRC)/connect.c $(INCLUDE)/database.h $(INCLUDE)/type.h $(INCLUDE)/error.h
	$(CC) $(OBJECT_FLAGS_SO) $(PRODUCTION_SEARCH_PATH) $(SRC)/connect.c

disconnect.o : $(INCLUDE)/disconnect.h $(SRC)/disconnect.c $(INCLUDE)/database.h $(INCLUDE)/type.h $(INCLUDE)/error.h
	$(CC) $(OBJECT_FLAGS_SO) $(PRODUCTION_SEARCH_PATH) $(SRC)/disconnect.c

query.o : $(INCLUDE)/query.h $(SRC)/query.c $(INCLUDE)/database.h $(INCLUDE)/type.h $(INCLUDE)/error.h
	$(CC) $(OBJECT_FLAGS_SO) $(PRODUCTION_SEARCH_PATH) $(SRC)/query.c

# Test files
test_main.o : $(TEST_INCLUDE)/test_main.hpp $(TEST_SRC)/test_main.cpp
	$(CC) $(OBJECT_FLAGS) $(TEST_SEARCH_PATH) $(TEST_SRC)/test_main.cpp

test_database.o : database.o $(TEST_INCLUDE)/test_database.hpp $(TEST_SRC)/test_database.cpp
	$(CC) $(OBJECT_FLAGS) $(TEST_SEARCH_PATH) $(TEST_SRC)/test_database.cpp

test_connect.o : connect.o $(TEST_INCLUDE)/test_connect.hpp $(TEST_SRC)/test_connect.cpp
	$(CC) $(OBJECT_FLAGS) $(TEST_SEARCH_PATH) $(VALID_CONN_STR) $(INVALID_CONN_STR) $(TEST_SRC)/test_connect.cpp

test_handle.o : handle.o $(TEST_INCLUDE)/test_handle.hpp $(TEST_SRC)/test_handle.cpp
	$(CC) $(OBJECT_FLAGS) $(TEST_SEARCH_PATH) $(TEST_SRC)/test_handle.cpp

test_error.o : error.o $(TEST_INCLUDE)/test_error.hpp $(TEST_SRC)/test_error.cpp
	$(CC) $(OBJECT_FLAGS) $(TEST_SEARCH_PATH) $(TEST_SRC)/test_error.cpp


test_disconnect.o : disconnect.o $(TEST_INCLUDE)/test_disconnect.hpp $(TEST_SRC)/test_disconnect.cpp
	$(CC) $(OBJECT_FLAGS) $(TEST_SEARCH_PATH) $(VALID_CONN_STR) $(INVALID_CONN_STR) $(TEST_SRC)/test_disconnect.cpp

clean :
	rm -rf *.o
	rm -rf *.i
	rm -rf *.s
	rm -rf test
	rm -rf ibmdb2
	rm -rf core
	rm -rf lib*.so.*
