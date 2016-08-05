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

#ifndef test_connect_h
#define test_connect_h

#include <string>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>


// The functions to test
#include "connect.h"

class TestConnect : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestConnect);
  CPPUNIT_TEST(testValidConnection);
  CPPUNIT_TEST(testValidConnectionMultiple);
  CPPUNIT_TEST(testInvalidConnection);
  CPPUNIT_TEST(testInvalidConnectionMultiple);
  CPPUNIT_TEST(testMixedConnections);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testValidConnection(void);
  void testValidConnectionMultiple(void);
  void testInvalidConnection(void);
  void testInvalidConnectionMultiple(void);
  void testMixedConnections(void);
};

#endif
