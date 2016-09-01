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


/*
 * Function:  TestQuery::testResultQuerySuccess
 * ------------------
 * Connects to a database, queries the database with a select statement
 * and disconnects from the database.
 *
 */
void TestQuery::testResultQuerySuccess(void) {

  // Connect to the database.
  database* db = NULL;
  state s = connect(&db, (char*) VALID_CONN_STR);
  CPPUNIT_ASSERT_MESSAGE("Can't connect to database.", s == SUCCESS || s == SUCCESS_WITH_INFO);

  // Query the database.
  queryStruct* testQuery = NULL;
  s = query(db, &testQuery, (char*) VALID_SELECT_QUERY_STR);
  CPPUNIT_ASSERT_MESSAGE("Query failed to execute.", s == SUCCESS || s == SUCCESS_WITH_INFO);
  
  // Disconnect.
  s = disconnect(&db);
  CPPUNIT_ASSERT_MESSAGE("Can't disconnect from database.", s == SUCCESS || s == SUCCESS_WITH_INFO);

  // Free the query struct, since we're done with it.
  freeQueryStruct(&testQuery);

}



void TestQuery::testInfoQuerySuccess(void) {

  // Connect to the database.
  database* db = NULL;
  state s = connect(&db, (char*) VALID_CONN_STR);
  CPPUNIT_ASSERT_MESSAGE("Can't connect to database.", s == SUCCESS || s == SUCCESS_WITH_INFO);

  // Query the database.
  queryStruct* testQuery = NULL;
  s = query(db, &testQuery, (char*) VALID_INSERT_QUERY_STR);
  CPPUNIT_ASSERT_MESSAGE("Query Failed.", s == SUCCESS || s == SUCCESS_WITH_INFO);
  CPPUNIT_ASSERT_MESSAGE("Failed to insert value.", testQuery->rowCountPtr == 1);

  // Disconnect.
  s = disconnect(&db);
  CPPUNIT_ASSERT_MESSAGE("Can't disconnect from database.", s == SUCCESS || s == SUCCESS_WITH_INFO);

  // Free the query struct, since we're done with it.
  freeQueryStruct(&testQuery);

}


void TestQuery::testInfoQueryFail(void) {

  // Connect to the database.
  database* db = NULL;
  state s = connect(&db, (char*) VALID_CONN_STR);
  CPPUNIT_ASSERT_MESSAGE("Can't connect to database.", s == SUCCESS || s == SUCCESS_WITH_INFO);

  // Query the database.
  queryStruct* testQuery = NULL;
  s = query(db, &testQuery, (char*) INVALID_INSERT_QUERY_STR);
  CPPUNIT_ASSERT_MESSAGE("Query passed", s == QUERY_EXECUTION_FAILURE);

  // Disconnect
  s = disconnect(&db);
  CPPUNIT_ASSERT_MESSAGE("Can't disconnect from database.", s == SUCCESS || s == SUCCESS_WITH_INFO);

  // Free the query struct, since we're done with it.
  freeQueryStruct(&testQuery);

}
