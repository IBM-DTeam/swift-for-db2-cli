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
* Function:  TestDisconnect::testDisconnectSuccess
* ------------------
* Disconnects from a running DB
*
*/
void TestDisconnect::testDisconnectSuccess(void) {

  // Connect to the database.
  database* db = NULL;
  state c = connect(&db, (char*) VALID_CONN_STR);

  if(c != SUCCESS)
    freeDatabase(&db);

  CPPUNIT_ASSERT_MESSAGE("Couldn't connect to the databse", c == SUCCESS);

  state s = disconnect(&db);
  freeDatabase(&db);

  // Ensure we had a successful disconnection.
  CPPUNIT_ASSERT_MESSAGE("Couldn't Disconnect successfully from the database", s == SUCCESS);


}

/*
* Function:  TestDisconnect::testDisconnectFailure
* ------------------
* Fails to disconnect from a database
*
*/
void TestDisconnect::testDisconnectFailure(void) {

  // Create a NULL database
  database* db = NULL;
  state d = createDatabase(&db);

  if(d != SUCCESS)
    freeDatabase(&db);

  CPPUNIT_ASSERT_MESSAGE("Couldn't create the database", d == SUCCESS);

  state s = disconnect(&db);
  freeDatabase(&db);
  // Ensure we had an unsuccessful disconnection.
  CPPUNIT_ASSERT_MESSAGE("Proper disconnect from the database", s == DATABASE_DISCONNECT_FAILURE);

}


/*
* Function:  TestDisconnect::testDisconnectNoDatabase
* ------------------
* Fails to disconnect from a database
*
*/
void TestDisconnect::testDisconnectNoDatabase(void) {

  database* db = NULL;
  state s = disconnect(&db);

  CPPUNIT_ASSERT_MESSAGE("The database exists and there was a successfull disconnect", s == NO_DATABASE_FOUND);

}
