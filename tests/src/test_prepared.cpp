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
  const char *query = "INSERT INTO MYTABLE VALUES(?,?,?);";
  char** valList = (char**) malloc(sizeof(char*) * 3);
  valList[0] = (char *)"1";
  valList[1] = (char *)"howare";
  valList[2] = (char *)"test";

  state s = connect(&db, (char*) VALID_CONN_STR);

  if (s == SETUP_DATABASE_FAILURE)
    freeDatabase(&db);

  queryStruct* testQuery = NULL;
  CPPUNIT_ASSERT_MESSAGE("Can't connect to database.", s == SUCCESS || s == SUCCESS_WITH_INFO);
  state a = prepare(db, &testQuery, query, valList);
  state b = executePrepared(db, &testQuery);

  freeQueryStruct(&testQuery);
  // Disconnect.
  CPPUNIT_ASSERT_MESSAGE("Can't prepare.", a == SUCCESS || a == SUCCESS_WITH_INFO);
  CPPUNIT_ASSERT_MESSAGE("Can't executePrepare.", b == SUCCESS || b == SUCCESS_WITH_INFO);
  s = disconnect(&db);
  free(valList);
  CPPUNIT_ASSERT_MESSAGE("Can't disconnect from database.", s == SUCCESS);


}


/*
 * Function:  TestQuery::testResultQuerySuccessDecimal
 * ------------------
 * Connects to a database, queries the database with a select statement
 * and disconnects from the database.
 *
 */
void TestPrepared::testPreparedSuccessDecimal(void) {

  // Connect to the database.
  database* db = NULL;
  const char *query = "INSERT INTO MYTABLE3 VALUES(?,?,?);";
  char** valList = (char**) malloc(sizeof(char*) * 3);
  valList[0] = (char *)"1";
  valList[1] = (char *)"howa";
  valList[2] = (char *)"2.533";

  state s = connect(&db, (char*) VALID_CONN_STR);

  if (s == SETUP_DATABASE_FAILURE)
    freeDatabase(&db);

  queryStruct* testQuery = NULL;
  CPPUNIT_ASSERT_MESSAGE("Can't connect to database.", s == SUCCESS || s == SUCCESS_WITH_INFO);
  state a = prepare(db, &testQuery, query, valList);
  state b = executePrepared(db, &testQuery);

  freeQueryStruct(&testQuery);
  // Disconnect.
  CPPUNIT_ASSERT_MESSAGE("Can't prepare.", a == SUCCESS || a == SUCCESS_WITH_INFO);
  CPPUNIT_ASSERT_MESSAGE("Can't executePrepare.", b == SUCCESS || b == SUCCESS_WITH_INFO);
  s = disconnect(&db);
  free(valList);
  CPPUNIT_ASSERT_MESSAGE("Can't disconnect from database.", s == SUCCESS);


}


/*
 * Function:  TestQuery::testResultQuerySuccessDecimal
 * ------------------
 * Connects to a database, queries the database with a select statement
 * and disconnects from the database.
 *
 */
void TestPrepared::testPreparedFailure(void) {

  // Connect to the database.
  database* db = NULL;
  const char *query = "INSERT INTO MYTABLE3 VALUES(?,?,?);";
  char** valList = (char**) malloc(sizeof(char*) * 3);
  valList[0] = (char *)"1";
  valList[1] = (char *)"howsdadsdsdsdsddsdsdsdsdsdsdasdsaasda";
  valList[2] = (char *)"20";

  state s = connect(&db, (char*) VALID_CONN_STR);

  if (s == SETUP_DATABASE_FAILURE)
    freeDatabase(&db);

  queryStruct* testQuery = NULL;
  CPPUNIT_ASSERT_MESSAGE("Can't connect to database.", s == SUCCESS || s == SUCCESS_WITH_INFO);
  state a = prepare(db, &testQuery, query, valList);
  state b = executePrepared(db, &testQuery);

  // Disconnect.
  CPPUNIT_ASSERT_MESSAGE("Can't prepare.", a == SUCCESS || a == SUCCESS_WITH_INFO);
  CPPUNIT_ASSERT_MESSAGE("Can executePrepare.", b != SUCCESS && b != SUCCESS_WITH_INFO);


  freeQueryStruct(&testQuery);

  s = disconnect(&db);
  free(valList);
  CPPUNIT_ASSERT_MESSAGE("Can't disconnect from database.", s == SUCCESS);


}



/*
 * Function:  TestQuery::testResultQuerySuccessDecimal
 * ------------------
 * Connects to a database, queries the database with a select statement
 * and disconnects from the database.
 *
 */
void TestPrepared::testPreparedSelect(void) {

  // Connect to the database.
  database* db = NULL;
  const char *query = "SELECT * FROM MYTABLE WHERE COL1 = ?";
  char** valList = (char**) malloc(sizeof(char*) * 1);
  valList[0] = (char *)"1";

  state s = connect(&db, (char*) VALID_CONN_STR);

  if (s == SETUP_DATABASE_FAILURE)
    freeDatabase(&db);

  queryStruct* testQuery = NULL;
  CPPUNIT_ASSERT_MESSAGE("Can't connect to database.", s == SUCCESS || s == SUCCESS_WITH_INFO);
  state a = prepare(db, &testQuery, query, valList);
  state b = executePrepared(db, &testQuery);


  // Disconnect.
  CPPUNIT_ASSERT_MESSAGE("Can't prepare.", a == SUCCESS || a == SUCCESS_WITH_INFO);
  CPPUNIT_ASSERT_MESSAGE("Can't get results", b == SUCCESS || b == SUCCESS_WITH_INFO);

  CPPUNIT_ASSERT_MESSAGE("Didn't have at least one column.", testQuery->retrieve->sNumColResults > 0);
  freeQueryStruct(&testQuery);
  s = disconnect(&db);
  free(valList);
  CPPUNIT_ASSERT_MESSAGE("Can't disconnect from database.", s == SUCCESS);


}
