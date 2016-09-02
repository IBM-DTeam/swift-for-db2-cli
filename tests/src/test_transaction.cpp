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

#include "test_transaction.hpp"


/*
 * Function:  TestQuery::testResultQuerySuccess
 * ------------------
 * Connects to a database, queries the database with a select statement
 * and disconnects from the database.
 *
 */
void TestTransaction::testTransactionSuccess(void) {

  database* db = NULL;
  state s = connect(&db, (char*) VALID_CONN_STR);
  if (s == SETUP_DATABASE_FAILURE)
    freeDatabase(&db);

  CPPUNIT_ASSERT_MESSAGE("Can't connect to database.", s == SUCCESS || s == SUCCESS_WITH_INFO);

  state b = beginTrans(&db);
  CPPUNIT_ASSERT_MESSAGE("Can't being the transaction.", b == SUCCESS || b == SUCCESS_WITH_INFO);

  // Query the database.
  queryStruct* testQuery = NULL;
  queryStruct* testQuery2 = NULL;
  state v = query(db, &testQuery, (char*) VALID_SELECT_QUERY_STR);

  // Free the query struct, since we won't pass the test case.
  if (v != SUCCESS && v != SUCCESS_WITH_INFO)
    freeQueryStruct(&testQuery);

  state t = query(db, &testQuery2, (char*) VALID_INSERT_QUERY_STR);
  // Free the query struct, since we won't pass the test case.
  if (t != SUCCESS && s != SUCCESS_WITH_INFO)
    freeQueryStruct(&testQuery2);

  state p = commitTrans(&db);
  CPPUNIT_ASSERT_MESSAGE("Couldn't commit the transaction.", p == SUCCESS || p == SUCCESS_WITH_INFO);

  CPPUNIT_ASSERT_MESSAGE("Failed to insert value.", testQuery2->rowCountPtr == 1);
  CPPUNIT_ASSERT_MESSAGE("Didn't have at least one column.", testQuery->retrieve->sNumColResults > 0);

  freeQueryStruct(&testQuery);

  // Disconnect.
  s = disconnect(&db);
  CPPUNIT_ASSERT_MESSAGE("Can't disconnect from database.", s == SUCCESS);


}
