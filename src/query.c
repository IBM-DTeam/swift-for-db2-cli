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
#include "query.h"


#define numColumns_NAME_LEN 256


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
  int numColumns = 0;
  bool haveInfo = false;
  SQLRETURN retCode = SQL_SUCCESS;

  // Check if database exists
  if (db == NULL)
    return NO_DATABASE_FOUND;

  // Check for existing struct
  if(*hStmtStruct != NULL)
    return HANDLE_STATEMENT_EXISTS;

  *hStmtStruct = (queryStruct*) malloc(sizeof(queryStruct));

  if(*hStmtStruct == NULL)
    return MALLOC_FAILURE;

  // Allocate handle for STATEMENT_HANDLE
  (*hStmtStruct)->hStmts = SQL_NULL_HSTMT;
  (*hStmtStruct)->rowCountPtr = NULL;
  (*hStmtStruct)->queryType = NULL;
  (*hStmtStruct)->retrieve = NULL;

  retCode = SQLAllocHandle(SQL_HANDLE_STMT, db->hnd->hDbc, &((*hStmtStruct)->hStmts));

  if(retCode != SQL_SUCCESS){
    generateDatabaseError(db->err, db->hnd);
    if (retCode == SQL_SUCCESS_WITH_INFO) {
      haveInfo = true;
    } else {
      // GC: Free what you malloc'd
      return STMT_HANDLE_SETUP_FAILURE;
    }
  }

  // Directly execute ad hoc queries
  retCode = SQLExecDirect((*hStmtStruct)->hStmts, (SQLCHAR*) query, strlen(query));

  //SQLExecDirect Worked
  if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO){
    retCode = SQLNumResultCols((*hStmtStruct)->hStmts, (SQLSMALLINT*) &numColumns);
    //SQLNumResultCols Didn't work and we don't have a SELECT statement
    if(numColumns == 0){
      retCode = SQLRowCount((*hStmtStruct)->hStmts, &((*hStmtStruct)->rowCountPtr));
      if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO){
        //It was an other query. i.e chreate drop etc.
        if((*hStmtStruct)->rowCountPtr == -1){
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

      //The Query was a SELECT or VALUES Statement
      // Initialize and malloc retrieve variables
      (*hStmtStruct)->retrieve->sNumColResults = numColumns;
      (*hStmtStruct)->retrieve->rowCount = 0;
      (*hStmtStruct)->retrieve->columnName = (SQLCHAR **) malloc(numColumns * sizeof(SQLCHAR*));
      (*hStmtStruct)->retrieve->columnData = (SQLCHAR **) malloc(numColumns * sizeof(SQLCHAR*));
      (*hStmtStruct)->retrieve->columnDataType = (SQLSMALLINT **) malloc(numColumns * sizeof(SQLSMALLINT*));


      // Set up SQLDescribeCol and SQLBindCol variables
      SQLSMALLINT    columnNameLen[numColumns];
      SQLULEN        columnDataSize[numColumns];
      SQLSMALLINT    columnDataDigits[numColumns];
      SQLSMALLINT    columnDataNullable[numColumns];
      SQLLEN         columnDataLen[numColumns];

      //Initialize things for malloc
      for (int i=0;i<numColumns;i++) {
         (*hStmtStruct)->retrieve->columnName[i] = (SQLCHAR*) malloc(numColumns_NAME_LEN * sizeof(SQLCHAR));
         *((*hStmtStruct)->retrieve->columnName[i])=NULL;

         (*hStmtStruct)->retrieve->columnData[i] = (SQLCHAR*) malloc(numColumns_NAME_LEN * sizeof(columnDataSize[i]+1));
         *((*hStmtStruct)->retrieve->columnData[i]) = NULL;

         (*hStmtStruct)->retrieve->columnDataType[i] = (SQLSMALLINT *) malloc(numColumns_NAME_LEN * sizeof(SQLSMALLINT*));
         *((*hStmtStruct)->retrieve->columnDataType[i]) = NULL;

      }
      // Get info about each column
      for(int i = 0; i < numColumns; i++){
        // Malloc space for names

        *((*hStmtStruct)->retrieve->columnName[i]) = NULL;
        // Get SQLDescribeCol info
        retCode = SQLDescribeCol(
          (*hStmtStruct)->hStmts,
          i,
          (*hStmtStruct)->retrieve->columnName[i],
          numColumns_NAME_LEN,
          &columnNameLen[i],
          (*hStmtStruct)->retrieve->columnDataType[i],
          &columnDataSize[i],
          &columnDataDigits[i],
          &columnDataNullable[i]);
        if(retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO){
          }


        // Get SQLBindCol info
        retCode = SQLBindCol(
          (*hStmtStruct)->hStmts,
          i,
          *((*hStmtStruct)->retrieve->columnDataType[i]),
          (*hStmtStruct)->retrieve->columnData[i],
          columnDataSize[i],
          &columnDataLen[i]);
    }

    //Get actual data until no more data is returned
    for(int i = 0;;i++){
      retCode = SQLFetch((*hStmtStruct)->hStmts);
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
  for(int i = 0; i < (*hStmtStruct)->retrieve->sNumColResults; i++){
  printf("%hhu\n", *((*hStmtStruct)->retrieve->columnData[i]) );
}
  return haveInfo ? SUCCESS_WITH_INFO : SUCCESS;
}else{
  // GC: free what you malloc'd
  // return somehting like CANNOT_QUERY
  return CATASTROPHIC_FAILURE;
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

  // Check if Column names has been made
  if((*hStmtStruct)->retrieve->columnName == NULL)
    return;

  // Loop through the column names and free all names
  // Check Column names actually exist
  for(int i = 0; i < (*hStmtStruct)->retrieve->sNumColResults; i++ ){
    if((*hStmtStruct)->retrieve->columnName[i] == NULL)
      return;
    if((*hStmtStruct)->retrieve->columnData[i] == NULL)
      return;

    if((*hStmtStruct)->retrieve->columnDataType[i] == NULL)
      return;

    free((*hStmtStruct)->retrieve->columnName[i]);
    free((*hStmtStruct)->retrieve->columnData[i]);
    free((*hStmtStruct)->retrieve->columnDataType[i]);

  }
  // Free ColumnNames after freeing each column name
  free((*hStmtStruct)->retrieve->columnName);

  // Free the data collected
  free((*hStmtStruct)->retrieve->columnData);

  // Free DataTypes
  free((*hStmtStruct)->retrieve->columnDataType);

  // Free the inner retrieve struct
  free((*hStmtStruct)->retrieve);

  // Free the final statement struct
  free(*hStmtStruct );

}
