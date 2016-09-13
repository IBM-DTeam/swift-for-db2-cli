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

#include "test_prepared.hpp"


/*
 * Function:  TestQuery::testResultQuerySuccess
 * ------------------
 * Connects to a database, queries the database with a select statement
 * and disconnects from the database.
 *
 */
void TestPrepared::testPreparedSuccess(void) {

  // Connect to the database.
  database* db = NULL;
  const char *query = "INSERT INTO MYTABLES VALUES(?,?);";
  char** valList = (char**) malloc(sizeof(char*)* 40);
  valList[0] = (char *)"hi";
  valList[1] = (char *)"hell";

  state s = connect(&db, (char*) VALID_CONN_STR);

  if (s == SETUP_DATABASE_FAILURE)
    freeDatabase(&db);

  queryStruct* testQuery = NULL;
  CPPUNIT_ASSERT_MESSAGE("Can't connect to database.", s == SUCCESS || s == SUCCESS_WITH_INFO);
  state a = prepare(db, &testQuery, query, valList);
  freeQueryStruct(&testQuery);
  // Disconnect.
  CPPUNIT_ASSERT_MESSAGE("Can't preapre.", a == SUCCESS || a == SUCCESS_WITH_INFO);
  s = disconnect(&db);
  free(valList);
  CPPUNIT_ASSERT_MESSAGE("Can't disconnect from database.", s == SUCCESS);


}
