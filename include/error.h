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

#ifndef error_h
#define error_h

#include "sqlcli1.h"
#include "type.h"
#include "handle.h"

#include <stdlib.h>
#include <string.h>

// Origin of error
#define NO_ERROR 0
#define PACKAGE_ERROR 1
#define DATABASE_ERROR 2

// Package error struct
typedef struct packageError {
  float version;
  char* file;
  int line;
  char* message;
} packageError;

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
  packageError* package;
  databaseError* database;
} error;


// Methods
state initializeError(error** e);
void resetError(error* e);
void freeError(error** e);
state generatePackageError(error* e, const char* file, int line, const char* message);
state generateDatabaseError(error* e, SQLHANDLE h, SQLSMALLINT hType);
void freePackageError(error* e);
void freeDatabaseError(error* e);

#endif
