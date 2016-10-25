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
 * Function:  TestHandle::testCreateHandleSuccess
 * ------------------
 * Create a Handle struct, successfully.
 *
 */
void TestHandle::testCreateHandleSuccess(void) {

  // Create the handle
  handle* h = NULL;
  state s = db_createHandle(&h);

  // Clean up
  db_freeHandle(&h);

  CPPUNIT_ASSERT_MESSAGE("Couldn't create the handle.", s == SUCCESS);

}

/*
 * Function:  TestHandle::testCreateHandleSuccessMultiple
 * ------------------
 * Create a Handle struct, successfully, multiple times.
 *
 */
void TestHandle::testCreateHandleSuccessMultiple(void) {

  int i = 0;
  for (; i < 5; i++)
    testCreateHandleSuccess();

}

/*
 * Function:  TestHandle::testCreateHandleMallocFailure
 * ------------------
 * Create a Handle struct, but fail due to a malloc issue.
 *
 */
void TestHandle::testCreateHandleMallocFailure(void) {

  // Nothing here for now.
  CPPUNIT_ASSERT_MESSAGE("Stub", 1);

}

/*
 * Function:  TestHandle::testCreateHandleMallocFailureMultiple
 * ------------------
 * Create a Handle struct, but fail due to a malloc issue, multiple times.
 *
 */
void TestHandle::testCreateHandleMallocFailureMultiple(void) {

  int i = 0;
  for (; i < 5; i++)
    testCreateHandleMallocFailure();

}

/*
 * Function:  TestHandle::testFreeHandle
 * ------------------
 * Creates and frees a basic handle struct, multiple times.
 *
 */
void TestHandle::testFreeHandle(void) {

  // Create the handle
  handle* h = NULL;
  state s = db_createHandle(&h);

  // Clean up
  db_freeHandle(&h);

  CPPUNIT_ASSERT_MESSAGE("Couldn't create the handle.", s == SUCCESS);
  CPPUNIT_ASSERT_MESSAGE("Couldn't handle wasn't freed properly.", h == NULL);

}

/*
 * Function:  TestHandle::testFreeHandleMultiple
 * ------------------
 * Creates and frees a basic handle struct, multiple times.
 *
 */
void TestHandle::testFreeHandleMultiple(void) {

  int i = 0;
  for (; i < 5; i++)
    testFreeHandle();

}
