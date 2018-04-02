/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_BodyHubTestSuite_init = false;
#include "test_suite.h"

static BodyHubTestSuite suite_BodyHubTestSuite;

static CxxTest::List Tests_BodyHubTestSuite = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_BodyHubTestSuite( "test_suite.h", 8, "BodyHubTestSuite", suite_BodyHubTestSuite, Tests_BodyHubTestSuite );

static class TestDescription_suite_BodyHubTestSuite_test_class_variables : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_BodyHubTestSuite_test_class_variables() : CxxTest::RealTestDescription( Tests_BodyHubTestSuite, suiteDescription_BodyHubTestSuite, 24, "test_class_variables" ) {}
 void runTest() { suite_BodyHubTestSuite.test_class_variables(); }
} testDescription_suite_BodyHubTestSuite_test_class_variables;

static class TestDescription_suite_BodyHubTestSuite_test_CHS : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_BodyHubTestSuite_test_CHS() : CxxTest::RealTestDescription( Tests_BodyHubTestSuite, suiteDescription_BodyHubTestSuite, 36, "test_CHS" ) {}
 void runTest() { suite_BodyHubTestSuite.test_CHS(); }
} testDescription_suite_BodyHubTestSuite_test_CHS;

static class TestDescription_suite_BodyHubTestSuite_test_UHS : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_BodyHubTestSuite_test_UHS() : CxxTest::RealTestDescription( Tests_BodyHubTestSuite, suiteDescription_BodyHubTestSuite, 56, "test_UHS" ) {}
 void runTest() { suite_BodyHubTestSuite.test_UHS(); }
} testDescription_suite_BodyHubTestSuite_test_UHS;

static SensorNodeTestSuite suite_SensorNodeTestSuite;

static CxxTest::List Tests_SensorNodeTestSuite = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_SensorNodeTestSuite( "test_suite.h", 75, "SensorNodeTestSuite", suite_SensorNodeTestSuite, Tests_SensorNodeTestSuite );

static class TestDescription_suite_SensorNodeTestSuite_test_class_variables : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_SensorNodeTestSuite_test_class_variables() : CxxTest::RealTestDescription( Tests_SensorNodeTestSuite, suiteDescription_SensorNodeTestSuite, 95, "test_class_variables" ) {}
 void runTest() { suite_SensorNodeTestSuite.test_class_variables(); }
} testDescription_suite_SensorNodeTestSuite_test_class_variables;

static class TestDescription_suite_SensorNodeTestSuite_test_controllerFSM : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_SensorNodeTestSuite_test_controllerFSM() : CxxTest::RealTestDescription( Tests_SensorNodeTestSuite, suiteDescription_SensorNodeTestSuite, 106, "test_controllerFSM" ) {}
 void runTest() { suite_SensorNodeTestSuite.test_controllerFSM(); }
} testDescription_suite_SensorNodeTestSuite_test_controllerFSM;

static class TestDescription_suite_SensorNodeTestSuite_test_statusAnalysis : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_SensorNodeTestSuite_test_statusAnalysis() : CxxTest::RealTestDescription( Tests_SensorNodeTestSuite, suiteDescription_SensorNodeTestSuite, 124, "test_statusAnalysis" ) {}
 void runTest() { suite_SensorNodeTestSuite.test_statusAnalysis(); }
} testDescription_suite_SensorNodeTestSuite_test_statusAnalysis;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
