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

#ifndef test_getData_h
#define test_getData_h

#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>


// The functions to test
#include "query.h"
#include "connect.h"
#include "disconnect.h"
#include "result.h"

class TestGetData : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestGetData);
  CPPUNIT_TEST(testGetColumnAndRow);
  CPPUNIT_TEST(testGetNextRow);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testGetColumnAndRow(void);
  void testGetNextRow(void);

};

#endif
