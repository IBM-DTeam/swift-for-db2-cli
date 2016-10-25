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
#include "prepared.h"

#define MAX_COL_NAME 256


/*
 * Function:  result
 * ------------------
 * Given a proper hStmt and bound columns, will store information into the
 * struct based on the given query type
 *
 * db: Where the database struct will be setup.
 * hStmt: The statement handle pointer
 * query: The string of the actual query
 * values: A char array of the values to be entered
 *
 * returns:
 *   SUCCESS: The database struct was setup without issues.
 *
 */
state db_result(database *db, queryStruct **hStmtStruct){
  int numColumns = 0;
  SQLRETURN retCode = SQL_SUCCESS;
  bool haveInfo = false;

  // Get the number of columns
  retCode = SQLNumResultCols((*hStmtStruct)->hStmts, (SQLSMALLINT*) &numColumns);
  if (retCode != SQL_SUCCESS) {
    db_generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
    if (retCode == SQL_SUCCESS_WITH_INFO) {
      haveInfo = true;
    } else {
      db_freeQueryStruct(hStmtStruct);
      return GET_NUM_COLUMNS_FAILURE;
    }
  }

  // SQLNumResultCols Didn't generate a result set, it wasn't a SELECT or VALUES query.
  if (numColumns == 0) {

    retCode = SQLRowCount((*hStmtStruct)->hStmts, &((*hStmtStruct)->rowCountPtr));
    if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
      if (retCode == SQL_SUCCESS_WITH_INFO) {
        db_generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
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
      db_freeQueryStruct(hStmtStruct);
      return GET_NUM_ROWS_FAILURE;
    }

  } else {

    // The query returned a result set
    (*hStmtStruct)->type = RECIEVE;

    // Allocate appropriate structures
    (*hStmtStruct)->retrieve = (retrieveQuery*) malloc(sizeof(retrieveQuery));
    if ((*hStmtStruct)->retrieve == NULL) {
      db_freeQueryStruct(hStmtStruct);
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
      db_freeQueryStruct(hStmtStruct);
      return MALLOC_FAILURE;
    }

    // Initialize values.
    for (int i = 0; i < numColumns; i++)
      (*hStmtStruct)->retrieve->columnName[i] = NULL;

    // Allocation
    (*hStmtStruct)->retrieve->columnData = (data**) malloc(numColumns * sizeof(data*));
    if ((*hStmtStruct)->retrieve->columnData == NULL) {
      db_freeQueryStruct(hStmtStruct);
      return MALLOC_FAILURE;
    }

    // Initialize values.
    for (int i = 0; i < numColumns; i++)
      (*hStmtStruct)->retrieve->columnData[i] = NULL;

    // Allocation
    for (int i = 0; i < numColumns; i++) {
      (*hStmtStruct)->retrieve->columnData[i] = (data*) malloc(sizeof(data));
      if ((*hStmtStruct)->retrieve->columnData[i] == NULL) {
        db_freeQueryStruct(hStmtStruct);
        return MALLOC_FAILURE;
      }

      // Initialize values.
      (*hStmtStruct)->retrieve->columnData[i]->item = NULL;
      (*hStmtStruct)->retrieve->columnData[i]->next = NULL;
    }

    // Allocation
    (*hStmtStruct)->retrieve->columnDataType = (short int*) malloc(numColumns * sizeof(short int));
    if ((*hStmtStruct)->retrieve->columnDataType == NULL) {
      db_freeQueryStruct(hStmtStruct);
      return MALLOC_FAILURE;
    }

    // Allocation
    (*hStmtStruct)->retrieve->columnDataSize = (SQLULEN*) malloc(numColumns * sizeof(SQLULEN));
    if ((*hStmtStruct)->retrieve->columnDataSize == NULL) {
      db_freeQueryStruct(hStmtStruct);
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
          db_generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
          haveInfo = true;
        }

        // Allocate the length of the column name
        (*hStmtStruct)->retrieve->columnName[i] = (char*) malloc(sizeof(char) * (columnNameLen + 1));
        if ((*hStmtStruct)->retrieve->columnName[i] == NULL) {
          db_freeQueryStruct(hStmtStruct);
          return MALLOC_FAILURE;
        }

        // Copy it over.
        strcpy((*hStmtStruct)->retrieve->columnName[i], (char *)colName);

      } else {
        db_freeQueryStruct(hStmtStruct);
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
          db_generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
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
              db_freeQueryStruct(hStmtStruct);
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
                db_generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
                db_freeQueryStruct(hStmtStruct);
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
                    db_freeQueryStruct(hStmtStruct);
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
          db_generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
          db_freeQueryStruct(hStmtStruct);
          return CANNOT_FETCH_ROW;
        } else {
          // No more rows.
          break;
        }
      }
    }
    return haveInfo ? SUCCESS_WITH_INFO : SUCCESS;
}
}


/*
 * Function:  getColumn
 * ------------------
 * Returns a pointer to the first row in the columnName.
 * Returns NULL if the column doesn't exist.
 * Returns STATEMENT_HANDLE_NONEXISTANT if the queryStruct is NULL.
 *
 */
data *db_getColumn(queryStruct *hStmtStruct, char *columnName) {

  if (hStmtStruct == NULL)
    return NULL;

  for (int i = 0; i < hStmtStruct->retrieve->sNumColResults; i++)
    if (strcmp(hStmtStruct->retrieve->columnName[i], columnName) == 0)
      return hStmtStruct->retrieve->columnData[i];
  return NULL;

}

/*
 * Function:  getNextRow
 * ------------------
 * Fetches the next row in the result set to allow easy access.
 * Discards and cleans up old rows.
 * Returns STATEMENT_HANDLE_NONEXISTANT if the queryStruct is NULL.
 *
 */
state db_getNextRow(queryStruct *hStmtStruct) {

  if (hStmtStruct == NULL)
    return STATEMENT_HANDLE_NONEXISTANT;

  for (int i = 0; i < hStmtStruct->retrieve->sNumColResults; i++) {
    data* tempData = hStmtStruct->retrieve->columnData[i];

    if (tempData != NULL) {
      hStmtStruct->retrieve->columnData[i] = hStmtStruct->retrieve->columnData[i]->next;
      free(tempData->item);
      free(tempData);
    } else {
      return NO_MORE_ROWS;
    }

  }

  return NO_MORE_ROWS;

}
