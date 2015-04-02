#include <cppunit/config/SourcePrefix.h>
#include <vector>

#include "ASTTest.h"
#include "AST.h"

void ASTTest::setUp() {
    ast = new AST;
}

void ASTTest::tearDown() {
    delete ast;
}

CPPUNIT_TEST_SUITE_REGISTRATION(ASTTest);

void ASTTest::testCreateTNode() {
    TNode* node = (*ast).createTNode(VARN, "x");
    CPPUNIT_ASSERT((*node).getValue() == "x");
}

void ASTTest::testSetSibling() {
    TNode* nodeSiblingLeft = (*ast).createTNode(VARN, "x");
    TNode* nodeSiblingRight = (*ast).createTNode(VARN, "y");
    CPPUNIT_ASSERT((*ast).setSibling(nodeSiblingLeft, nodeSiblingRight));
    CPPUNIT_ASSERT(nodeSiblingRight == (*ast).getRightSibling(nodeSiblingLeft));
    CPPUNIT_ASSERT(nodeSiblingLeft == (*ast).getLeftSibling(nodeSiblingRight));
}

void ASTTest::testAddChildTNode() {
    TNode* nodeStmtLst = (*ast).createTNode(STMTLSTN, "");

    vector<TNode*> childrenLocal;
    TNode* node = (*ast).createTNode(PLUSN, "+");
    childrenLocal.push_back(node);
    (*ast).addChildTNode(nodeStmtLst, node);

    TNode* nodeSiblingLeft = (*ast).createTNode(VARN, "x");
    childrenLocal.push_back(nodeSiblingLeft);
    (*ast).addChildTNode(nodeStmtLst, nodeSiblingLeft);

    TNode* nodeSiblingRight = (*ast).createTNode(VARN, "y");
    childrenLocal.push_back(nodeSiblingRight);
    (*ast).addChildTNode(nodeStmtLst, nodeSiblingRight);

    vector<TNode*> childrenRetrieved = (*ast).getChildren(nodeStmtLst);
    CPPUNIT_ASSERT(childrenRetrieved == childrenLocal);
    CPPUNIT_ASSERT((*ast).getParentTNode(node) == nodeStmtLst);
}

void ASTTest::testSetStmtLine() {
    TNode* node = (*ast).createTNode(PLUSN, "+");
    (*ast).setStmtLine(node, 2);
    CPPUNIT_ASSERT((*ast).getTNode(2) == node);

    (*ast).addToStmtLineMap(PLUSN, 2);
    vector<STMTLINE> stmtLines = (*ast).getStmtLines(PLUSN) ;
    CPPUNIT_ASSERT(stmtLines.size() == 1);
    CPPUNIT_ASSERT(stmtLines[0] == 2);
}

void ASTTest::testSetRoot() {
    TNode* node = (*ast).createTNode(PLUSN, "+");
    (*ast).setRoot(node);
    (*ast).setStmtLine(node, 2);
    (*ast).addToStmtLineMap(PLUSN, 2);
    vector<STMTLINE> stmtLines = (*ast).getStmtLines(PLUSN) ;
    CPPUNIT_ASSERT((*ast).getRoot() == node);

}

void ASTTest::testIsMatch() {
    TNode* node = (*ast).createTNode(VARN, "x");
    CPPUNIT_ASSERT((*ast).isMatch(node, VARN));

}
void ASTTest::testPatternMatch() {
    TNode* nodeStmtLst = (*ast).createTNode(STMTLSTN, "");
    vector<TNode*> childrenLocal;
    TNode* node = (*ast).createTNode(PLUSN, "+");
    childrenLocal.push_back(node);
    TNode* nodeSiblingLeft = (*ast).createTNode(VARN, "x");
    childrenLocal.push_back(nodeSiblingLeft);
    TNode* nodeSiblingRight = (*ast).createTNode(VARN, "y");
    childrenLocal.push_back(nodeSiblingRight);
    (*ast).setSibling(nodeSiblingLeft, nodeSiblingRight);
    (*nodeStmtLst).addChild(node);

    CPPUNIT_ASSERT((*ast).patternMatch(2, "x+y",true));
}
