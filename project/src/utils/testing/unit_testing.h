/*******************************************************************************
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
Description : unit testing tool
*******************************************************************************/
#pragma once

#include "unit_testing_toolset.hpp"


// ----------------
// -- how to use --
// ----------------
/*
// Define test case group with BEGIN_UNIT_TEST and END_UNIT_TEST in source file (.cpp).
// Between BEGIN_UNIT_TEST and END_UNIT_TEST, only tests can be declared (CREATE_UNIT_TEST_PROCEDURE) and settings (SET_PROCEDURE_CALLS).

// In the program entry point, you can execute a test case group this way :
        unit_testing::yourUnitTestName(false);  // simple execution
        unit_testing::yourUnitTestName(true);   // execute with concurrency check (tests executed multiple times with multiple threads).
        
// If the program entry point is in a different source file, use DECLARE_UNIT_TEST_HEADER(unitTestName) in a header file (.h) and include it.

// Note that you can execute special procedures before/after the tests:
//      - once before/after the whole test case group, by using more arguments in BEGIN_UNIT_TEST :
//        BEGIN_UNIT_TEST(myUnitTest1, "myUnitTest1", myBeforeUnit, myAfterUnit);
//      - before/after each test, by declaring SET_PROCEDURE_CALLS once, just after BEGIN_UNIT_TEST :
//        SET_PROCEDURE_CALLS(myBeforeEach, myAfterEach);
//      - before/after one specific test, by using more arguments in CREATE_UNIT_TEST_PROCEDURE :
//        CREATE_UNIT_TEST_PROCEDURE("myTest", myBefore, myAfter, { ... });
// Use lambdas ([](void){ ... }) or std::function<void()> for the procedures to call before/after something.

// Inside the tests, use asserts to check the validity of your unit. See list of asserts below, at the end of this file.


// - simple example -
// -----------------

BEGIN_UNIT_TEST(myUnitTest1, "myUnitTest1");  // required, before first procedure

    CREATE_UNIT_TEST_PROCEDURE("myTest", {    // example of test
        int i = 2;
        ASSERT_EQ(i, 2);
        return true;
    });

END_UNIT_TEST();    // required, after last procedure

void main()         // program entry point
{
    unit_testing::myUnitTest1(false);  // start unit testing group (execute all procedures, with concurrency set to false)
}


// - advanced example -
// --------------------

BEGIN_UNIT_TEST(myUnitTest2, "myUnitTest2", 
                [](void) { printf("start"); },            // lambda called before whole test case
                [](void) { printf("end"); });             // lambda called after whole test case
SET_PROCEDURE_CALLS([](void) { printf("new test"); },     // called before each test
                    [](void) { printf("end of test"); }); // called after each test

    CREATE_UNIT_TEST_PROCEDURE("mySimpleTest", {
        int i = 2;
        ASSERT_EQ(i, 2);
        return true;
    });
    CREATE_UNIT_TEST_PROCEDURE("myTestWithBeforeAfter", 
                               [](void) { printf("special test started"); }, // called only before this specific test
                               [](void) { printf("special test ended"); },   // called only after this specific test
    {
        bool boolArray[] = { true, false };
        ASSERT_TRUE(boolArray[0]);
        ASSERT_ARRAY_CONTAINS(false);
        return true;
    });

END_UNIT_TEST();

void main()
{
    unit_testing::myUnitTest2(true);
}
*/


// -- macros to declare test procedures --

// declare unit test case (h/hpp file, if called from a different file)
#define DECLARE_UNIT_TEST_HEADER(unitTestName)                                     namespace unit_testing { void unitTestName(bool isConcurrency); }

// create unit test case (cpp file)
#define BEGIN_UNIT_TEST(unitTestName,unitTestNameString)                           _UTT_BEGIN_UNIT_TEST(unitTestName,unitTestNameString,[](void){},[](void){})
#define BEGIN_UNIT_TEST(unitTestName,unitTestNameString,callBefore,callAfter)      _UTT_BEGIN_UNIT_TEST(unitTestName,unitTestNameString,callBefore,callAfter)
#define END_UNIT_TEST()                                                            _UTT_END_UNIT_TEST()

// set functions to call before/after each test
#define SET_PROCEDURE_CALLS(callBeforeEach, callAfterEach)                         _UTT_SET_PROCEDURE_CALLS(callBeforeEach, callAfterEach)
// add procedure to unit test
#define CREATE_UNIT_TEST_PROCEDURE(testNameString,procedure)                       _UTT_CREATE_UNIT_TEST_PROCEDURE(testNameString,[](void){},[](void){},procedure)
#define CREATE_UNIT_TEST_PROCEDURE(testNameString,callBefore,callAfter,procedure)  _UTT_CREATE_UNIT_TEST_PROCEDURE(testNameString,callBefore,callAfter,procedure)
// add procedure to unit test - unless concurrency enabled
#define CREATE_UNIT_TEST_PROCEDURE_NO_CONCURRENCY(testNameString,procedure)                       if(!isConcurrency) { _UTT_CREATE_UNIT_TEST_PROCEDURE(testNameString,[](void){},[](void){},procedure); }
#define CREATE_UNIT_TEST_PROCEDURE_NO_CONCURRENCY(testNameString,callBefore,callAfter,procedure)  if(!isConcurrency) { _UTT_CREATE_UNIT_TEST_PROCEDURE(testNameString,callBefore,callAfter,procedure); }


// -- macros to use in tests --

// exceptions
#define ASSERT_THROWS(test)                  try { (test); _UTT_FAILED_UNIT_TEST(); } catch (...) {}
#define ASSERT_NO_THROW(test)                try { (test); } catch (...) { _UTT_FAILED_UNIT_TEST(); }

// booleans
#define ASSERT_TRUE(test)                    _UTT_CHECK_TEST(test,    std::string("expected: true ; value: false"))
#define ASSERT_FALSE(test)                   _UTT_CHECK_TEST(!(test), std::string("expected: false ; value: true"))

// comparisons
#define ASSERT_EQ(test,val)                  _UTT_CHECK_TEST((test == val), (std::string("expected == ")+val)+std::string("; value: ")+test)
#define ASSERT_NOT_EQ(test,val)              _UTT_CHECK_TEST((test != val), (std::string("expected != ")+val)+std::string("; value: ")+test)
#define ASSERT_LOWER(test,val)               _UTT_CHECK_TEST((test < val),  (std::string("expected < ")+val)+std::string("; value: ")+test)
#define ASSERT_LOWER_EQ(test,val)            _UTT_CHECK_TEST((test <= val), (std::string("expected <= ")+val)+std::string("; value: ")+test)
#define ASSERT_GREATER(test,val)             _UTT_CHECK_TEST((test > val),  (std::string("expected > ")+val)+std::string("; value: ")+test)
#define ASSERT_GREATER_EQ(test,val)          _UTT_CHECK_TEST((test >= val), (std::string("expected >= ")+val)+std::string("; value: ")+test)

// objects state
#define ASSERT_NULL(obj)                     _UTT_CHECK_TEST(utils::testing::ObjectVerifier::isNull(obj),                std::string("expected: nullptr ; value: not null"))
#define ASSERT_NOT_NULL(obj)                 _UTT_CHECK_TEST(!utils::testing::ObjectVerifier::isNull(obj),               std::string("expected: not null; value: nullptr"))
#define ASSERT_INSTANCE(class,obj)           _UTT_CHECK_TEST(utils::testing::ObjectVerifier::isInstanceOf<class>(obj),   std::string("not an instance of specified class"))
#define ASSERT_NOT_INSTANCE(class,obj)       _UTT_CHECK_TEST(!utils::testing::ObjectVerifier::isInstanceOf<class>(obj),  std::string("is instance of specified class"))

// strings
#define ASSERT_STR_EMPTY(str)                _UTT_CHECK_TEST(utils::testing::StringVerifier::isEmpty(str),         std::string("expected: \"\"; value: \")+str+std::string("\""))
#define ASSERT_STR_NOT_EMPTY(str)            _UTT_CHECK_TEST(!utils::testing::StringVerifier::isEmpty(str),        std::string("expected: not empty; value: \"\""))
#define ASSERT_STR_EQ(str,val)               _UTT_CHECK_TEST(utils::testing::StringVerifier::equals(str,val),      std::string("expected: \"")+val+std::string("\"; value: \")+str+std::string("\""))
#define ASSERT_STR_NOT_EQ(str,val)           _UTT_CHECK_TEST(!utils::testing::StringVerifier::equals(str,val),     std::string("expected: != \"")+val+std::string("\"; value: \")+str+std::string("\""))
#define ASSERT_STR_START(str,val)            _UTT_CHECK_TEST(utils::testing::StringVerifier::startsWith(str,val),  std::string("expected: \"")+val+std::string("\"[...]; value: \")+str+std::string("\""))
#define ASSERT_STR_NOT_START(str,val)        _UTT_CHECK_TEST(!utils::testing::StringVerifier::startsWith(str,val), std::string("expected: \"")+val+std::string("\"[...]; value: \")+str+std::string("\""))
#define ASSERT_STR_END(str,val)              _UTT_CHECK_TEST(utils::testing::StringVerifier::endsWith(str,val),    std::string("expected: [...]\"")+val+std::string("\"; value: \")+str+std::string("\""))
#define ASSERT_STR_NOT_END(str,val)          _UTT_CHECK_TEST(!utils::testing::StringVerifier::endsWith(str,val),   std::string("expected: [...]\"")+val+std::string("\"; value: \")+str+std::string("\""))
#define ASSERT_STR_CONTAINS(str,val)         _UTT_CHECK_TEST(utils::testing::StringVerifier::contains(str,val),    std::string("expected: [...]\"")+val+std::string("\"[...]; value: \")+str+std::string("\""))
#define ASSERT_STR_NOT_CONTAINS(str,val)     _UTT_CHECK_TEST(!utils::testing::StringVerifier::contains(str,val),   std::string("expected: [...]\"")+val+std::string("\"[...]; value: \")+str+std::string("\""))

// arrays / hashes
#define ASSERT_ARRAY_EMPTY(array)            _UTT_CHECK_TEST(utils::testing::CollectionVerifier::isEmpty(array),           std::string("expected: {}; value: not empty"))
#define ASSERT_ARRAY_NOT_EMPTY(array)        _UTT_CHECK_TEST(!utils::testing::CollectionVerifier::isEmpty(array),          std::string("expected: not empty; value: {}"))
#define ASSERT_ARRAY_CONTAINS(array,val)     _UTT_CHECK_TEST(utils::testing::CollectionVerifier::contains(array,val),      std::string("value not found in array"))
#define ASSERT_ARRAY_CONTAINS(array,key,val) _UTT_CHECK_TEST(utils::testing::CollectionVerifier::contains(array,key,val),  std::string("key or value not found in hash"))
#define ASSERT_ARRAY_LACKS(array,val)        _UTT_CHECK_TEST(!utils::testing::CollectionVerifier::contains(array,val),     std::string("excluded value is in array"))
#define ASSERT_ARRAY_LACKS(array,key,val)    _UTT_CHECK_TEST(!utils::testing::CollectionVerifier::contains(array,key,val), std::string("excluded key and value is in hash"))
