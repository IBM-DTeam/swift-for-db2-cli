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
 * connectionString: The database connection string.
 *
 * returns: The handles for the database connection which will be used
 *          for operations like querying, disconnecting, etc.
 *          returns null on error.
 */
handles* connect(const char* connectionString) {

  // Create the handles struct
  handles*      h             = (handles*) malloc(sizeof(handles)); // Handles struct
  h->hDbc                     = SQL_NULL_HDBC;                      // Environment
  h->hEnv                     = SQL_NULL_HENV;                      // Connection handle
  SQLRETURN     retCode       = SQL_SUCCESS;                        // Statement handle
  SQLCHAR       outStr[1024];
  SQLSMALLINT   outStrLen;

  // Allocate an environment handle
  retCode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &(h->hEnv));
  if (retCode != SQL_SUCCESS) {
    // Error
    free(h);
    return NULL;
  }

  // ODBC 3 support
  retCode = SQLSetEnvAttr(h->hEnv, SQL_ATTR_ODBC_VERSION, (void*) SQL_OV_ODBC3, 0);
  if (retCode != SQL_SUCCESS) {
    // Error
    free(h);
    return NULL;
  }

  // Allocate a connection handle
  retCode = SQLAllocHandle(SQL_HANDLE_DBC, h->hEnv, &(h->hDbc));
  if (retCode != SQL_SUCCESS) {
    // Error
    free(h);
    return NULL;
  }

  // Connect to the database
  retCode = SQLDriverConnect(h->hDbc,
                             NULL,
                             (SQLCHAR*) connectionString,
                             SQL_NTS,
                             outStr,
                             sizeof(outStr),
                             &outStrLen,
                             SQL_DRIVER_COMPLETE
                            );

  if (retCode != SQL_SUCCESS) {
    // Error
    free(h);
    return NULL;
  }

  return h;
}
