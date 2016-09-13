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

#include "test_main.hpp"

// Register the tests to run.
CPPUNIT_TEST_SUITE_REGISTRATION(TestHandle);
CPPUNIT_TEST_SUITE_REGISTRATION(TestError);
CPPUNIT_TEST_SUITE_REGISTRATION(TestDatabase);
CPPUNIT_TEST_SUITE_REGISTRATION(TestConnect);
CPPUNIT_TEST_SUITE_REGISTRATION(TestDisconnect);
CPPUNIT_TEST_SUITE_REGISTRATION(TestQuery);
CPPUNIT_TEST_SUITE_REGISTRATION(TestTransaction);
CPPUNIT_TEST_SUITE_REGISTRATION(TestPrepared);

/*
 * Function:  main
 * ------------------
 * Executes the registered tests.
 *
 */
int main() {

  CPPUNIT_NS::TestResult testResult;

  CPPUNIT_NS::TestResultCollector collectedResults;
  testResult.addListener(&collectedResults);

  CPPUNIT_NS::BriefTestProgressListener progress;
  testResult.addListener(&progress);

  CPPUNIT_NS::TestRunner testrunner;
  testrunner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
  testrunner.run(testResult);

  CPPUNIT_NS::CompilerOutputter compilerOutputter(&collectedResults, std::cerr);
  compilerOutputter.write();

  return collectedResults.wasSuccessful() ? 0 : 1;

}
