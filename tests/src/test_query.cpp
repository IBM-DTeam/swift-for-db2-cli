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

#include "test_query.hpp"
#include <stdio.h>


/*
 * Function:  TestQuery::testConnectSuccess
 * ------------------
 * Connects to a database with a valid connection string.
 *
 */
void TestQuery::testSelectQuerySuccess(void) {

  // Connect to the database.
  database* db = NULL;
  state s = connect(&db, (char*) VALID_CONN_STR);
  queryStruct* testQuery = NULL;

  state d = query(db, &testQuery, (char*) VALID_SELECT_QUERY_STR);
  printf("%s\n", testQuery->retrieve->columnData[0]);
//*((*testQuery)->retrieve->ColumnDatas[0])

  // Clean up
  freeDatabase(&db);
  freeQueryStruct(&testQuery);
}

// /*
//  * Function:  TestQuery::testConnectSuccess
//  * ------------------
//  * Connects to a database with a valid connection string.
//  *
//  */
// void TestQuery::testSelectQueryFail(void) {
//
//   // Connect to the database.
//   database* db = NULL;
//   state s = connect(&db, (char*) VALID_CONN_STR);
//
//   // Clean up
//   freeDatabase(&db);
//
//   // Ensure we had a successful connection.
//   CPPUNIT_ASSERT_MESSAGE("Couldn't connect successfully to the database", s == SUCCESS || s == SUCCESS_WITH_INFO);
// }
//
// /*
//  * Function:  TestQuery::testConnectSuccessMultiple
//  * ------------------
//  * Connects to a database with a valid connection string, multiple times.
//  *
//  */
// void TestQuery::testUpdateQuerySuccess(void) {
//
//   int i = 0;
//   for (; i < 5; i++)
//     testConnectSuccessAndSuccessWithInfo();
//
// }
//
// /*
//  * Function:  TestQuery::testConnectSuccessMultiple
//  * ------------------
//  * Connects to a database with a valid connection string, multiple times.
//  *
//  */
// void TestQuery::testUpdateQueryFail(void) {
//
//   int i = 0;
//   for (; i < 5; i++)
//     testConnectSuccessAndSuccessWithInfo();
//
// }
//
// /*
//  * Function:  TestQuery::testConnectCatastrophicFailure
//  * ------------------
//  * Fake a catastrophic failure to see our recovery.
//  *
//  */
// void TestQuery::testOtherQuerySuccess(void) {
//   // Nothing here for now.
//   CPPUNIT_ASSERT_MESSAGE("Stub", 1);
// }
//
// /*
//  * Function:  TestQuery::testConnectCatastrophicFailure
//  * ------------------
//  * Fake a catastrophic failure to see our recovery.
//  *
//  */
// void TestQuery::testOtherQueryFail(void) {
//   // Nothing here for now.
//   CPPUNIT_ASSERT_MESSAGE("Stub", 1);
// }
