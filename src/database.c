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

#include "database.h"

/*
 * Function:  createDatabase
 * ------------------
 * Setups a new database struct.
 *
 * db: Where the database struct will be setup.
 *
 * returns:
 *   SUCCESS: The database struct was setup without issues.
 *   MALLOC_FAILURE: The database struct cannot be setup properly.
 */
state db_createDatabase(database** db) {

  state retCode = SUCCESS;

  // Create the database struct itself.
  *db = (database*) malloc(sizeof(database));
  if (*db == NULL)
    return MALLOC_FAILURE;

  // Setup the error struct.
  error* e;
  retCode = db_initializeError(&e);
  if (retCode == MALLOC_FAILURE) {
    free(*db);
    *db = NULL;
    return MALLOC_FAILURE;
  }
  (*db)->err = e;

  // Setup the handle struct.
  handle* h;
  retCode = db_createHandle(&h);
  if (retCode != SUCCESS) {
    free((*db)->err);
    free(*db);
    *db = NULL;
    return MALLOC_FAILURE;
  }
  (*db)->hnd = h;

  return SUCCESS;

}

/*
 * Function:  freeDatabase
 * ------------------
 * Deallocs the database struct.
 *
 * db: the database struct to cleanup.
 *
 */
void db_freeDatabase(database** db) {

  if (*db == NULL)
    return;

  db_freeErrors(&((*db)->err));
  db_freeHandle(&((*db)->hnd));
  free(*db);

  *db = NULL;

}
