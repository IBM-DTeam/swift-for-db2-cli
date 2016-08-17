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

 #include "query.h"


 /*
  * Function:  query
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
state query(database* db, stmtHandle** hStmtStruct, char* query){
  bool haveInfo = false;
  SQLRETURN retCode = SQL_SUCCESS;
  *hStmtStruct = (stmtHandle*) malloc(sizeof(stmtHandle));

  if(*hStmtStruct== NULL)
    return MALLOC_FAILURE;

  // Check if database exists
  if (db == NULL)
    return NO_DATABASE_FOUND;

  // Check to make sure statement handle doesn't exist
  if (hStmtStruct != NULL)
    return STATEMENT_HANDLE_EXISTS;

  // Allocate handle for STATEMENT_HANDLE
  retCode = SQLAllocHandle(SQL_HANDLE_STMT, db->hnd->hDbc, (SQLHANDLE*) (*hStmtStruct)->hStmts);
  if(retCode != SQL_SUCCESS){
    if (retCode == SQL_SUCCESS_WITH_INFO) {
      haveInfo = true;
    } else {
      return STMT_HANDLE_SETUP_FAILURE;
    }
  }

 // Directly execute ad hoc queries
  retCode = SQLExecDirect(*(*hStmtStruct)->hStmts, (SQLCHAR*) query, strlen(query));

  if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO){
    SQLNumResultCols(*(*hStmtStruct)->hStmts, ((*hStmtStruct)->sNumResults));

    if(*((*hStmtStruct)->sNumResults) > 0){

    }

  }
  if(retCode != SQL_SUCCESS){
    if (retCode == SQL_SUCCESS_WITH_INFO) {
      haveInfo = true;
    } else {
      return QUERY_FAILURE;
    }
  }
  return haveInfo ? SUCCESS_WITH_INFO : SUCCESS;
 }
