#pragma once

#include <cppunit/extensions/HelperMacros.h>

#include "GlobalType.h"

class ParserTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(ParserTest);
	CPPUNIT_TEST(testWhile);
	CPPUNIT_TEST(testIf);
	CPPUNIT_TEST(testAssign);
	CPPUNIT_TEST(testMultiProcedure);
    CPPUNIT_TEST(testNodeType);
    CPPUNIT_TEST(testRelationsOfNodes);
    CPPUNIT_TEST(testModifies);
    CPPUNIT_TEST(testUses);
    CPPUNIT_TEST_SUITE_END();

  public:
    void setUp();
    void tearDown();

	void testMultiProcedure();
	void testAssign();
	void testIf();
	void testWhile();

    void testNodeType();
    void testRelationsOfNodes();
    void testModifies();
    void testUses();

    // supporting functions
    bool isModifies(STMTLINE stmt, VARINDEX varIndex);
    bool isUses(STMTLINE stmt, VARINDEX varIndex);
};