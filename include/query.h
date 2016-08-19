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

typedef struct retrieveQuery{
  SQLSMALLINT* sNumColResults;
  SQLCHAR *      ColumnName;
  SQLSMALLINT *  DataTypePtr;
} retrieveQuery;

typedef struct updateData{

} updateData;

typedef struct queryStruct{
  SQLHSTMT* hStmts;
  int queryType;
  updateData* update;
  retrieveQuery* retrieve;
}queryStruct;


typedef struct stmtHandle {
  SQLHSTMT* hStmts;
  int queryType;
  SQLSMALLINT * sNumResults;
} stmtHandle;

// Methods
state query(database** db, SQLHANDLE* hStmtStruct, char* query);
#endif
