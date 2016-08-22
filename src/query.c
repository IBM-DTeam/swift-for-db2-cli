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
  int MAX_COL = 0;
  SQLSMALLINT tempColumncount;
  bool haveInfo = false;
  SQLRETURN retCode = SQL_SUCCESS;

  // Check for existing struct
  if(*hStmtStruct== NULL)
    return HANDLE_STATEMENT_EXISTS;

  *hStmtStruct = (queryStruct*) malloc(sizeof(query));

  if(*hStmtStruct == NULL)
    return MALLOC_FAILURE;

  // Check if database exists
  if (db == NULL)
    return NO_DATABASE_FOUND;

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
    retCode = SQLNumResultCols( *(*hStmtStruct)->hStmts, &tempColumncount);
    //SQLNumResultCols Didn't work and we don't have a SELECT statement
    if(tempColumncount == 0){
      retCode = SQLRowCount(*(*hStmtStruct)->hStmts, (*hStmtStruct)->RowCountPtr);
      if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO){
        //It was an other query. i.e chreate drop etc.
        if(*((*hStmtStruct)->RowCountPtr) == -1){
          return SUCCESS;
        }else{
          //It was an update query i.e update insert etc
          return SUCCESS;
        }

      }

    }else{
      // Select query, allocate appropriate structures
      (*hStmtStruct)->retrieve = (retrieveQuery*) malloc(sizeof(retrieveQuery));

      if((*hStmtStruct)->retrieve == NULL)
        return MALLOC_FAILURE;

      *((*hStmtStruct)->retrieve->sNumColResults) = tempColumncount;
      //The Query was a Select Statement
      // Initialize and malloc retrieve variables
      (*hStmtStruct)->retrieve->rowCount = 0;
      MAX_COL = *((*hStmtStruct)->retrieve->sNumColResults);
      (*hStmtStruct)->retrieve->ColumnNames = (SQLCHAR **) malloc(MAX_COL * sizeof(SQLCHAR*));
      (*hStmtStruct)->retrieve->ColumnDatas = (SQLCHAR **) malloc(MAX_COL * sizeof(SQLCHAR*));
      (*hStmtStruct)->retrieve->ColumnDataTypes = (SQLSMALLINT **) malloc(MAX_COL * sizeof(SQLSMALLINT*));
      // Set up SQLDescribeCol and SQLBindCol variables
      SQLSMALLINT    ColumnNameLen[MAX_COL];
      SQLULEN        ColumnDataSize[MAX_COL];
      SQLSMALLINT    ColumnDataDigits[MAX_COL];
      SQLSMALLINT    ColumnDataNullable[MAX_COL];
      SQLLEN         ColumnDataLen[MAX_COL];

      //Initialize things for malloc
      for (int i=0;i<MAX_COL;i++) {
         (*hStmtStruct)->retrieve->ColumnNames[i]=NULL;
         (*hStmtStruct)->retrieve->ColumnDatas[i]=NULL;
      }
      // Get info about each column
      for(int i = 0; i < MAX_COL; i++){
        // Malloc space for names
        (*hStmtStruct)->retrieve->ColumnNames[i] = (SQLCHAR*) malloc(MAX_COL_NAME_LEN * sizeof(SQLCHAR));
        // Get SQLDescribeCol info
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
        // Get SQLBindCol info
        retCode = SQLBindCol(
          *(*hStmtStruct)->hStmts,
          i,
          *(*hStmtStruct)->retrieve->ColumnDataTypes[i],
          (*hStmtStruct)->retrieve->ColumnDatas[i],
          ColumnDataSize[i],
          &ColumnDataLen[i]);
    }

    //Get actual data until no more data is returned
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
    //Free everything we malloced
    freeQueryStruct(hStmtStruct);
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

  // Check if the whole struct exists
  if (*hStmtStruct == NULL)
    return;

  // Check if the retrieve struct exists
  if((*hStmtStruct)->retrieve == NULL)
    return;

  // Check if the number of columns are recieved
  if((*hStmtStruct)->retrieve->sNumColResults == NULL)
    return;

  // Check if Column names has been made
  if((*hStmtStruct)->retrieve->ColumnNames == NULL)
    return;

  // Loop through the column names and free all names
  // Check Column names actually exist
  for(int i = 0; i < *((*hStmtStruct)->retrieve->sNumColResults); i++ ){
    if((*hStmtStruct)->retrieve->ColumnNames[i] == NULL)
      return;

    free((*hStmtStruct)->retrieve->ColumnNames[i]);

  }
  // Free ColumnNames after freeing each column name
  free((*hStmtStruct)->retrieve->ColumnNames);

  // Free the data collected
  free((*hStmtStruct)->retrieve->ColumnDatas);

  // Free DataTypes
  free((*hStmtStruct)->retrieve->ColumnDataTypes);

  // Free the inner retrieve struct
  free((*hStmtStruct)->retrieve);

  // Free the final statement struct
  free(*hStmtStruct );

}
