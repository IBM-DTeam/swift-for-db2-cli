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

#include "test_connect.hpp"


/*
 * Function:  TestConnect::testValidConnection
 * ------------------
 * Connects to a database with a valid connection string.
 *
 */
void TestConnect::testValidConnection(void) {
  handles* connection = connect((char *) VALID_CONN_STR);

  // Ensure nothing is NULL where it shouldn't be.
  CPPUNIT_ASSERT_MESSAGE("Handles struct is NULL", connection != NULL);
  CPPUNIT_ASSERT_MESSAGE("Handles struct hEnv was SQL_NULL_HENV", connection->hEnv != SQL_NULL_HENV);
  CPPUNIT_ASSERT_MESSAGE("Handles struct hDbc was SQL_NULL_HDBC", connection->hDbc != SQL_NULL_HDBC);

  SQLRETURN retCode = SQL_SUCCESS;
  SQLINTEGER connectionState = SQL_CD_TRUE;

  // Check if the database connection is dead.
  retCode = SQLGetConnectAttr(connection->hDbc,
                              SQL_ATTR_CONNECTION_DEAD,
                              (SQLPOINTER) &connectionState,
                              (SQLINTEGER) sizeof(connectionState),
                              NULL);

  CPPUNIT_ASSERT_MESSAGE("SQLGetConnectAttr cannot check if the connection is dead", retCode == SQL_SUCCESS);
  CPPUNIT_ASSERT_MESSAGE("Database connection is dead before disconnecting manually", retCode == SQL_CD_FALSE);

  // TODO: Disconnect & free object.
}

/*
 * Function:  TestConnect::testValidConnectionMultiple
 * ------------------
 * Connects to a database with a valid connection string multiple times.
 *
 */
void TestConnect::testValidConnectionMultiple(void) {
  int i = 0;
  for (; i < 10; i++) {
    testValidConnection();
  }
}

/*
 * Function:  TestConnect::testInvalidConnection
 * ------------------
 * Connects to a database with an invalid connection string.
 *
 */
void TestConnect::testInvalidConnection(void) {
  handles* connection = connect((char*) INVALID_CONN_STR);

  // TODO: Disconnect the connection if not NULL and free.
  CPPUNIT_ASSERT_MESSAGE("We have a connection on an invalid connection string", connection == NULL);

}

/*
 * Function:  TestConnect::testInvalidConnectionMultiple
 * ------------------
 * Connects to a database with an invalid connection string multiple times.
 *
 */
void TestConnect::testInvalidConnectionMultiple(void) {
  int i = 0;
  for (; i < 10; i++) {
    testInvalidConnection();
  }
}

/*
 * Function:  TestConnect::testMixedConnections
 * ------------------
 * Connects to a database with a mix of valid and invalid connection string
 * multiple times.
 *
 */
void TestConnect::testMixedConnections(void) {
  int i = 0;
  for (; i < 10; i++) {
    if (i % 2 == 0)
      testValidConnection();
    else
      testInvalidConnection();
  }
}
