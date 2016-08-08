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

#include "test_handles.hpp"


/*
 * Function:  TestHandles::testCreateHandles
 * ------------------
 * Tests if the handles are created correctly.
 *
 */
void TestHandles::testCreateHandles(void) {
  handles* h = createHandles();

  CPPUNIT_ASSERT_MESSAGE("The handles struct is NULL", h != NULL);
  CPPUNIT_ASSERT_MESSAGE("The handles struct is h->hDbc NULL", h->hDbc == SQL_NULL_HDBC);
  CPPUNIT_ASSERT_MESSAGE("The handles struct is h->hEnv NULL", h->hEnv == SQL_NULL_HENV);

  freeHandles(&h);

}

/*
 * Function:  TestHandles::testFreeHandles
 * ------------------
 * Tests if the handles are created correctly.
 *
 */
void TestHandles::testFreeHandles(void) {
  handles* h = createHandles();
  freeHandles(&h);

  CPPUNIT_ASSERT_MESSAGE("The handles were not freed", h == NULL);
}
