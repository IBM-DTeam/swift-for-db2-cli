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
state query(database* db, queryStruct** hStmtStruct, char* query){
  SQLULEN *      ColumnSizePtr = NULL;
  SQLSMALLINT *  DecimalDigitsPtr = NULL;
  SQLSMALLINT    BufferLength = 0;
  SQLUSMALLINT   ColumnNumber = 0;
  SQLSMALLINT *  NameLengthPtr = NULL;
  SQLSMALLINT *  NullablePtr= NULL;


  SQLSMALLINT    TargetType = 0;
  SQLPOINTER     TargetValuePtr = NULL;
  SQLLEN         BufferLength2 = 0;
  SQLLEN *       StrLen_or_Ind = NULL;

  bool haveInfo = false;
  SQLRETURN retCode = SQL_SUCCESS;
  if(*hStmtStruct== NULL)
    return HANDLE_STATEMENT_EXISTS;

  *hStmtStruct = (queryStruct*) malloc(sizeof(query));

  if(*hStmtStruct == NULL)
    return MALLOC_FAILURE;

  (*hStmtStruct)->retrieve = (retrieveQuery*) malloc(sizeof(retrieveQuery));

  if((*hStmtStruct)->retrieve == NULL)
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
  //SQLExecDirect Worked
  if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO){
    SQLNumResultCols( *(*hStmtStruct)->hStmts, (*hStmtStruct)->retrieve->sNumColResults);
    //SQLNumResultCols Didn't work and we don't have a SELECT statement
    if(*((*hStmtStruct)->retrieve->sNumColResults) == 0){
      // UPDATE OR OTHER
      //SQLRowCount
    //The Query was a Select Statement
    }else{
      retCode = SQLDescribeCol( *(*hStmtStruct)->hStmts, ColumnNumber, (*hStmtStruct)->retrieve->ColumnName, BufferLength, NameLengthPtr, (*hStmtStruct)->retrieve->DataTypePtr, ColumnSizePtr, DecimalDigitsPtr, NullablePtr);
      if(retCode != SQL_SUCCESS || retCode != SQL_SUCCESS_WITH_INFO){

      //SQLDescribeCol worked
      }else{
        retCode = SQLBindCol(*(*hStmtStruct)->hStmts, ColumnNumber, TargetType, TargetValuePtr, BufferLength2, StrLen_or_Ind);


      }
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
