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
 #define MAX_COL_NAME_LEN 256

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
  int MAX_COL =0;

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
      (*hStmtStruct)->retrieve->rowCount = 0;
      MAX_COL = *((*hStmtStruct)->retrieve->sNumColResults);
      (*hStmtStruct)->retrieve->ColumnNames = (SQLCHAR **) malloc(MAX_COL * sizeof(SQLCHAR*));
      (*hStmtStruct)->retrieve->ColumnDatas = (SQLCHAR **) malloc(MAX_COL * sizeof(SQLCHAR*));
      (*hStmtStruct)->retrieve->ColumnDataTypes = (SQLSMALLINT **) malloc(MAX_COL * sizeof(SQLSMALLINT*));

      SQLSMALLINT    ColumnNameLen[MAX_COL];

      SQLULEN        ColumnDataSize[MAX_COL];
      SQLSMALLINT    ColumnDataDigits[MAX_COL];
      SQLSMALLINT    ColumnDataNullable[MAX_COL];
      SQLLEN         ColumnDataLen[MAX_COL];


      for (int i=0;i<MAX_COL;i++) {
         (*hStmtStruct)->retrieve->ColumnNames[i]=NULL;
         (*hStmtStruct)->retrieve->ColumnDatas[i]=NULL;
      }

      for(int i = 0; i < MAX_COL; i++){
        (*hStmtStruct)->retrieve->ColumnNames[i] = (SQLCHAR*) malloc(MAX_COL_NAME_LEN * sizeof(SQLCHAR));
        retCode = SQLDescribeCol(
          *(*hStmtStruct)->hStmts,
          i,
          (*hStmtStruct)->retrieve->ColumnNames[i],
          MAX_COL_NAME_LEN,
          &ColumnNameLen[i],
          (*hStmtStruct)->retrieve->ColumnDataTypes[i],
          &ColumnDataSize[i],
          &ColumnDataDigits[i],
          &ColumnDataNullable[i]);
        if(retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO){
          }

        retCode = SQLBindCol(
          *(*hStmtStruct)->hStmts,
          i,
          *(*hStmtStruct)->retrieve->ColumnDataTypes[i],
          (*hStmtStruct)->retrieve->ColumnDatas[i],
          ColumnDataSize[i],
          &ColumnDataLen[i]);
    }

    for(int i = 0;;i++){
      retCode = SQLFetch(*(*hStmtStruct)->hStmts);
       if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
         ((*hStmtStruct)->retrieve->rowCount)++;
       } else {
           if (retCode != SQL_NO_DATA) {

           } else {
               break;
           }
       }

    }

  }
  return haveInfo ? SUCCESS_WITH_INFO : SUCCESS;
 }

 return SUCCESS;
}


/*
 * Function:  freeQueryStruct
 * ------------------
 * Frees queryStruct with retrieve filled
 *
 */
void freeQueryStruct(queryStruct** hStmtStruct) {
  if (*hStmtStruct == NULL)
    return;

  if((*hStmtStruct)->retrieve == NULL)
    return;

  if((*hStmtStruct)->retrieve->sNumColResults == NULL)
    return;

  if((*hStmtStruct)->retrieve->ColumnNames == NULL)
    return;

  for(int i = 0; i < *((*hStmtStruct)->retrieve->sNumColResults); i++ ){
    if((*hStmtStruct)->retrieve->ColumnNames[i] == NULL)
      return;

    free((*hStmtStruct)->retrieve->ColumnNames[i]);

  }

  free((*hStmtStruct)->retrieve->ColumnNames);
  free((*hStmtStruct)->retrieve->ColumnDatas);
  free((*hStmtStruct)->retrieve->ColumnDataTypes);
  free((*hStmtStruct)->retrieve);
  free(*hStmtStruct );

}
