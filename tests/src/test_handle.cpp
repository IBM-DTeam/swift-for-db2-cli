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

#include "test_handle.hpp"


/*
 * Function:  Testhandle::testcreateHandle
 * ------------------
 * Tests if the handle are created correctly.
 *
 */
void Testhandle::testcreateHandle(void) {
  handle* h = createHandle();

  CPPUNIT_ASSERT_MESSAGE("The handle struct is NULL", h != NULL);
  CPPUNIT_ASSERT_MESSAGE("The handle struct is h->hDbc NULL", h->hDbc == SQL_NULL_HDBC);
  CPPUNIT_ASSERT_MESSAGE("The handle struct is h->hEnv NULL", h->hEnv == SQL_NULL_HENV);

  freeHandle(&h);

}

/*
 * Function:  Testhandle::testfreeHandle
 * ------------------
 * Tests if the handle are created correctly.
 *
 */
void Testhandle::testfreeHandle(void) {
  handle* h = createHandle();
  freeHandle(&h);

  CPPUNIT_ASSERT_MESSAGE("The handle were not freed", h == NULL);
}
