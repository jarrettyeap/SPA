#pragma once

#include <cppunit/extensions/HelperMacros.h>

#include "Follows.h"

class FollowsTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(FollowsTest);
	CPPUNIT_TEST(testSet);
	CPPUNIT_TEST(testIsFollows);
	CPPUNIT_TEST(testIsFollowsStar);
	CPPUNIT_TEST(testGetFollowsFrom);
	CPPUNIT_TEST(testGetFollowsFromStar);
	CPPUNIT_TEST(testGetFollowedBy);
	CPPUNIT_TEST(testGetFollowedByStar);
	CPPUNIT_TEST_SUITE_END();

public:
	Follows* fTest;
	void setUp();
	void tearDown();

	void testSet();
	void testIsFollows();
	void testIsFollowsStar();
	void testGetFollowsFrom();
	void testGetFollowsFromStar();
	void testGetFollowedBy();
	void testGetFollowedByStar();
};