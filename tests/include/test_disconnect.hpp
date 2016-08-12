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

#ifndef test_disconnect_h
#define test_disconnect_h

#include <string>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "connect.h"


// The functions to test
#include "disconnect.h"

class TestDisconnect : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestDisconnect);
  CPPUNIT_TEST(testDisconnectFailure);
  CPPUNIT_TEST(testDisconnectFailureMultiple);
  CPPUNIT_TEST(testDisconnectSuccess);
  CPPUNIT_TEST(testDisconnectSuccessMultiple);
  CPPUNIT_TEST(testDisconnectNoDatabase);
  CPPUNIT_TEST(testDisconnectNoDatabaseMultiple);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testDisconnectFailure(void);
  void testDisconnectFailureMultiple(void);
  void testDisconnectSuccess(void);
  void testDisconnectSuccessMultiple(void);
  void testDisconnectNoDatabase(void);
  void testDisconnectNoDatabaseMultiple(void);
};

#endif
