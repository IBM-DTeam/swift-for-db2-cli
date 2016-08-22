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

#ifndef test_database_h
#define test_database_h

#include <string>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

// The functions to test
#include "database.h"

class TestDatabase : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestDatabase);
  CPPUNIT_TEST(testCreateDatabaseSuccess);
  CPPUNIT_TEST(testCreateDatabaseSuccessMultiple);
  CPPUNIT_TEST(testCreateDatabaseMallocFailure);
  CPPUNIT_TEST(testCreateDatabaseMallocFailureMultiple);
  CPPUNIT_TEST(testFreeDatabase);
  CPPUNIT_TEST(testFreeDatabaseMultiple);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testCreateDatabaseSuccess(void);
  void testCreateDatabaseSuccessMultiple(void);
  void testCreateDatabaseMallocFailure(void);
  void testCreateDatabaseMallocFailureMultiple(void);
  void testFreeDatabase(void);
  void testFreeDatabaseMultiple(void);
};

#endif
