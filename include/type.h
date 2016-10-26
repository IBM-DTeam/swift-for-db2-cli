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

#ifndef db_type_h
#define db_type_h

#define SUCCESS 1
#define SUCCESS_WITH_INFO 2
#define CATASTROPHIC_FAILURE -1
#define MALLOC_FAILURE -2
#define SETUP_DATABASE_FAILURE -3
#define FETCH_DATABASE_ERROR_FAILURE -4
#define DATABASE_DISCONNECT_FAILURE -5
#define NO_DATABASE_FOUND -6
#define DATABASE_EXISTS -7
#define STATEMENT_HANDLE_EXISTS -8
#define QUERY_FAILURE -9
#define STMT_HANDLE_SETUP_FAILURE -10
#define PARAMETER_BIND_FAILURE -11
#define QUERY_EXECUTION_FAILURE -12
#define GET_NUM_COLUMNS_FAILURE -13
#define GET_NUM_ROWS_FAILURE -14
#define DESCRIBE_COL_FAILURE -15
#define CANNOT_FETCH_ROW -16
#define CANNOT_GET_DATA -17
#define SET_CONNECTION_ATTR_FAIL -18
#define COMMIT_TRANSACTION_FAILURE -19
#define ROLLBACK_TRANSACTION_FAILURE -20
#define SQL_PREPARE_FAILURE -21
#define NUM_PARAMETERS_FAILURE -22
#define DESCRIBE_PARAMETER_FAILURE -23
#define STATEMENT_HANDLE_NONEXISTANT -24
#define NO_MORE_ROWS -25
#define DATA_RETRIVAL_FAILURE -26

// Type of query
#define RECIEVE 3
#define INFO 4
#define OTHER 5

typedef int state;
typedef int queryType;

#endif
