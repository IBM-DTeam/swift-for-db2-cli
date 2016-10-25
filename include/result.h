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

#ifndef db_result_h
#define db_result_h

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "/usr/local/ibmdb/include/sqlcli1.h"

#include "error.h"
#include "type.h"
#include "database.h"
#include "query.h"
// Methods
state db_result(database *db, queryStruct **hStmtStruct);
data *db_getColumn(queryStruct *hStmtStruct, char *column);
state db_getNextRow(queryStruct *hStmtStruct);

#endif
