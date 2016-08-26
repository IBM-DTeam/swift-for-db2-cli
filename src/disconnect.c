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

 #include "disconnect.h"


/*
 * Function:  disconnect
 * ------------------
 * disconnects from a database.
 *
 * db: The database struct to disconnect..
 *
 * returns:
 *   NO_DATABASE_FOUND: The struct is NULL, can't disconnect it.
 *   DATABASE_DISCONNECT_FAILURE: Couldn't disconnect the database, check errors
 *   SUCCESS: Database disconnected.
 *
 */
state disconnect(database** db) {

  // The database pointer is NULL.
  if ((*db) == NULL)
    return NO_DATABASE_FOUND;

  SQLRETURN retCode = SQL_SUCCESS; // Return code

  //Disconnect From the database
  retCode = SQLDisconnect((*db)->hnd->hDbc);
  if (!(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)) {
    generateDatabaseError((*db)->err, (*db)->hnd->hDbc, SQL_HANDLE_DBC);
    return DATABASE_DISCONNECT_FAILURE;
  }

  //Free allocated memory before returning
  freeDatabase(db);
  return SUCCESS;
}
