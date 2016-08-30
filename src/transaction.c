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

#include <stdio.h>
#include <stdbool.h>
#include "transaction.h"


/*
 * Function:  beginTrans
 * ------------------
 * Begins the transaction
 *
 * db: The database with allocated handles
 *
 * returns:
 *   SUCCESS: There is a connection to the database.
 *   NO_DATABASE_FOUND: There was no allocated database
 *   SET_CONNECTION_ATTR_FAIL: Failed to switch to manual commit mode
 */

state beginTrans(database** db) {
  SQLRETURN retCode = SQL_SUCCESS;
  bool haveInfo = false;

  // Check to make sure the database exists first
  if((*db) == NULL)
    return NO_DATABASE_FOUND;

  // Set the connection attribute to manual commit
  retCode = SQLSetConnectAttr((*db)->hnd->hDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)false, 0);
  if (retCode != SQL_SUCCESS) {
    generateDatabaseError((*db)->err, (*db)->hnd->hDbc, SQL_HANDLE_DBC);
    if (retCode == SQL_SUCCESS_WITH_INFO) {
      haveInfo = true;
    } else {
      return SET_CONNECTION_ATTR_FAIL;
    }
  }

  return haveInfo ? SUCCESS_WITH_INFO : SUCCESS;
}


/*
 * Function:  commitTrans
 * ------------------
 * Commits the transaction
 *
 * db: The database with allocated handles
 *
 * returns:
 *   SUCCESS: There is a connection to the database.
 *   NO_DATABASE_FOUND: There was no allocated database
 *   SET_CONNECTION_ATTR_FAIL: Failed to switch to manual commit mode
 *   COMMIT_TRANSACTION_FAILURE: Failed to commit the transaction
 */

state commitTrans(database** db) {
  SQLRETURN retCode = SQL_SUCCESS;
  bool haveInfo = false;

  // Check to make sure the database exists first
  if((*db) == NULL)
    return NO_DATABASE_FOUND;

  // End the transaction by commit
  retCode = SQLEndTran(SQL_HANDLE_DBC, (*db)->hnd->hDbc, SQL_COMMIT);
  if (retCode != SQL_SUCCESS) {
    generateDatabaseError((*db)->err, (*db)->hnd->hDbc, SQL_HANDLE_DBC);
    if (retCode == SQL_SUCCESS_WITH_INFO) {
      haveInfo = true;
    } else {
      return COMMIT_TRANSACTION_FAILURE;
    }
  }
  // Set the connection attribute to auto commit
  retCode = SQLSetConnectAttr((*db)->hnd->hDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)true, 0);
  if (retCode != SQL_SUCCESS) {
    generateDatabaseError((*db)->err, (*db)->hnd->hDbc, SQL_HANDLE_DBC);
    if (retCode == SQL_SUCCESS_WITH_INFO) {
      haveInfo = true;
    } else {
      return SET_CONNECTION_ATTR_FAIL;
    }
  }

  return haveInfo ? SUCCESS_WITH_INFO : SUCCESS;

}


/*
 * Function:  rollbackTrans
 * ------------------
 * Rollsback any changes made during the transaction
 *
 * db: The database with allocated handles
 *
 * returns:
 *   SUCCESS: There is a connection to the database.
 *   NO_DATABASE_FOUND: There was no allocated database
 *   SET_CONNECTION_ATTR_FAIL: Failed to switch to manual commit mode
 *   ROLLBACK_TRANSACTION_FAILURE: Failed to rollback the transaction
 */

state rollbackTrans(database** db) {

  SQLRETURN retCode = SQL_SUCCESS;
  bool haveInfo = false;
  // Check to make sure the database exists first
  if((*db) == NULL)
    return NO_DATABASE_FOUND;

  // End the transaction by rollback
  retCode = SQLEndTran(SQL_HANDLE_DBC, (*db)->hnd->hDbc, SQL_ROLLBACK);
  if (retCode != SQL_SUCCESS) {
    generateDatabaseError((*db)->err, (*db)->hnd->hDbc, SQL_HANDLE_DBC);
    if (retCode == SQL_SUCCESS_WITH_INFO) {
      haveInfo = true;
    } else {
      return ROLLBACK_TRANSACTION_FAILURE;
    }
  }

  // Set the connection attribute to auto commit
  retCode = SQLSetConnectAttr((*db)->hnd->hDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)true, 0);
  if (retCode != SQL_SUCCESS) {
    generateDatabaseError((*db)->err, (*db)->hnd->hDbc, SQL_HANDLE_DBC);
    if (retCode == SQL_SUCCESS_WITH_INFO) {
      haveInfo = true;
    } else {
      return SET_CONNECTION_ATTR_FAIL;
    }
  }

  return haveInfo ? SUCCESS_WITH_INFO : SUCCESS;
}
