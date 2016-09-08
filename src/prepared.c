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
#include "prepared.h"



/*
 * Function:  prepare
 * ------------------
 * Querys the database
 *
 * db: Where the database struct will be setup.
 * hStmt: The statement handle pointer
 * query: The string of the actual query
 *
 * returns:
 *   SUCCESS: The database struct was setup without issues.
 *   NO_DATABASE_FOUND: Database does not exist
 *   STATEMENT_HANDLE_EXISTS: The provided handle is already allocated
 *   QUERY_FAILURE: Execution of query failed
 *   STMT_HANDLE_SETUP_FAILURE: failed to properly set up the statement handle
 *
 */
state prepare(database *db, queryStruct **hStmtStruct, char *query, char** values) {
  SQLRETURN retCode = SQL_SUCCESS;
  bool haveInfo = false;
  int valueSize = sizeof(values)/ sizeof(char**);
  SQLULEN FIRSTNAME_LEN = 0;
  SQLLEN lenFirstName = NULL;

  retCode = SQLPrepare((*hStmtStruct)->hStmts, (SQLCHAR *)query, strlen(query));
  if (retCode != SQL_SUCCESS) {
    generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
    if (retCode == SQL_SUCCESS_WITH_INFO) {
      haveInfo = true;
    } else {
      return SQL_PREPARE_FAILURE;
    }
  }

  for(int i =0 ; i < valueSize; i++){
    retCode = SQLBindParameter(
      (*hStmtStruct)->hStmts,
      i +1,
      SQL_PARAM_INPUT,
      SQL_C_CHAR,
      SQL_CHAR,
      FIRSTNAME_LEN,
      0,
      values[i],
      strlen(values[i]),
      &lenFirstName);

    if (retCode != SQL_SUCCESS) {
      generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
      if (retCode == SQL_SUCCESS_WITH_INFO) {
        haveInfo = true;
      } else {
        return PARAMETER_BIND_FAILURE;
      }
    }

  }
  retCode = SQLExecute((*hStmtStruct)->hStmts);
  if (retCode != SQL_SUCCESS) {
    generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
    if (retCode == SQL_SUCCESS_WITH_INFO) {
      haveInfo = true;
    } else {
      return QUERY_EXECUTION_FAILURE;
    }
  }
  return haveInfo ? SUCCESS_WITH_INFO : SUCCESS;

}
