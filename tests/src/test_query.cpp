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

  CPPUNIT_ASSERT_MESSAGE("Can't connect to database", s == SUCCESS || s == SUCCESS_WITH_INFO);

  CPPUNIT_ASSERT_MESSAGE("Query failed to execute", d == SUCCESS || d == SUCCESS_WITH_INFO);
  
  // Clean up
  freeDatabase(&db);
  freeQueryStruct(&testQuery);
}



void TestQuery::testUpdateQuerySuccess(void) {

  // Connect to the database.
  database* db = NULL;
  state s = connect(&db, (char*) VALID_CONN_STR);
  queryStruct* testQuery = NULL;
  CPPUNIT_ASSERT_MESSAGE("Can't connect to database", s == SUCCESS || s == SUCCESS_WITH_INFO);
  state d = query(db, &testQuery, (char*) VALID_INSERT_QUERY_STR);

  CPPUNIT_ASSERT_MESSAGE("Query Failed", d == SUCCESS || d == SUCCESS_WITH_INFO);
  CPPUNIT_ASSERT_MESSAGE("Failed to insert value", testQuery->rowCountPtr == 1);

  // Clean up
  freeDatabase(&db);
  freeQueryStruct(&testQuery);
}


void TestQuery::testQueryFail(void) {

  // Connect to the database.
  database* db = NULL;
  state s = connect(&db, (char*) VALID_CONN_STR);
  queryStruct* testQuery = NULL;

  state d = query(db, &testQuery, (char*) INVALID_INSERT_QUERY_STR);
  CPPUNIT_ASSERT_MESSAGE("Can't connect to database", s == SUCCESS || s == SUCCESS_WITH_INFO);
  CPPUNIT_ASSERT_MESSAGE("Query passed", d == QUERY_EXECUTION_FAILURE);

  // Clean up
  freeDatabase(&db);
  freeQueryStruct(&testQuery);
}
