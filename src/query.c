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

#define MAX_COL_NAME 256

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
state query(database *db, queryStruct **hStmtStruct, char *query) {
  int numColumns = 0;
  bool haveInfo = false;
  SQLRETURN retCode = SQL_SUCCESS;

  // Check if database exists
  if (db == NULL)
    return NO_DATABASE_FOUND;

  // Check for existing struct
  if (*hStmtStruct != NULL)
    return HANDLE_STATEMENT_EXISTS;

  *hStmtStruct = (queryStruct *)malloc(sizeof(queryStruct));
  if (*hStmtStruct == NULL)
    return MALLOC_FAILURE;

  // Initialize values
  (*hStmtStruct)->hStmts = SQL_NULL_HSTMT;
  (*hStmtStruct)->rowCountPtr = 0;
  (*hStmtStruct)->type = -1;
  (*hStmtStruct)->retrieve = NULL;

  // Allocate a statement handle
  retCode =
      SQLAllocHandle(SQL_HANDLE_STMT, db->hnd->hDbc, &((*hStmtStruct)->hStmts));
  if (retCode != SQL_SUCCESS) {
    generateDatabaseError(db->err, db->hnd);
    if (retCode == SQL_SUCCESS_WITH_INFO) {
      haveInfo = true;
    } else {
      free(*hStmtStruct);
      *hStmtStruct = NULL;
      return STMT_HANDLE_SETUP_FAILURE;
    }
  }

  // Directly execute ad hoc queries
  retCode =
      SQLExecDirect((*hStmtStruct)->hStmts, (SQLCHAR *)query, strlen(query));
  if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
    retCode =
        SQLNumResultCols((*hStmtStruct)->hStmts, (SQLSMALLINT *)&numColumns);
    // SQLNumResultCols Didn't work and we don't have a SELECT statement
    if (numColumns == 0) {
      retCode =
          SQLRowCount((*hStmtStruct)->hStmts, &((*hStmtStruct)->rowCountPtr));
      if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
        // It was an other query. i.e chreate drop etc.
        if ((*hStmtStruct)->rowCountPtr == -1) {
          return SUCCESS;
        } else {
          // It was an update query i.e update insert etc
          return SUCCESS;
        }
      }

    } else {
      // Select query, allocate appropriate structures
      (*hStmtStruct)->retrieve = (retrieveQuery *)malloc(sizeof(retrieveQuery));

      if ((*hStmtStruct)->retrieve == NULL)
        return MALLOC_FAILURE;

      // The Query was a SELECT or VALUES Statement
      // Initialize and malloc retrieve variables
      (*hStmtStruct)->retrieve->sNumColResults = numColumns;
      (*hStmtStruct)->retrieve->rowCount = 0;
      (*hStmtStruct)->retrieve->columnName =
          (char **)malloc(numColumns * sizeof(char *));
      (*hStmtStruct)->retrieve->columnData =
          (data *)malloc(numColumns * sizeof(data));
      (*hStmtStruct)->retrieve->columnDataType =
          (short int *)malloc(numColumns * sizeof(short int));
      (*hStmtStruct)->retrieve->columnDataSize =
          (SQLULEN *)malloc(numColumns * sizeof(SQLULEN));

      // Initialize variables
      for (int i = 0; i < numColumns; i++) {
        (*hStmtStruct)->retrieve->columnName[i] = NULL;
        (*hStmtStruct)->retrieve->columnDataType[i] = -1;
        (*hStmtStruct)->retrieve->columnDataSize[i] = 0;
        (*hStmtStruct)->retrieve->columnData[i].item = NULL;
        (*hStmtStruct)->retrieve->columnData[i].next = NULL;
      }

      // Get info about each column
      for (int i = 0; i < numColumns; i++) {
        SQLCHAR *colName = (SQLCHAR *)malloc(sizeof(SQLCHAR) * MAX_COL_NAME);

        SQLSMALLINT columnNameLen;
        SQLSMALLINT columnDataDigits;
        SQLSMALLINT columnDataNullable;

        // Get SQLDescribeCol info
        retCode = SQLDescribeCol((*hStmtStruct)->hStmts, i + 1, colName,
                                 MAX_COL_NAME, &columnNameLen,
                                 &((*hStmtStruct)->retrieve->columnDataType[i]),
                                 &((*hStmtStruct)->retrieve->columnDataSize[i]),
                                 &columnDataDigits, &columnDataNullable);

        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {

          (*hStmtStruct)->retrieve->columnName[i] =
              (char *)malloc(sizeof(char) * (columnNameLen + 1));
          strcpy((*hStmtStruct)->retrieve->columnName[i], (char *)colName);
          free(colName);
          printf("\nColumn type: %d, Column name: %s, Column data size%d\n",
                 (*hStmtStruct)->retrieve->columnDataType[i],
                 (*hStmtStruct)->retrieve->columnName[i],
                 (*hStmtStruct)->retrieve->columnName[i]);
        }
      }

      // Setup temporary pointers to cell heads
      data *tempHead[numColumns] = NULL;
      for (int i = 0; i < numColumns; i++)
        tempHead[i] = &((*hStmtStruct)->retrieve->columnData[i]);

      // Get actual data until no more data is returned
      while (true) {
        retCode = SQLFetch((*hStmtStruct)->hStmts);
        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {

          ((*hStmtStruct)->retrieve->rowCount)++;

          if ((*hStmtStruct)->retrieve->rowCount > 1) {
            for (int i = 0; i < numColumns; i++) {
              tempHead[i]->next = (data *)malloc(sizeof(data));
              // check malloc
              tempHead[i] = tempHead[i]->next;
              tempHead[i]->item = NULL;
              tempHead[i]->next = NULL;
            }
          }

          for (int i = 0; i < numColumns; i++) {

          }

        } else {
          if (retCode != SQL_NO_DATA) {
            // free and return err
          } else {
            break;
          }
        }
      }
    }

    return haveInfo ? SUCCESS_WITH_INFO : SUCCESS;

  } else {

    SQLFreeHandle(SQL_HANDLE_STMT, (*hStmtStruct)->hStmts);
    free(*hStmtStruct);
    *hStmtStruct = NULL;
    return QUERY_EXECUTION_FAILURE;
  }

  return SUCCESS;
}

/*
 * Function:  freeQueryStruct
 * ------------------
 * Frees queryStruct with retrieve filled
 *
 */
void freeQueryStruct(queryStruct **hStmtStruct) {

  // Check if the whole struct exists
  if (*hStmtStruct == NULL)
    return;

  // Check if the retrieve struct exists
  if ((*hStmtStruct)->retrieve == NULL)
    return;

  // Check if Column names has been made
  if ((*hStmtStruct)->retrieve->columnName == NULL)
    return;

  // Loop through the column names and free all names
  // Check Column names actually exist
  for (int i = 0; i < (*hStmtStruct)->retrieve->sNumColResults; i++) {
    if ((*hStmtStruct)->retrieve->columnName[i] == NULL)
      return;
    // if((*hStmtStruct)->retrieve->columnData[i] == NULL)
    // return;

    // if((*hStmtStruct)->retrieve->columnDataType[i] == NULL)
    // return;

    // free((*hStmtStruct)->retrieve->columnName[i]);
    // free((*hStmtStruct)->retrieve->columnData[i]);
    // free((*hStmtStruct)->retrieve->columnDataType[i]);
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
  free(*hStmtStruct);
}
