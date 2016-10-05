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
 * Function:  prepare
 * ------------------
 * Sets and executes prepared statements
 *
 * db: Where the database struct will be setup.
 * hStmt: The statement handle pointer
 * query: The string of the actual query
 * values: A char array of the values to be entered
 *
 * returns:
 *   SUCCESS: The database struct was setup without issues.
 *   NO_DATABASE_FOUND: Database does not exist
 *   MALLOC_FAILURE: Unable to alloca memory for the given item
 *   STATEMENT_HANDLE_EXISTS: The provided handle is already allocated
 *   STMT_HANDLE_SETUP_FAILURE: COuld not allocate the handle statement
 *   QUERY_FAILURE: Execution of query failed
 *   STMT_HANDLE_SETUP_FAILURE: failed to properly set up the statement handle
 *   SQL_PREPARE_FAILURE: Failed to prepare the handle statement with the query string
 *   NUM_PARAMETERS_FAILURE: Failed to get the number of parameters in the query
 *   DESCRIBE_PARAMETER_FAILURE: Failed the get information about the parameter
 *   PARAMETER_BIND_FAILURE: Could not bind the parameters to the columns
 *   QUERY_EXECUTION_FAILURE: Could not execute the query
 *
 */
state prepare(database *db, queryStruct **hStmtStruct, const char* query, char** values) {
  int numColumns = 0;
  SQLRETURN retCode = SQL_SUCCESS;
  bool haveInfo = false;
  SQLSMALLINT valueSize = 0;

  if (db == NULL)
    return NO_DATABASE_FOUND;

  // Check for existing struct
  if (*hStmtStruct != NULL)
    return STATEMENT_HANDLE_EXISTS;

  // Allocate memory for the handle statement
  *hStmtStruct = (queryStruct*) malloc(sizeof(queryStruct));
  if (*hStmtStruct == NULL)
    return MALLOC_FAILURE;

  // Initialize values
  (*hStmtStruct)->hStmts = SQL_NULL_HSTMT;
  (*hStmtStruct)->rowCountPtr = 0;
  (*hStmtStruct)->type = -1;
  (*hStmtStruct)->retrieve = NULL;

  // Allocate the handle statement with the environment variables
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

  // Pre pare the handle statement with the query
  retCode = SQLPrepare((*hStmtStruct)->hStmts, (SQLCHAR *) query, strlen(query));
  if (retCode != SQL_SUCCESS) {
    generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
    printf("%s\n", db->err->database->desc);
    if (retCode == SQL_SUCCESS_WITH_INFO) {
      haveInfo = true;
    } else {
      return SQL_PREPARE_FAILURE;
    }
  }

  // Get eh number of paramters in the given query
  retCode = SQLNumParams((*hStmtStruct)->hStmts, &valueSize);
  if (retCode != SQL_SUCCESS) {
    generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
    if (retCode == SQL_SUCCESS_WITH_INFO) {
      haveInfo = true;
    } else {
      return NUM_PARAMETERS_FAILURE;
    }
  }

  // Looping over the paramters
  for(int i = 0 ; i < (int) valueSize; i++){
    SQLSMALLINT  dataTypePtr = 0;
    SQLULEN parameterSizePtr = 0;
    SQLSMALLINT decimalDigitsPtr = 0;
    SQLSMALLINT nullablePtr = 0;

    // Get information about the paramters
    retCode = SQLDescribeParam(
      (*hStmtStruct)->hStmts,
      i+1,
      &dataTypePtr,
      &parameterSizePtr,
      &decimalDigitsPtr,
      &nullablePtr);
    if (retCode != SQL_SUCCESS) {
      generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
      if (retCode == SQL_SUCCESS_WITH_INFO) {
        haveInfo = true;
      } else {
        return DESCRIBE_PARAMETER_FAILURE;
      }
    }
    SQLINTEGER pcbValue = SQL_NTS;
    //Bind the paramters with thier right types and descriptions
    retCode = SQLBindParameter(
      (*hStmtStruct)->hStmts,
      i+1,
      SQL_PARAM_INPUT,
      SQL_C_CHAR,
      dataTypePtr,
      parameterSizePtr,
      decimalDigitsPtr,
      values[i],
      strlen(values[i]),
      &pcbValue);
    if (retCode != SQL_SUCCESS) {
      generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
      if (retCode == SQL_SUCCESS_WITH_INFO) {
        haveInfo = true;
      } else {
        return PARAMETER_BIND_FAILURE;
      }
    }

  }

  // Run the command
  retCode = SQLExecute((*hStmtStruct)->hStmts);

  if (retCode != SQL_SUCCESS) {
    generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
    if (retCode == SQL_SUCCESS_WITH_INFO) {
      haveInfo = true;
    } else {
      return QUERY_EXECUTION_FAILURE;
    }
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
            retCode = SQLGetData((*hStmtStruct)->hStmts, i + 1, SQL_C_CHAR, cell + writeIndex, sizeof(cell) - (sizeof(char) * writeIndex), &binaryLenOrInd);
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


  return haveInfo ? SUCCESS_WITH_INFO : SUCCESS;


}}
