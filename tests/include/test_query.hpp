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

#ifndef test_query_h
#define test_query_h

#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>


// The functions to test
#include "query.h"
#include "connect.h"
#include "disconnect.h"

class TestQuery : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestQuery);
  CPPUNIT_TEST(testResultQuerySuccess);
  CPPUNIT_TEST(testInfoQuerySuccess);
  CPPUNIT_TEST(testInfoQueryFail);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testResultQuerySuccess(void);
  void testInfoQuerySuccess(void);
  void testInfoQueryFail(void);
};

#endif
