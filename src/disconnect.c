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
  * connection: The database connection handle.
  *
  * returns: True if successfully disconnected else false
  *
  */

bool disconnect(handles* connection) {

  SQLRETURN retCode = SQL_SUCCESS; // Return code

  //Disconnect From the database
  retCode = SQLDisconnect( connection->hDbc);
  if(retCode != SQL_SUCCESS){
    free(connection);
  }

  //Free allocated memory before returning
  free(connection);
  return true;
}
