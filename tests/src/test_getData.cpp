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

#include "test_getData.hpp"



/*
 * Function:  TestQuery::testResultQuerySuccess
 * ------------------
 * Connects to a database, queries the database with a select statement
 * and disconnects from the database.
 *
 */
void TestGetData::testGetColumnAndRow(void) {

  // Connect to the database.
  database* db = NULL;
  state s = db_connect(&db, (char*) VALID_CONN_STR);

  if (s == SETUP_DATABASE_FAILURE)
    db_freeDatabase(&db);

  CPPUNIT_ASSERT_MESSAGE("Can't connect to database.", s == SUCCESS || s == SUCCESS_WITH_INFO);

  // Query the database.
  queryStruct* testQuery = NULL;
  s = db_query(db, &testQuery, (char*) VALID_SELECT_QUERY_STR);

  data * col = db_getColumn(testQuery, "COL2");
  int num = 0;
  while(col != NULL){
    col = db_getColumnNextRow(col);
    num++;

  }

  // Free the query struct, since we won't pass the test case.
  if (s != SUCCESS && s != SUCCESS_WITH_INFO)
    db_freeQueryStruct(&testQuery);

  CPPUNIT_ASSERT_MESSAGE("Incorrect number of Rows", testQuery->retrieve->rowCount == num);
  CPPUNIT_ASSERT_MESSAGE("Query failed to execute, when it should've.", s == SUCCESS || s == SUCCESS_WITH_INFO);
  CPPUNIT_ASSERT_MESSAGE("Didn't have at least one column.", testQuery->retrieve->sNumColResults > 0);

  // Free the query struct, since we're done with it.
  db_freeQueryStruct(&testQuery);

  // Disconnect.
  s = db_disconnect(&db);
  CPPUNIT_ASSERT_MESSAGE("Can't disconnect from database.", s == SUCCESS);

}


/*
 * Function:  TestQuery::testResultQuerySuccess
 * ------------------
 * Connects to a database, queries the database with a select statement
 * and disconnects from the database.
 *
 */
 void TestGetData::testGetNextRow(void) {

   // Connect to the database.
   database* db = NULL;
   state s = db_connect(&db, (char*) VALID_CONN_STR);

   if (s == SETUP_DATABASE_FAILURE)
     db_freeDatabase(&db);

   CPPUNIT_ASSERT_MESSAGE("Can't connect to database.", s == SUCCESS || s == SUCCESS_WITH_INFO);

   // Query the database.
   queryStruct* testQuery = NULL;
   s = db_query(db, &testQuery, (char*) VALID_SELECT_QUERY_STR);
   int rowNum = 0;
   for(int i = 0; i < testQuery->retrieve->rowCount; i++){
     for(int j =0; j< testQuery->retrieve->sNumColResults ; j++){

     }
     rowNum++;
     db_getNextRow(testQuery);



   }

   // Free the query struct, since we won't pass the test case.
   if (s != SUCCESS && s != SUCCESS_WITH_INFO)
     db_freeQueryStruct(&testQuery);


   CPPUNIT_ASSERT_MESSAGE("Query failed to execute, when it should've.", s == SUCCESS || s == SUCCESS_WITH_INFO);
   CPPUNIT_ASSERT_MESSAGE("Didn't have at least one column.", testQuery->retrieve->sNumColResults > 0);
   CPPUNIT_ASSERT_MESSAGE("Incorrect Rows", testQuery->retrieve->rowCount == rowNum);
   // Free the query struct, since we're done with it.
   db_freeQueryStruct(&testQuery);

   // Disconnect.
   s = db_disconnect(&db);
   CPPUNIT_ASSERT_MESSAGE("Can't disconnect from database.", s == SUCCESS);

 }
