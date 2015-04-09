#pragma once

#include <cppunit/extensions/HelperMacros.h>

#include "Uses.h"
#include "PKB.h"

class ParserUsesTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(ParserUsesTest);
    CPPUNIT_TEST(testUsesStmt);
    CPPUNIT_TEST(testUsesVarIndex);
    CPPUNIT_TEST_SUITE_END();

  private:
    Uses* uTest;
	bool containStmtLine(vector<STMTLINE> *list, STMTLINE s);
	bool containVarIndex(vector<VARINDEX> *list, VARNAME v);
	bool matchSize(vector<VARINDEX> *list, int size);

  public:
    void setUp();
    void tearDown();

    void testUsesStmt();
    void testUsesVarIndex();
};