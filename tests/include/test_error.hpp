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

 #ifndef test_error_h
 #define test_error_h

 #include <cppunit/TestCase.h>
 #include <cppunit/TestFixture.h>
 #include <cppunit/extensions/HelperMacros.h>


 // The functions to test
 #include "error.h"

 class TestError : public CppUnit::TestFixture {
   CPPUNIT_TEST_SUITE(TestError);
   CPPUNIT_TEST(testInitializeErrorSuccess);
   CPPUNIT_TEST(testInitializeErrorSuccessMultiple);
   CPPUNIT_TEST(testInitializeErrorMallocFailure);
   CPPUNIT_TEST(testInitializeErrorMallocFailureMulitple);
   CPPUNIT_TEST(testResetError);
   CPPUNIT_TEST(testResetErrorMultiple);
   CPPUNIT_TEST(testFreeError);
   CPPUNIT_TEST(testFreeErrorMultiple);
   CPPUNIT_TEST(testGeneratePackageErrorSuccess);
   CPPUNIT_TEST(testGeneratePackageErrorSuccessMultiple);
   CPPUNIT_TEST(testGeneratePackageErrorMallocFailure);
   CPPUNIT_TEST(testGeneratePackageErrorMallocFailureMultiple);
   CPPUNIT_TEST(testGenerateDatabaseErrorSuccess);
   CPPUNIT_TEST(testGenerateDatabaseErrorSuccessMultiple);
   CPPUNIT_TEST(testGenerateDatabaseErrorMallocFailure);
   CPPUNIT_TEST(testGenerateDatabaseErrorMallocFailureMultiple);
   CPPUNIT_TEST(testGenerateDatabaseErrorFetchDatabaseErrorFailure);
   CPPUNIT_TEST(testGenerateDatabaseErrorFetchDatabaseErrorFailureMultiple);
   CPPUNIT_TEST(freePackageError);
   CPPUNIT_TEST(freePackageErrorMultiple);
   CPPUNIT_TEST(freeDatabaseError);
   CPPUNIT_TEST(freeDatabaseErrorMultiple);
   CPPUNIT_TEST_SUITE_END();
 protected:
   void testInitializeErrorSuccess(void);
   void testInitializeErrorSuccessMultiple(void);
   void testInitializeErrorMallocFailure(void);
   void testInitializeErrorMallocFailureMulitple(void);
   void testResetError(void);
   void testResetErrorMultiple(void);
   void testFreeError(void);
   void testFreeErrorMultiple(void);
   void testGeneratePackageErrorSuccess(void);
   void testGeneratePackageErrorSuccessMultiple(void);
   void testGeneratePackageErrorMallocFailure(void);
   void testGeneratePackageErrorMallocFailureMultiple(void);
   void testGenerateDatabaseErrorSuccess(void);
   void testGenerateDatabaseErrorSuccessMultiple(void);
   void testGenerateDatabaseErrorMallocFailure(void);
   void testGenerateDatabaseErrorMallocFailureMultiple(void);
   void testGenerateDatabaseErrorFetchDatabaseErrorFailure(void);
   void testGenerateDatabaseErrorFetchDatabaseErrorFailureMultiple(void);
   void freePackageError(void);
   void freePackageErrorMultiple(void);
   void freeDatabaseError(void);
   void freeDatabaseErrorMultiple(void);
 };

 #endif
