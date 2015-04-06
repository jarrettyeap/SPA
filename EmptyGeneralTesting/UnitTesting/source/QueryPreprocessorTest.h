#pragma once

#include <cppunit/extensions/HelperMacros.h>

#include "QueryPreprocessor.h"

class QueryPreprocessorTest : public CPPUNIT_NS::TestFixture{

	CPPUNIT_TEST_SUITE(QueryPreprocessorTest);
    CPPUNIT_TEST(testParsing);
	CPPUNIT_TEST(testNoCondition);
	CPPUNIT_TEST(testNoConditionMoreComplex);
	CPPUNIT_TEST(testOneCondition);
	CPPUNIT_TEST(testOneConditionTwoSynonyms);
	CPPUNIT_TEST(testPatternCondition);
    CPPUNIT_TEST_SUITE_END();

  private:
	  QueryPreprocessor* queryTest;
  public:
    void setUp();
    void tearDown();
	void testParsing();
	void testNoCondition();
	void testNoConditionMoreComplex();
	void testOneCondition();
	void testOneConditionTwoSynonyms();
	void testPatternCondition();
};