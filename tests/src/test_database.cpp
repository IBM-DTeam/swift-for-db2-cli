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

#include "test_database.hpp"


/*
 * Function:  TestDisconnect::testDatabaseSuccess
 * ------------------
 * Creates a database struct successfully.
 *
 */
void TestDatabase::testCreateDatabaseSuccess(void) {

  // Create the database struct
  database* db = NULL;
  state s = createDatabase(&db);

  // Clean up
  freeDatabase(&db);

  // Ensure we had a successful connection.
  CPPUNIT_ASSERT_MESSAGE("Couldn't create the database struct", s == SUCCESS);
}

/*
 * Function:  TestDisconnect::testDatabaseSuccessMultiple
 * ------------------
 * Creates a database struct successfully, multiple times.
 *
 */
void TestDatabase::testCreateDatabaseSuccessMultiple(void) {

  int i = 0;
  for (; i < 5; i++)
    testCreateDatabaseSuccess();

}

/*
 * Function:  TestDisconnect::testDatabaseMallocFailure
 * ------------------
 * Tries to create a database struct, but hits a malloc error.
 *
 */
void TestDatabase::testCreateDatabaseMallocFailure(void) {

  // Nothing here for now.
  CPPUNIT_ASSERT_MESSAGE("Stub", 1);

}

/*
 * Function:  TestDisconnect::testDatabaseMallocFailureMultiple
 * ------------------
 * Tries to create a database struct, but hits a malloc error, multiple times.
 *
 */
void TestDatabase::testCreateDatabaseMallocFailureMultiple(void) {

  int i = 0;
  for (; i < 5; i++)
    testCreateDatabaseMallocFailure();

}


/*
 * Function:  TestDisconnect::testFreeDatabase
 * ------------------
 * Creates and frees a basic database struct.
 * NOTE: Inner structs such as handles and errors will be tested more in depth
 * in their own unit tests,
 *
 */
void TestDatabase::testFreeDatabase(void) {

  database* db = NULL;

  state s = createDatabase(&db);
  CPPUNIT_ASSERT_MESSAGE("State wasn't successful", s == SUCCESS);

  freeDatabase(&db);

  // Valgrind will check for leaks.
  CPPUNIT_ASSERT_MESSAGE("Database wasn't freed properly.", db == NULL);


}

/*
 * Function:  TestDisconnect::testFreeDatabaseMultiple
 * ------------------
 * Creates and frees a basic database struct, multiple times.
 * NOTE: Inner structs such as handles and errors will be tested more in depth
 * in their own unit tests,
 *
 */
void TestDatabase::testFreeDatabaseMultiple(void) {

  int i = 0;
  for (; i < 5; i++)
    testFreeDatabase();


}
