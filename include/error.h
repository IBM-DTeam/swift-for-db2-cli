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

#ifndef db_error_h
#define db_error_h

#include "/usr/local/ibmdb/include/sqlcli1.h"
#include "type.h"
#include "handle.h"

#include <stdlib.h>
#include <string.h>

// Origin of error
#define NO_ERROR 0
#define DATABASE_ERROR 1

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


// Methods
state db_initializeError(error** e);
void db_resetErrors(error* e);
void db_freeErrors(error** e);
databaseError* db_getNextError(error* e);
state db_generateDatabaseError(error* e, SQLHANDLE h, SQLSMALLINT hType);
void db_freeDatabaseErrors(error* e);
void db_freeDatabaseError(databaseError** e);

#endif
