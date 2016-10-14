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

  return haveInfo ? SUCCESS_WITH_INFO : SUCCESS;


}


state executePrepared(database *db, queryStruct **hStmtStruct){
  SQLRETURN retCode = SUCCESS;
  bool haveInfo = false;


    state results = result(db, hStmtStruct);
    generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
    if (results != SUCCESS) {
      generateDatabaseError(db->err, (*hStmtStruct)->hStmts, SQL_HANDLE_STMT);
      if (results == SUCCESS_WITH_INFO) {
        haveInfo = true;
      } else {
        return QUERY_EXECUTION_FAILURE;
      }
    }

    return haveInfo ? SUCCESS_WITH_INFO : SUCCESS;

}
