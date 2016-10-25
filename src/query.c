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
#include "result.h"

#define MAX_COL_NAME 256

/*
 * Function:  query
 * ------------------
 * Querys the database (db) with the provided query and fills out the
 * hStmtStruct properly.
 *
 * More info to come.
 *
 * db: Connected and ready database..
 * hStmt: The statement handle pointer.
 * query: The string of the query.
 *
 * returns:
 *   SUCCESS: The database struct was setup without issues.
 *   SUCCESS_WITH_INFO: The database strruct was setup without a message.
 *   NO_DATABASE_FOUND: Database does not exist.
 *   STATEMENT_HANDLE_EXISTS: The provided handle is already allocated.
 *   STMT_HANDLE_SETUP_FAILURE: failed to properly set up the statement handle.
 *   QUERY_EXECUTION_FAILURE: Execution of query failed.
 *   GET_NUM_COLUMNS_FAILURE: Cannot get the number of columns for the table.
 *   GET_NUM_ROWS_FAILURE: Cannot get number of rows affected.
 *   DESCRIBE_COL_FAILURE: Cannot describe the column.
 *   CANNOT_GET_DATA: Cannot get data for the nth column and nth row.
 *   CANNOT_FETCH_ROW: Cannot get the next row.
 *   MALLOC_FAILURE: Failed to allocate memory somewhere.
 *
 */
state db_query(database *db, queryStruct **hStmtStruct, char *query) {
  bool haveInfo = false;
  SQLRETURN retCode = SQL_SUCCESS;

  // Check if database exists
  if (db == NULL)
    return NO_DATABASE_FOUND;

  // Check for existing struct
  if (*hStmtStruct != NULL)
    return STATEMENT_HANDLE_EXISTS;

  *hStmtStruct = (queryStruct*) malloc(sizeof(queryStruct));
  if (*hStmtStruct == NULL)
    return MALLOC_FAILURE;

  // Initialize values
  (*hStmtStruct)->hStmts = SQL_NULL_HSTMT;
  (*hStmtStruct)->rowCountPtr = 0;
  (*hStmtStruct)->type = -1;
  (*hStmtStruct)->retrieve = NULL;

  // Allocate a statement handle
  retCode = SQLAllocHandle(SQL_HANDLE_STMT, db->hnd->hDbc, &((*hStmtStruct)->hStmts));
  if (retCode != SQL_SUCCESS) {
    db_generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
    if (retCode == SQL_SUCCESS_WITH_INFO) {
      haveInfo = true;
    } else {
      db_freeQueryStruct(hStmtStruct);
      return STMT_HANDLE_SETUP_FAILURE;
    }
  }

  // Directly execute ad hoc queries
  retCode = SQLExecDirect((*hStmtStruct)->hStmts, (SQLCHAR*)query, strlen(query));

  if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
    if (retCode == SQL_SUCCESS_WITH_INFO) {
      db_generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
      haveInfo = true;
    }

    state results = db_result(db, hStmtStruct);

    if (results != SUCCESS) {
      db_generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
      if (results == SUCCESS_WITH_INFO) {
        haveInfo = true;
      } else {
        db_freeQueryStruct(hStmtStruct);
        return QUERY_EXECUTION_FAILURE;
      }
    }

  } else {
    // Query failed to execute.
    db_generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
    db_freeQueryStruct(hStmtStruct);
    return QUERY_EXECUTION_FAILURE;
  }

  return haveInfo ? SUCCESS_WITH_INFO : SUCCESS;

}

/*
 * Function:  freeQueryStruct
 * ------------------
 * Frees queryStruct with retrieve filled
 *
 */
void db_freeQueryStruct(queryStruct **hStmtStruct) {

  // Check if the whole struct exists
  if (*hStmtStruct == NULL)
    return;

  // Check if the retrieve struct exists
  if ((*hStmtStruct)->retrieve != NULL) {

    // Ensure we have at least 1 or more columns
    if ((*hStmtStruct)->retrieve->sNumColResults > 0) {

      for (int i = 0; i < (*hStmtStruct)->retrieve->sNumColResults; i++) {

        // Free the column name's
        if ((*hStmtStruct)->retrieve->columnName[i] != NULL)
          free((*hStmtStruct)->retrieve->columnName[i]);

        // Temporary pointers to help us free
        data* head = (*hStmtStruct)->retrieve->columnData[i];
        data* previous = (*hStmtStruct)->retrieve->columnData[i];

        // Goes through every row (j) for the column (i)
        for (int j = 0; j < (*hStmtStruct)->retrieve->rowCount; j++) {

          // Clears the items
          if (head->item != NULL)
            free(head->item);

          // Goes to the next item.
          if (head->next != NULL) {
            previous = head;
            head = head->next;
            free(previous);
          } else {
            // Last item, free it.
            free(head);
            break;
          }

        }

      }

    }

    // Free the column name
    if ((*hStmtStruct)->retrieve->columnName != NULL)
      free((*hStmtStruct)->retrieve->columnName);

    // Free the column data type
    if ((*hStmtStruct)->retrieve->columnDataType != NULL)
      free((*hStmtStruct)->retrieve->columnDataType);

    // Free the column data size
    if ((*hStmtStruct)->retrieve->columnDataSize != NULL)
      free((*hStmtStruct)->retrieve->columnDataSize);

    // Free the column data
    if ((*hStmtStruct)->retrieve->columnData != NULL)
      free((*hStmtStruct)->retrieve->columnData);

    // Free retrieve itself.
    free((*hStmtStruct)->retrieve);

  }

  // Free the handle statement
  if ((*hStmtStruct)->hStmts != SQL_NULL_HSTMT)
    SQLFreeHandle(SQL_HANDLE_STMT, (*hStmtStruct)->hStmts);

  // Free the final statement struct
  free(*hStmtStruct);

  *hStmtStruct = NULL;

}
