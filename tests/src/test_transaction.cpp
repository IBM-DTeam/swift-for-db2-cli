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
 * Function:  TestQuery::testCommitSuccess
 * ------------------
 * Connects to a database, switches to transaction and commits succuessfully
 *
 */

void TestTransaction::testCommitSuccess(void) {

  database* db = NULL;
  state s = connect(&db, (char*) VALID_CONN_STR);
  if (s == SETUP_DATABASE_FAILURE)
    freeDatabase(&db);

  CPPUNIT_ASSERT_MESSAGE("Can't connect to database.", s == SUCCESS || s == SUCCESS_WITH_INFO);

  state b = beginTrans(&db);
  CPPUNIT_ASSERT_MESSAGE("Can't being the transaction.", b == SUCCESS || b == SUCCESS_WITH_INFO);

  state p = commitTrans(&db);

  CPPUNIT_ASSERT_MESSAGE("Couldn't commit the transaction.", p == SUCCESS || p == SUCCESS_WITH_INFO);
  // Disconnect.
  s = disconnect(&db);
  CPPUNIT_ASSERT_MESSAGE("Can't disconnect from database.", s == SUCCESS);


}

/*
 * Function:  TestQuery::testRollbackSuccess
 * ------------------
 * Connects to a database, switches to transaction and rollsback succuessfully
 *
 */
void TestTransaction::testRollbackSuccess(void) {

  database* db = NULL;
  state s = connect(&db, (char*) VALID_CONN_STR);
  if (s == SETUP_DATABASE_FAILURE)
    freeDatabase(&db);

  CPPUNIT_ASSERT_MESSAGE("Can't connect to database.", s == SUCCESS || s == SUCCESS_WITH_INFO);

  state b = beginTrans(&db);
  CPPUNIT_ASSERT_MESSAGE("Can't being the transaction.", b == SUCCESS || b == SUCCESS_WITH_INFO);

  state p = rollbackTrans(&db);

  CPPUNIT_ASSERT_MESSAGE("Couldn't rollback the transaction.", p == SUCCESS || p == SUCCESS_WITH_INFO);
  // Disconnect.
  s = disconnect(&db);
  CPPUNIT_ASSERT_MESSAGE("Can't disconnect from database.", s == SUCCESS);


}

/*
 * Function:  TestQuery::testBeginTransFailure
 * ------------------
 * Connects to a database, and fails to switch to transaction
 *
 */

void TestTransaction::testBeginTransFailure(void) {

  database* db = NULL;
  createDatabase(&db);

  state b = beginTrans(&db);
  freeDatabase(&db);
  CPPUNIT_ASSERT_MESSAGE("Connected successfully.", b ==SET_CONNECTION_ATTR_FAIL);

}


/*
 * Function:  TestQuery::testCommitFailure
 * ------------------
 * Connects to a database, switches to transaction and commits unsuccessfully
 *
 */

void TestTransaction::testCommitFailure(void) {

  database* db = NULL;
  state s = connect(&db, (char*) VALID_CONN_STR);
  if (s == SETUP_DATABASE_FAILURE)
    freeDatabase(&db);

  CPPUNIT_ASSERT_MESSAGE("Can't connect to database.", s == SUCCESS || s == SUCCESS_WITH_INFO);

  state b = beginTrans(&db);
  CPPUNIT_ASSERT_MESSAGE("Can't being the transaction.", b == SUCCESS || b == SUCCESS_WITH_INFO);

  (db)->hnd->hDbc = 0;
  state p = commitTrans(&db);

  CPPUNIT_ASSERT_MESSAGE("Commited the transaction", p = COMMIT_TRANSACTION_FAILURE);
  // Disconnect.
  freeDatabase(&db);


}


/*
 * Function:  TestQuery::testrollBackFailure
 * ------------------
 * Connects to a database, switches to transaction and rollsback unsuccuessfully
 *
 */

void TestTransaction::testRollbackFailure(void) {

  database* db = NULL;
  state s = connect(&db, (char*) VALID_CONN_STR);
  if (s == SETUP_DATABASE_FAILURE)
    freeDatabase(&db);

  CPPUNIT_ASSERT_MESSAGE("Can't connect to database.", s == SUCCESS || s == SUCCESS_WITH_INFO);

  state b = beginTrans(&db);
  CPPUNIT_ASSERT_MESSAGE("Can't being the transaction.", b == SUCCESS || b == SUCCESS_WITH_INFO);

  (db)->hnd->hDbc = 0;
  state p = rollbackTrans(&db);

  CPPUNIT_ASSERT_MESSAGE("rollbacked the transaction", p = ROLLBACK_TRANSACTION_FAILURE);
  // Disconnect.
  freeDatabase(&db);


}
