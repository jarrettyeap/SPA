#pragma once

#include <vector>
#include <map>
#include <string>

#include "GlobalType.h"
#include "TNode.h"

class AST {
  private:
    std::map<STMTLINE, TNode*> stmt2NodeMap;
    std::multimap<STMTLINE, TType> TType2StmtMap;
    std::multimap<STMTLINE,TType>::iterator stmtIt;
    std::pair <std::multimap<STMTLINE,TType>::iterator, std::multimap<STMTLINE,TType>::iterator> stmtRet;
    TNode* procedureRoot; //temp solution until ProcTable is complete

  public:
    AST(void);
    ~AST(void);

    TNode* createTNode(TType nodeType, std::string value);
    bool setSibling(TNode leftNode, TNode rightNode);
    bool addChildTNode(TNode parent, TNode child);
    std::vector<TNode*> getChildren(TNode parent);
    TNode* getLeftSibling(TNode node);
    TNode* getRightSibling(TNode node);
    TNode* getParentTNode(TNode node);
    TNode* getTNode(STMTLINE line);
    bool setRoot(TNode root);
    TNode* getRoot();
    std::string getValue(TNode node);
    bool setStmtLine(TNode node, STMTLINE stmtNumber);
    void addToStmtLineMap(STMTLINE stmtNumber, TType type);
    vector<STMTLINE> getStmtLines(TType type);
    bool isMatch(TNode node, TType type);
    bool patternMatch(STMTLINE assignRoot, std::string expression, bool strict);
    TNode* createExprTree(std::string expression);
};
