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

#ifndef handle_h
#define handle_h

#include <sqlcli1.h>

#include "type.h"

// handle struct
typedef struct handle {
  SQLHDBC hDbc;
  SQLHENV hEnv;
} handle;

// Methods
state createHandle(handle** h);
void freeHandle(handle** h);

#endif
