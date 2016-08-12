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

#include "test_connect.hpp"


/*
 * Function:  TestConnect::testConnectSuccess
 * ------------------
 * Connects to a database with a valid connection string.
 *
 */
void TestConnect::testConnectSuccess(void) {

  // Connect to the database.
  database* db = NULL;
  state s = connect(&db, (char*) VALID_CONN_STR);

  // Clean up
  freeDatabase(&db);

  // Ensure we had a successful connection.
  CPPUNIT_ASSERT_MESSAGE("Couldn't connect successfully to the database", s == SUCCESS);
}

/*
 * Function:  TestConnect::testConnectSuccessMultiple
 * ------------------
 * Connects to a database with a valid connection string, multiple times.
 *
 */
void TestConnect::testConnectSuccessMultiple(void) {

  int i = 0;
  for (; i < 5; i++)
    testConnectSuccess();

}

/*
 * Function:  TestConnect::testConnectCatastrophicFailure
 * ------------------
 * Fake a catastrophic failure to see our recovery.
 *
 */
void TestConnect::testConnectCatastrophicFailure(void) {
  // Nothing here for now.
  CPPUNIT_ASSERT_MESSAGE("Stub", 1);
}

/*
 * Function:  TestConnect::testConnectCatastrophicFailureMultiple
 * ------------------
 * Fake a catastrophic failure to see our recovery, multiple times.
 *
 */
void TestConnect::testConnectCatastrophicFailureMultiple(void) {

  int i = 0;
  for (; i < 5; i++)
    testConnectCatastrophicFailure();

}

/*
 * Function:  TestConnect::testConnectSetupDatabaseFailure
 * ------------------
 * Connects to a database with an invalid connection string.
 *
 */
void TestConnect::testConnectSetupDatabaseFailure(void) {
  // Try to connect to the database.
  database* db = NULL;
  state s = connect(&db, (char*) INVALID_CONN_STR);

  // Clean up
  freeDatabase(&db);

  // Ensure we had a database setup failure.
  CPPUNIT_ASSERT_MESSAGE("The database didn't fail to setup.", s == SETUP_DATABASE_FAILURE);

}

/*
 * Function:  TestConnect::testConnectSetupDatabaseFailureMultiple
 * ------------------
 * Connects to a database with an invalid connection string, multiple times.
 *
 */
void TestConnect::testConnectSetupDatabaseFailureMultiple(void) {

  int i = 0;
  for (; i < 5; i++)
    testConnectSetupDatabaseFailure();

}

/*
 * Function:  TestConnect::testConnectDatabaseExists
 * ------------------
 * Connects to a database with an existing database in the database**.
 *
 */
void TestConnect::testConnectDatabaseExists(void) {

  // Try to connect to the database
  database* db = NULL;
  state s = connect(&db, (char*) INVALID_CONN_STR);

  // Ensure we didn't connect to it
  if (s != SETUP_DATABASE_FAILURE)
    freeDatabase(&db);

  CPPUNIT_ASSERT_MESSAGE("Database didn't fail to setup.", s == SETUP_DATABASE_FAILURE);

  // Now we try to connect again to hit DATABASE_EXISTS
  s = connect(&db, (char*) INVALID_CONN_STR);

  // Clean up
  freeDatabase(&db);

  CPPUNIT_ASSERT_MESSAGE("The database didn't exist", s == DATABASE_EXISTS);

}

/*
 * Function:  TestConnect::testConnectDatabaseExistsMultiple
 * ------------------
 * Connects to a database with an existing database in the database**, multiple times.
 *
 */
void TestConnect::testConnectDatabaseExistsMultiple(void) {

  int i = 0;
  for (; i < 5; i++)
    testConnectDatabaseExists();

}
