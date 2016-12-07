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

#ifndef structs_h
#define structs_h

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "/usr/local/ibmdb/include/sqlcli1.h"
#include "type.h"

// Database error struct
typedef struct databaseError {
  char* state;
  int native;
  char* desc;
  struct databaseError* next;
} databaseError;

// Error struct
typedef struct error {
  int errorType;
  databaseError* database;
} error;

// handle struct
typedef struct handle {
  SQLHDBC hDbc;
  SQLHENV hEnv;
} handle;

typedef struct database {
  handle* hnd;
  error* err;
} database;


typedef struct data {
  char*            item;
  struct data*     next;
} data;

typedef struct retrieveQuery{
  SQLSMALLINT      sNumColResults;
  char**           columnName;
  short int*       columnDataType;
  SQLULEN*         columnDataSize;
  data**           columnData;
  SQLINTEGER       rowCount;
} retrieveQuery;

typedef struct queryStruct{
  SQLLEN           rowCountPtr;
  SQLHSTMT         hStmts;
  queryType        type;
  retrieveQuery*   retrieve;
} queryStruct;


#endif
