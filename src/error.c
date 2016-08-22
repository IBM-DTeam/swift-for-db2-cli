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

#include "error.h"

/*
 * Function:  initializeError
 * ------------------
 * Setups up the error struct
 *
 * e: Where to create the error struct
 *
 * returns:
 *   SUCCESS: The struct was created.
 *   MALLOC_FAILURE: Failed to malloc the struct.
 */
state initializeError(error** e) {

  *e = (error*) malloc(sizeof(error));
  if (*e == NULL)
    return MALLOC_FAILURE;

  // Set the default values
  (*e)->errorType = NO_ERROR;
  (*e)->package = NULL;
  (*e)->database = NULL;

  return SUCCESS;

}

/*
 * Function:  resetError
 * ------------------
 * Resets the error to a clean state.
 *
 * e: The error struct to reset.
 *
 */
void resetError(error* e) {

  // Resets the error respective to its current state.
  if (e->errorType == PACKAGE_ERROR)
    freePackageError(e);
  else if (e->errorType == DATABASE_ERROR)
    freeDatabaseError(e);

  // Reset to default values
  e->errorType = NO_ERROR;
  e->package = NULL;
  e->database = NULL;

}

/*
 * Function:  freeError
 * ------------------
 * Free the memory of an error.
 *
 * e: The error struct to free.
 *
 */
void freeError(error** e) {

  if (*e == NULL)
    return;

  resetError(*e);
  free(*e);

  *e = NULL;

}

/*
 * Function:  generatePackageError
 * ------------------
 * Creates an error for the package for the user to read.
 *
 * e: The error struct to store the file in.
 * file: Which file the error occured.
 * line: Line of the file the error occured, parameter should be "__LINE__" macro.
 * message: Human readable error message.
 *
 * returns:
 *   SUCCESS: The error was generated.
 *   MALLOC_FAILURE: Error could not be generated due to a malloc failure.
 */
state generatePackageError(error* e, const char* file, int line, const char* message) {

  // Reset the error
  resetError(e);

  // The error type is from the package.
  e->errorType = PACKAGE_ERROR;

  e->package = (packageError*) malloc(sizeof(packageError));
  if (e->package == NULL) {
    e->errorType = NO_ERROR;
    return MALLOC_FAILURE;
  }

  // Shared library version.
  e->package->version = VERSION;

  // File where the error originated.
  e->package->file = (char*) malloc(sizeof(file) + 1);
  if (e->package->file == NULL) {
    e->errorType = NO_ERROR;
    free(e->package);
    e->package = NULL;
    return MALLOC_FAILURE;
  }
  strcpy(e->package->file, file);

  // Line of the file where the error originated.
  e->package->line = line;

  // Message with the error.
  e->package->message = (char*) malloc(sizeof(message) + 1);
  if (e->package->message == NULL) {
    e->errorType = NO_ERROR;
    free(e->package->file);
    free(e->package);
    e->package = NULL;
    return MALLOC_FAILURE;
  }
  strcpy(e->package->message, (char*) message);

  return SUCCESS;

}

/*
 * Function:  generateDatabaseError
 * ------------------
 * Creates an error for the database for the user to read.
 *
 * db: The database struct to read the database error in aswell as to
 *     set the error in.
 *
 * returns:
 *   SUCCESS: The error was generated.
 *   MALLOC_FAILURE: Error could not be generated due to a malloc failure.
 *   FETCH_DATABASE_ERROR_FAILURE: Cannot get the error from the database.
 */
state generateDatabaseError(error* e, handle* h) {

  // Reset the error
  resetError(e);

  // The error type is from the ODBC.
  e->errorType = DATABASE_ERROR;

  // Variables needed by ODBC for fetching database errors
  SQLINTEGER i = 0;
  SQLINTEGER native;
  SQLCHAR state[7];
  SQLCHAR desc[256];
  SQLSMALLINT len;
  SQLRETURN retCode;

  // Do environment handle first
  do {
    retCode = SQLGetDiagRec(SQL_HANDLE_ENV, h->hEnv, ++i, state, &native, desc,
                        sizeof(desc), &len );
    if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {

      // If the head of the error linked list is empty.
      if (e->database == NULL) {

        // Malloc a databaseError struct.
        e->database = (databaseError*) malloc(sizeof(databaseError));
        if (e->database == NULL) {
          e->errorType = NO_ERROR;
          return MALLOC_FAILURE;
        }

        // Malloc the state string.
        e->database->state = (char*) malloc(sizeof(SQLCHAR) * 7);
        if (e->database->state == NULL) {
          e->errorType = NO_ERROR;
          free(e->database);
          e->database = NULL;
          return MALLOC_FAILURE;
        }
        strcpy(e->database->state, (char*) state);

        // Set the native variable.
        e->database->native = native;

        // Malloc the description string.
        e->database->desc = (char*) malloc(sizeof(SQLCHAR) * 256);
        if (e->database->desc == NULL) {
          e->errorType = NO_ERROR;
          free(e->database->state);
          free(e->database);
          e->database = NULL;
          return MALLOC_FAILURE;
        }
        strcpy(e->database->desc, (char*) desc);

        // Set the next error to NULL.
        e->database->next = NULL;
      } else {

        // Find the next empty position in the error linked list.
        databaseError* position = e->database;
        while (position->next != NULL)
          position = position->next;

        // Malloc the next databaseError struct.
        position->next = (databaseError*) malloc(sizeof(databaseError));
        if (position->next == NULL) {
          resetError(e);
          return MALLOC_FAILURE;
        }

        // Malloc the state string.
        position->next->state = (char*) malloc(sizeof(SQLCHAR) * 7);
        if (position->next->state == NULL) {
          free(position->next);
          position->next = NULL;
          resetError(e);
          return MALLOC_FAILURE;
        }
        strcpy(position->next->state, (char*) state);

        // Set the variable native in the struct.
        position->next->native = native;

        // Malloc the description string.
        position->next->desc = (char*) malloc(sizeof(SQLCHAR) * 256);
        if (position->next->desc == NULL) {
          free(position->next->state);
          free(position->next);
          position->next = NULL;
          resetError(e);
          return MALLOC_FAILURE;
        }
        strcpy(position->next->desc, (char*) desc);

        // Set the next databaseError to NULL.
        position->next->next = NULL;

      }

    } else {
      // Clear the struct on a database error.
      if (e->database == NULL)
        e->errorType = NO_ERROR;
      resetError(e);
      return FETCH_DATABASE_ERROR_FAILURE;
    }

  } while( retCode == SQL_SUCCESS );

  // Do database handle next
  do {
    retCode = SQLGetDiagRec(SQL_HANDLE_DBC, h->hDbc, ++i, state, &native, desc,
                        sizeof(desc), &len );
    if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {

      // If the head of the error linked list is empty.
      if (e->database == NULL) {

        // Malloc a databaseError struct.
        e->database = (databaseError*) malloc(sizeof(databaseError));
        if (e->database == NULL) {
          e->errorType = NO_ERROR;
          return MALLOC_FAILURE;
        }

        // Malloc the state string.
        e->database->state = (char*) malloc(sizeof(SQLCHAR) * 7);
        if (e->database->state == NULL) {
          e->errorType = NO_ERROR;
          free(e->database);
          e->database = NULL;
          return MALLOC_FAILURE;
        }
        strcpy(e->database->state, (char*) state);

        // Set the structs native variable.
        e->database->native = native;

        // Malloc the description string.
        e->database->desc = (char*) malloc(sizeof(SQLCHAR) * 256);
        if (e->database->desc == NULL) {
          e->errorType = NO_ERROR;
          free(e->database->state);
          free(e->database);
          e->database = NULL;
          return MALLOC_FAILURE;
        }
        strcpy(e->database->desc, (char*) desc);

        // Set the next databaseError to NULL.
        e->database->next = NULL;
      } else {

        // Find the next empty position in the error linked list.
        databaseError* position = e->database;
        while (position->next != NULL)
          position = position->next;

        // Malloc the next databaseError.
        position->next = (databaseError*) malloc(sizeof(databaseError));
        if (position->next == NULL) {
          resetError(e);
          return MALLOC_FAILURE;
        }

        // Malloc the state string.
        position->next->state = (char*) malloc(sizeof(SQLCHAR) * 7);
        if (position->next->state == NULL) {
          free(position->next);
          position->next = NULL;
          resetError(e);
          return MALLOC_FAILURE;
        }
        strcpy(position->next->state, (char*) state);

        // Set the structs native variable.
        position->next->native = native;

        // Malloc the desc string.
        position->next->desc = (char*) malloc(sizeof(SQLCHAR) * 256);
        if (position->next->desc == NULL) {
          free(position->next->state);
          free(position->next);
          position->next = NULL;
          resetError(e);
          return MALLOC_FAILURE;
        }
        strcpy(position->next->desc, (char*) desc);

        // Set the next error to NULL.
        position->next->next = NULL;

      }

    } else {
      // Clear the struct on a database error.
      if (e->database == NULL)
        e->errorType = NO_ERROR;
      resetError(e);
      return FETCH_DATABASE_ERROR_FAILURE;
    }

  } while( retCode == SQL_SUCCESS );

  return SUCCESS;

}

/*
 * Function:  freePackageError
 * ------------------
 * Frees the packageError struct.
 *
 * e: The error struct to free packageError in.
 */
void freePackageError(error* e) {

  free(e->package->message);
  free(e->package->file);
  free(e->package);

}

/*
 * Function:  freeDatabaseError
 * ------------------
 * Frees the databaseError struct.
 *
 * e: The error struct to free databaseError in.
 */
void freeDatabaseError(error* e) {

  databaseError* head = e->database;
  databaseError* temp;

  while (head != NULL) {
    temp = head;
    head = head->next;
    free(temp->state);
    free(temp->desc);
    free(temp);
  }

}
