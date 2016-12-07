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

#ifndef db_query_h
#define db_query_h

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "/usr/local/ibmdb/include/sqlcli1.h"
#include "structs.h"
#include "error.h"
#include "type.h"
#include "database.h"

// Methods
void db_freeQueryStruct(queryStruct** hStmtStruct);
state db_query(database* db, queryStruct** hStmtStruct, char* query);

#endif
