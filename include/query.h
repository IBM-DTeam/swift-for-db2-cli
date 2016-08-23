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

#ifndef query_h
#define query_h

#include <stdlib.h>
#include <string.h>
#include <sqlcli1.h>

#include "error.h"
#include "type.h"
#include "database.h"

typedef struct cellData {
  char*            item;
  cellData*        next;
} cellData;

typedef struct retrieveQuery{
  SQLSMALLINT      sNumColResults;
  char***          columnName;
  short int**      columnDataType;
  cellData**       columnData;
  SQLINTEGER       rowCount;
} retrieveQuery;

typedef struct queryStruct{
  SQLLEN           rowCountPtr;
  SQLHSTMT         hStmts;
  int              queryType;
  retrieveQuery*   retrieve;
} queryStruct;

// Methods
void freeQueryStruct(queryStruct** hStmtStruct);
state query(database* db, queryStruct** hStmtStruct, char* query);

#endif
