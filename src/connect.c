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

#include "connect.h"


/*
 * Function:  connect
 * ------------------
 * Connects to a database.
 *
 * db: Where to create the database struct.
 * connectionString: The database connection string.
 *
 * returns:
 *   SUCCESS: There is a connection to the database.
 *   SUCCESS_WITH_INFO: There is a connection to the database with a message.
 *   CATASTROPHIC_FAILURE: The database struct cannot be setup (malloc'd).
 *   SETUP_DATABASE_FAILURE: Cannot setup the bridge to the database, check errors.
 *   DATABASE_EXISTS: A database already exists at this location.
 */
state connect(database** db, const char* connectionString) {

  // The database pointer isn't NULL.
  if ((*db) != NULL)
    return DATABASE_EXISTS;

  // Create the database struct
  state s = SUCCESS;
  s = createDatabase(db);
  if (s == MALLOC_FAILURE)
    return CATASTROPHIC_FAILURE;

  bool haveInfo = false;

  SQLRETURN retCode = SQL_SUCCESS;
  SQLCHAR outStr[1024];
  SQLSMALLINT outStrLen;

  // Allocate an environment handle
  retCode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &((*db)->hnd->hEnv));
  if (retCode != SQL_SUCCESS) {
    generateDatabaseError((*db)->err, (*db)->hnd);
    if (retCode == SQL_SUCCESS_WITH_INFO) {
      haveInfo = true;
    } else {
      return SETUP_DATABASE_FAILURE;
    }
  }

  // ODBC 3 support
  retCode = SQLSetEnvAttr((*db)->hnd->hEnv, SQL_ATTR_ODBC_VERSION, (void*) SQL_OV_ODBC3, 0);
  if (retCode != SQL_SUCCESS) {
    generateDatabaseError((*db)->err, (*db)->hnd);
    if (retCode == SQL_SUCCESS_WITH_INFO) {
      haveInfo = true;
    } else {
      return SETUP_DATABASE_FAILURE;
    }
  }

  // Allocate a connection handle
  retCode = SQLAllocHandle(SQL_HANDLE_DBC, (*db)->hnd->hEnv, &((*db)->hnd->hDbc));
  if (retCode != SQL_SUCCESS) {
    generateDatabaseError((*db)->err, (*db)->hnd);
    if (retCode == SQL_SUCCESS_WITH_INFO) {
      haveInfo = true;
    } else {
      return SETUP_DATABASE_FAILURE;
    }
  }

  // Connect to the database
  retCode = SQLDriverConnect((*db)->hnd->hDbc,
                             NULL,
                             (SQLCHAR*) connectionString,
                             SQL_NTS,
                             outStr,
                             sizeof(outStr),
                             &outStrLen,
                             SQL_DRIVER_COMPLETE
                            );

  if (retCode != SQL_SUCCESS) {
    generateDatabaseError((*db)->err, (*db)->hnd);
    if (retCode == SQL_SUCCESS_WITH_INFO) {
      haveInfo = true;
    } else {
      return SETUP_DATABASE_FAILURE;
    }
  }

  return haveInfo ? SUCCESS_WITH_INFO : SUCCESS;

}
