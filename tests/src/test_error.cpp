/**
 * Copyright IBM Corporation 2016
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

#include "test_error.hpp"
#include <stdio.h>


/*
 * Function:  TestError::testInitializeErrorSuccess
 * ------------------
 * Create a Error struct, successfully.
 *
 */
void TestError::testInitializeErrorSuccess(void) {

  // Initialize the error
  error* e = NULL;
  state s = db_initializeError(&e);

  // Clean up
  db_freeErrors(&e);

  CPPUNIT_ASSERT_MESSAGE("Couldn't initialize the error.", s == SUCCESS);

}

/*
 * Function:  TestError::testInitializeErrorSuccessMultiple
 * ------------------
 * Create a Error struct, successfully, multiple times.
 *
 */
void TestError::testInitializeErrorSuccessMultiple(void) {

  int i = 0;
  for (; i < 5; i++)
    testInitializeErrorSuccess();

}

/*
 * Function:  TestError::testInitializeErrorMallocFailure
 * ------------------
 * Create a Error struct, but fail due to a malloc issue.
 *
 */
void TestError::testInitializeErrorMallocFailure(void) {

  // Nothing here for now.
  CPPUNIT_ASSERT_MESSAGE("Stub", 1);

}

/*
 * Function:  TestError::testInitializeErrorMallocFailureMulitple
 * ------------------
 * Create a Error struct, but fail due to a malloc issue, multiple times.
 *
 */
void TestError::testInitializeErrorMallocFailureMulitple(void) {

  int i = 0;
  for (; i < 5; i++)
    testInitializeErrorMallocFailure();

}

/*
 * Function:  TestError::testResetError
 * ------------------
 * Connects to an invalid database to generate an error. We manually reset the error
 * afterwards to ensure the functionality works.
 *
 */
void TestError::testResetErrors(void) {

  // Connect to an invalid database
  database* db = NULL;
  state s = db_connect(&db, (char*) INVALID_CONN_STR);

  // Ensure we didn't connect
  CPPUNIT_ASSERT_MESSAGE("We connected to the database.", s == SETUP_DATABASE_FAILURE);

  // "connect" should've generated an error.
  CPPUNIT_ASSERT_MESSAGE("We don't have a error struct initialized properly.", db->err && db->err->errorType && db->err->database);
  CPPUNIT_ASSERT_MESSAGE("Connect set the wrong error type.", db->err->errorType == DATABASE_ERROR);

  // Reset the error.
  db_resetErrors(db->err);

  // Ensure it was reset.
  CPPUNIT_ASSERT_MESSAGE("The type wasn't reset properly.", db->err->errorType == NO_ERROR);
  CPPUNIT_ASSERT_MESSAGE("The database errors aren't empty.", db->err->database == NULL);

  // Clean up
  db_freeDatabase(&db);

}

/*
 * Function:  TestError::testResetErrorMultiple
 * ------------------
 * Connects to an invalid database to generate an error. We manually reset the error
 * afterwards to ensure the functionality works, multiple times.
 *
 */
void TestError::testResetErrorsMultiple(void) {

  int i = 0;
  for (; i < 5; i++)
    testResetErrors();

}

/*
 * Function:  TestError::testFreeError
 * ------------------
 * Connects to an invalid database to generate an error. We manually free the error
 * afterwards to ensure the functionality works.
 *
 */
void TestError::testFreeErrors(void) {

  // Connect to an invalid database
  database* db = NULL;
  state s = db_connect(&db, (char*) INVALID_CONN_STR);

  // Ensure we didn't connect
  CPPUNIT_ASSERT_MESSAGE("We connected to the database.", s == SETUP_DATABASE_FAILURE);

  // "connect" should've generated an error.
  CPPUNIT_ASSERT_MESSAGE("We don't have a error struct initialized properly.", db->err && db->err->errorType && db->err->database);
  CPPUNIT_ASSERT_MESSAGE("Connect set the wrong error type.", db->err->errorType == DATABASE_ERROR);

  // Free the error.
  db_freeErrors(&(db->err));

  // Ensure it was freed.
  CPPUNIT_ASSERT_MESSAGE("The error wasn't freed properly.", db->err == NULL);

  // Clean up
  db_freeDatabase(&db);

}

/*
 * Function:  TestError::testFreeErrorMultiple
 * ------------------
 * Connects to an invalid database to generate an error. We manually free the error
 * afterwards to ensure the functionality works, multiple times.
 *
 */
void TestError::testFreeErrorsMultiple(void) {

  int i = 0;
  for (; i < 5; i++)
    testFreeErrors();

}

/*
 * Function:  TestError::testGetNextError
 * ------------------
 * Connects to an invalid database to generate an error. We call getNextError
 * to ensure functionality works.
 *
 */
void TestError::testGetNextError(void) {

  // Connect to an invalid database
  database* db = NULL;
  state s = db_connect(&db, (char*) INVALID_CONN_STR);

  // Ensure we didn't connect
  CPPUNIT_ASSERT_MESSAGE("We connected to the database.", s == SETUP_DATABASE_FAILURE);

  // "connect" should've generated an error.
  CPPUNIT_ASSERT_MESSAGE("We don't have a error struct initialized properly.", db->err && db->err->errorType && db->err->database);
  CPPUNIT_ASSERT_MESSAGE("Connect set the wrong error type.", db->err->errorType == DATABASE_ERROR);

  // getNextError shouldn't return NULL.
  databaseError* e = db_getNextError(db);
  CPPUNIT_ASSERT_MESSAGE("There was no database error.", e != NULL);

  db_freeDatabaseError(&e);
  db_freeErrors(&(db->err));

  // Ensure it was freed.
  CPPUNIT_ASSERT_MESSAGE("The error wasn't freed properly.", db->err == NULL);

  // Clean up
  db_freeDatabase(&db);

}

/*
 * Function:  TestError::testGetNextErrorMultiple
 * ------------------
 * Connects to an invalid database to generate an error. We call getNextError
 * to ensure functionality works, multiple times.
 *
 */
void TestError::testGetNextErrorMultiple(void) {

  int i = 0;
  for (; i < 5; i++)
    testGetNextError();

}

/*
 * Function:  TestError::testGetNextErrorNull
 * ------------------
 * Connects to an invalid database to generate an error. We call getNextError
 * to ensure functionality works.
 *
 */
void TestError::testGetNextErrorNull(void) {

  // Connect to an invalid database
  database* db = NULL;
  state s = db_connect(&db, (char*) INVALID_CONN_STR);

  // Ensure we didn't connect
  CPPUNIT_ASSERT_MESSAGE("We connected to the database.", s == SETUP_DATABASE_FAILURE);

  // "connect" should've generated an error.
  CPPUNIT_ASSERT_MESSAGE("We don't have a error struct initialized properly.", db->err && db->err->errorType && db->err->database);
  CPPUNIT_ASSERT_MESSAGE("Connect set the wrong error type.", db->err->errorType == DATABASE_ERROR);

  // getNextError shouldn't return NULL.
  databaseError* e = db_getNextError(db);
  CPPUNIT_ASSERT_MESSAGE("There was no database error.", e != NULL);
  db_freeDatabaseError(&e);

  // There shouldn't be more errors.
  e = db_getNextError(db);
  CPPUNIT_ASSERT_MESSAGE("There was a database error.", e == NULL);

  db_freeErrors(&(db->err));

  // Ensure it was freed.
  CPPUNIT_ASSERT_MESSAGE("The error wasn't freed properly.", db->err == NULL);

  // Clean up
  db_freeDatabase(&db);

}

/*
 * Function:  TestError::testGetNextErrorNullMultiple
 * ------------------
 * Connects to an invalid database to generate an error. We call getNextError
 * to ensure functionality works, multiple times.
 *
 */
void TestError::testGetNextErrorNullMultiple(void) {

  int i = 0;
  for (; i < 5; i++)
    testGetNextErrorNull();

}


/*
 * Function:  TestError::testGenerateDatabaseErrorSuccess
 * ------------------
 * Connects to an invalid database to generate an error. We ensure the
 * error was properly setup.
 *
 */
void TestError::testGenerateDatabaseErrorSuccess(void) {

  // Connect to an invalid database
  database* db = NULL;
  state s = db_connect(&db, (char*) INVALID_CONN_STR);

  // Ensure we didn't connect
  CPPUNIT_ASSERT_MESSAGE("We connected to the database.", s == SETUP_DATABASE_FAILURE);

  // "connect" should've generated an error.
  CPPUNIT_ASSERT_MESSAGE("We don't have a error struct initialized properly.", db->err && db->err->errorType && db->err->database);
  CPPUNIT_ASSERT_MESSAGE("Connect set the wrong error type.", db->err->errorType == DATABASE_ERROR);

  // Clean up
  db_freeDatabase(&db);

}

/*
 * Function:  TestError::testGenerateDatabaseErrorSuccessMultiple
 * ------------------
 * Connects to an invalid database to generate an error. We ensure the
 * error was properly setup, multiple times.
 *
 */
void TestError::testGenerateDatabaseErrorSuccessMultiple(void) {

  int i = 0;
  for (; i < 5; i++)
    testGenerateDatabaseErrorSuccess();

}

/*
 * Function:  TestError::testGenerateDatabaseErrorMallocFailure
 * ------------------
 * Attempts to generate a database error but fails to due a malloc failure.
 *
 */
void TestError::testGenerateDatabaseErrorMallocFailure(void) {

  // Nothing here for now.
  CPPUNIT_ASSERT_MESSAGE("Stub", 1);

}

/*
 * Function:  TestError::testGenerateDatabaseErrorMallocFailureMultiple
 * ------------------
 * Attempts to generate a database error but fails to due a malloc failure,
 * multiple times.
 *
 */
void TestError::testGenerateDatabaseErrorMallocFailureMultiple(void) {

  int i = 0;
  for (; i < 5; i++)
    testGenerateDatabaseErrorMallocFailure();

}

/*
 * Function:  TestError::testGenerateDatabaseErrorFetchDatabaseErrorFailure
 * ------------------
 * Attempts to generate an error but fails to do so.
 *
 */
void TestError::testGenerateDatabaseErrorFetchDatabaseErrorFailure(void) {

  // Create an empty error
  error* e = NULL;
  state s = db_initializeError(&e);

  // Ensure it setup properly.
  CPPUNIT_ASSERT_MESSAGE("Couldn't initialize the error.", e != NULL);

  // Try to generate an error on an invalid database connection handle.
  s = db_generateDatabaseError(e, SQL_NULL_HDBC, SQL_HANDLE_DBC);

  CPPUNIT_ASSERT_MESSAGE("The database fetched an error, it shouldn't.", s == FETCH_DATABASE_ERROR_FAILURE);

  // Clean up
  db_freeErrors(&e);

}

/*
 * Function:  TestError::testGenerateDatabaseErrorFetchDatabaseErrorFailureMultiple
 * ------------------
 * Attempts to generate an error but fails to do so, multiple times.
 *
 */
void TestError::testGenerateDatabaseErrorFetchDatabaseErrorFailureMultiple(void) {

  int i = 0;
  for (; i < 5; i++)
    testGenerateDatabaseErrorFetchDatabaseErrorFailure();

}

/*
 * Function:  TestError::freeDatabaseError
 * ------------------
 * Connects to an invalid database to generate an error. We manually free the database
 * error afterwards to ensure the functionality works.
 *
 */
void TestError::testFreeDatabaseError(void) {
  // Connect to an invalid database
  database* db = NULL;
  state s = db_connect(&db, (char*) INVALID_CONN_STR);

  // Ensure we didn't connect
  CPPUNIT_ASSERT_MESSAGE("We connected to the database.", s == SETUP_DATABASE_FAILURE);

  // "connect" should've generated an error.
  CPPUNIT_ASSERT_MESSAGE("We don't have a error struct initialized properly.", db->err && db->err->errorType && db->err->database);
  CPPUNIT_ASSERT_MESSAGE("Connect set the wrong error type.", db->err->errorType == DATABASE_ERROR);

  // getNextError shouldn't return NULL.
  databaseError* e = db_getNextError(db);
  CPPUNIT_ASSERT_MESSAGE("There was no database error.", e != NULL);
  db_freeDatabaseError(&e);

  // There shouldn't be more errors.
  e = db_getNextError(db);
  CPPUNIT_ASSERT_MESSAGE("There was a database error.", e == NULL);

  db_freeErrors(&(db->err));

  // Ensure it was freed.
  CPPUNIT_ASSERT_MESSAGE("The error wasn't freed properly.", db->err == NULL);

  // Clean up
  db_freeDatabase(&db);
}

/*
 * Function:  TestError::freeDatabaseErrorMultiple
 * ------------------
 * Connects to an invalid database to generate an error. We manually free the database
 * error afterwards to ensure the functionality works, multiple times.
 *
 */
void TestError::testFreeDatabaseErrorMultiple(void) {

  int i = 0;
  for (; i < 5; i++)
    testFreeDatabaseError();

}

/*
 * Function:  TestError::freeDatabaseErrors
 * ------------------
 * Connects to an invalid database to generate an error. We manually free the database
 * error afterwards to ensure the functionality works.
 *
 */
void TestError::testFreeDatabaseErrors(void) {

  // Connect to an invalid database
  database* db = NULL;
  state s = db_connect(&db, (char*) INVALID_CONN_STR);

  // Ensure we didn't connect
  CPPUNIT_ASSERT_MESSAGE("We connected to the database.", s == SETUP_DATABASE_FAILURE);

  // "connect" should've generated an error.
  CPPUNIT_ASSERT_MESSAGE("We don't have a error struct initialized properly.", db->err && db->err->errorType && db->err->database);
  CPPUNIT_ASSERT_MESSAGE("Connect set the wrong error type.", db->err->errorType == DATABASE_ERROR);

  // Free the database error.
  db_freeDatabaseErrors(db->err);

  // Ensure it was reset.
  CPPUNIT_ASSERT_MESSAGE("The type wasn't reset properly.", db->err->database == NULL);

  // Clean up
  db_freeDatabase(&db);

}

/*
 * Function:  TestError::freeDatabaseErrorsMultiple
 * ------------------
 * Connects to an invalid database to generate an error. We manually free the database
 * error afterwards to ensure the functionality works, multiple times.
 *
 */
void TestError::testFreeDatabaseErrorsMultiple(void) {

  int i = 0;
  for (; i < 5; i++)
    testFreeDatabaseErrors();

}
