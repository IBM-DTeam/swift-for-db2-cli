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
state query(database *db, queryStruct **hStmtStruct, char *query) {
  int numColumns = 0;
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
    generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
    if (retCode == SQL_SUCCESS_WITH_INFO) {
      haveInfo = true;
    } else {
      freeQueryStruct(hStmtStruct);
      return STMT_HANDLE_SETUP_FAILURE;
    }
  }

  // Directly execute ad hoc queries
  retCode = SQLExecDirect((*hStmtStruct)->hStmts, (SQLCHAR*)query, strlen(query));
  if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
    if (retCode == SQL_SUCCESS_WITH_INFO) {
      generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
      haveInfo = true;
    }

    // Get the number of columns
    retCode = SQLNumResultCols((*hStmtStruct)->hStmts, (SQLSMALLINT*) &numColumns);
    if (retCode != SQL_SUCCESS) {
      generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
      if (retCode == SQL_SUCCESS_WITH_INFO) {
        haveInfo = true;
      } else {
        freeQueryStruct(hStmtStruct);
        return GET_NUM_COLUMNS_FAILURE;
      }
    }

    // SQLNumResultCols Didn't generate a result set, it wasn't a SELECT or VALUES query.
    if (numColumns == 0) {

      retCode = SQLRowCount((*hStmtStruct)->hStmts, &((*hStmtStruct)->rowCountPtr));
      if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
        if (retCode == SQL_SUCCESS_WITH_INFO) {
          generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
          haveInfo = true;
        }

        // It was an other query. i.e create, drop, etc.
        if ((*hStmtStruct)->rowCountPtr == -1) {

          (*hStmtStruct)->type = OTHER;
          return SUCCESS;

        } else {

          // It was an update query i.e update, insert, etc
          (*hStmtStruct)->type = INFO;
          return SUCCESS;
        }
      } else {
        freeQueryStruct(hStmtStruct);
        return GET_NUM_ROWS_FAILURE;
      }

    } else {

      // The query returned a result set
      (*hStmtStruct)->type = RECIEVE;

      // Allocate appropriate structures
      (*hStmtStruct)->retrieve = (retrieveQuery*) malloc(sizeof(retrieveQuery));
      if ((*hStmtStruct)->retrieve == NULL) {
        freeQueryStruct(hStmtStruct);
        return MALLOC_FAILURE;
      }

      // The Query was a SELECT or VALUES Statement
      // Initialize and malloc retrieve variables
      (*hStmtStruct)->retrieve->sNumColResults = numColumns;
      (*hStmtStruct)->retrieve->rowCount = 0;
      (*hStmtStruct)->retrieve->columnName = NULL;
      (*hStmtStruct)->retrieve->columnData = NULL;
      (*hStmtStruct)->retrieve->columnDataType = NULL;
      (*hStmtStruct)->retrieve->columnDataSize = NULL;

      // Allocation
      (*hStmtStruct)->retrieve->columnName = (char**) malloc(numColumns * sizeof(char*));
      if ((*hStmtStruct)->retrieve->columnName == NULL) {
        freeQueryStruct(hStmtStruct);
        return MALLOC_FAILURE;
      }

      // Initialize values.
      for (int i = 0; i < numColumns; i++)
        (*hStmtStruct)->retrieve->columnName[i] = NULL;

      // Allocation
      (*hStmtStruct)->retrieve->columnData = (data**) malloc(numColumns * sizeof(data*));
      if ((*hStmtStruct)->retrieve->columnData == NULL) {
        freeQueryStruct(hStmtStruct);
        return MALLOC_FAILURE;
      }

      // Initialize values.
      for (int i = 0; i < numColumns; i++)
        (*hStmtStruct)->retrieve->columnData[i] = NULL;

      // Allocation
      for (int i = 0; i < numColumns; i++) {
        (*hStmtStruct)->retrieve->columnData[i] = (data*) malloc(sizeof(data));
        if ((*hStmtStruct)->retrieve->columnData[i] == NULL) {
          freeQueryStruct(hStmtStruct);
          return MALLOC_FAILURE;
        }

        // Initialize values.
        (*hStmtStruct)->retrieve->columnData[i]->item = NULL;
        (*hStmtStruct)->retrieve->columnData[i]->next = NULL;
      }

      // Allocation
      (*hStmtStruct)->retrieve->columnDataType = (short int*) malloc(numColumns * sizeof(short int));
      if ((*hStmtStruct)->retrieve->columnDataType == NULL) {
        freeQueryStruct(hStmtStruct);
        return MALLOC_FAILURE;
      }

      // Allocation
      (*hStmtStruct)->retrieve->columnDataSize = (SQLULEN*) malloc(numColumns * sizeof(SQLULEN));
      if ((*hStmtStruct)->retrieve->columnDataSize == NULL) {
        freeQueryStruct(hStmtStruct);
        return MALLOC_FAILURE;
      }

      // Initialize variables
      for (int i = 0; i < numColumns; i++) {
        (*hStmtStruct)->retrieve->columnName[i] = NULL;
        (*hStmtStruct)->retrieve->columnDataType[i] = -1;
        (*hStmtStruct)->retrieve->columnDataSize[i] = 0;
        (*hStmtStruct)->retrieve->columnData[i]->item = NULL;
        (*hStmtStruct)->retrieve->columnData[i]->next = NULL;
      }

      // Get info about each column
      for (int i = 0; i < numColumns; i++) {
        char colName[MAX_COL_NAME];

        SQLSMALLINT columnNameLen;
        SQLSMALLINT columnDataDigits;
        SQLSMALLINT columnDataNullable;

        // Get SQLDescribeCol info
        retCode = SQLDescribeCol((*hStmtStruct)->hStmts, i + 1, (SQLCHAR*) colName,
                                 MAX_COL_NAME, &columnNameLen,
                                 &((*hStmtStruct)->retrieve->columnDataType[i]),
                                 &((*hStmtStruct)->retrieve->columnDataSize[i]),
                                 &columnDataDigits, &columnDataNullable);

        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
          if (retCode == SQL_SUCCESS_WITH_INFO) {
            generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
            haveInfo = true;
          }

          // Allocate the length of the column name
          (*hStmtStruct)->retrieve->columnName[i] = (char*) malloc(sizeof(char) * (columnNameLen + 1));
          if ((*hStmtStruct)->retrieve->columnName[i] == NULL) {
            freeQueryStruct(hStmtStruct);
            return MALLOC_FAILURE;
          }

          // Copy it over.
          strcpy((*hStmtStruct)->retrieve->columnName[i], (char *)colName);

        } else {
          freeQueryStruct(hStmtStruct);
          return DESCRIBE_COL_FAILURE;
        }
      }

      // Setup temporary pointers to cell heads
      data *tempHead[numColumns];
      for (int i = 0; i < numColumns; i++)
        tempHead[i] = (*hStmtStruct)->retrieve->columnData[i];

      // Get actual data until no more data is returned
      while (true) {

        // Get the next row
        retCode = SQLFetch((*hStmtStruct)->hStmts);
        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
          if (retCode == SQL_SUCCESS_WITH_INFO) {
            generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
            haveInfo = true;
          }

          // Increment the row count
          ((*hStmtStruct)->retrieve->rowCount)++;

          // Already initialized struct for rowCount 0, above.
          if ((*hStmtStruct)->retrieve->rowCount > 1) {
            for (int i = 0; i < numColumns; i++) {
              tempHead[i]->next = (data*) malloc(sizeof(data));
              if (tempHead[i]->next == NULL) {
                // Malloc failed, cleanup.
                freeQueryStruct(hStmtStruct);
                return MALLOC_FAILURE;
              }

              // Initialize the struct
              tempHead[i] = tempHead[i]->next;
              tempHead[i]->item = NULL;
              tempHead[i]->next = NULL;
            }
          }

          for (int i = 0; i < numColumns; i++) {

            SQLLEN binaryLenOrInd = 0;
            char cell[(*hStmtStruct)->retrieve->columnDataSize[i] + 1];
            while (true) {

              // Where to continue writing in the char array.
              int writeIndex = 0;

              // Get the data for the cell
              retCode = SQLGetData((*hStmtStruct)->hStmts, i + 1, SQL_C_CHAR, &cell[writeIndex], sizeof(cell) - (sizeof(char) * writeIndex), &binaryLenOrInd);
              if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
                // Set the next write index for the char array
                writeIndex = strlen(cell) + 1;
              } else {
                // We encountered an error getting the cell data
                if (retCode != SQL_NO_DATA) {
                  generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
                  freeQueryStruct(hStmtStruct);
                  return CANNOT_GET_DATA;
                } else {
                  // No more data for this cell.
                  // Copy it over to the data list and move onto
                  // the next cell.

                  // If the cell held a NULL, we do nothing
                  if (binaryLenOrInd != SQL_NULL_DATA) {
                    tempHead[i]->item = (char*) malloc(sizeof(char) * (strlen(cell) + 1));
                    if (tempHead[i]->item == NULL) {
                      // Malloc failed, cleanup.
                      freeQueryStruct(hStmtStruct);
                      return MALLOC_FAILURE;
                    }

                    // Copy the cell data over into our structure.
                    strcpy(tempHead[i]->item, cell);

                  }

                  break;
                }
              }
            }
          }

        } else {
          // We encountered an error getting the row
          if (retCode != SQL_NO_DATA) {
            // Error, clean up and return.
            generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
            freeQueryStruct(hStmtStruct);
            return CANNOT_FETCH_ROW;
          } else {
            // No more rows.
            break;
          }
        }
      }
    }

    return haveInfo ? SUCCESS_WITH_INFO : SUCCESS;

  } else {
    // Query failed to execute.
    generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
    freeQueryStruct(hStmtStruct);
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
void freeQueryStruct(queryStruct **hStmtStruct) {

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
