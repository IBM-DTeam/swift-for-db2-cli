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

#include "handle.h"


/*
 * Function:  createHandle
 * ------------------
 * Malloc's a handle struct
 *
 * returns:
 *   SUCCESS: The handle struct was created.
 *   MALLOC_FAILURE: Cannot create the handle struct due to a failed malloc.
 */
state createHandle(handle** h) {

  //Malloc handle
  *h = (handle*) malloc(sizeof(handle));
  if (*h == NULL)
    return MALLOC_FAILURE;

  // Initialize the variables to their respective NULLs.
  (*h)->hDbc = SQL_NULL_HDBC; // Connection handle
  (*h)->hEnv = SQL_NULL_HENV; // Statement handle

  return SUCCESS;

}

/*
 * Function:  freeHandle
 * ------------------
 * Frees a handle struct.
 *
 */
void freeHandle(handle** h) {

  if (*h == NULL)
    return;

  // Only free the database connection handle if it isn't NULL.
  if ((*h)->hDbc != SQL_NULL_HDBC)
    SQLFreeHandle(SQL_HANDLE_DBC, (*h)->hDbc);

  // Only free the environment handle if it isn't NULL.
  if ((*h)->hEnv != SQL_NULL_HENV)
    SQLFreeHandle(SQL_HANDLE_ENV, (*h)->hEnv);

  // Free the handle struct.
  free(*h);

  // Set the pointer to NULL, so users don't try to use it again.
  *h = NULL;
  
}
