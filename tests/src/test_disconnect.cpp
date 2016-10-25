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

#include "test_disconnect.hpp"


/*
* Function:  TestDisconnect::testDisconnectNoDatabaseFound
* ------------------
* Fails to disconnect from a database
*
*/
void TestDisconnect::testDisconnectNoDatabaseFound(void) {

  database* db = NULL;
  state s = db_disconnect(&db);

  CPPUNIT_ASSERT_MESSAGE("The database exists and there was a successful disconnect.", s == NO_DATABASE_FOUND);

}

/*
* Function:  TestDisconnect::testDisconnectNoDatabaseFoundMultiple
* ------------------
* Fails to disconnect from a database, multiple times
*
*/
void TestDisconnect::testDisconnectNoDatabaseFoundMultiple(void) {

  int i = 0;
  for (; i < 5; i++)
    testDisconnectNoDatabaseFound();

}

/*
* Function:  TestDisconnect::testDisconnectDatabaseDisconnectFailure
* ------------------
* Fails to disconnect from a database
*
*/
void TestDisconnect::testDisconnectDatabaseDisconnectFailure(void) {

  // Create a NULL database
  database* db = NULL;
  state s = db_createDatabase(&db);

  CPPUNIT_ASSERT_MESSAGE("Couldn't create the database.", s == SUCCESS);

  s = db_disconnect(&db);

  // Clean up
  db_freeDatabase(&db);

  // Ensure we had an unsuccessful disconnection.
  CPPUNIT_ASSERT_MESSAGE("Proper disconnect from the database.", s == DATABASE_DISCONNECT_FAILURE);

}

/*
* Function:  TestDisconnect::testDisconnectDatabaseDisconnectFailureMultiple
* ------------------
* Fails to disconnect from a database, multiple times
*
*/
void TestDisconnect::testDisconnectDatabaseDisconnectFailureMultiple(void) {

  int i = 0;
  for (; i < 5; i++)
    testDisconnectDatabaseDisconnectFailure();

}

/*
* Function:  TestDisconnect::testDisconnectSuccess
* ------------------
* Disconnects from a running DB
*
*/
void TestDisconnect::testDisconnectSuccess(void) {

  // Connect to the database.
  database* db = NULL;
  state s = db_connect(&db, (char*) VALID_CONN_STR);

  // Clean up
  if (s == SETUP_DATABASE_FAILURE)
    db_freeDatabase(&db);

  CPPUNIT_ASSERT_MESSAGE("Couldn't connect to the database.", s == SUCCESS || s == SUCCESS_WITH_INFO);

  s = db_disconnect(&db);

  // Ensure we had a successful disconnection.
  CPPUNIT_ASSERT_MESSAGE("Couldn't disconnect from the database.", s == SUCCESS);

}

/*
* Function:  TestDisconnect::testDisconnectSuccessMultiple
* ------------------
* Disconnects from a running DB, multiple times.
*
*/
void TestDisconnect::testDisconnectSuccessMultiple(void) {

  int i = 0;
  for (; i < 5; i++)
    testDisconnectSuccess();

}
