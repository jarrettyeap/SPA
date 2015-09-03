#include "AST.h"

#include <stack>
#include <stdlib.h>

#include "Parser.h"
#include "PKB.h"

/* Constructor & Destructor */
AST::AST(void) {
}

AST::~AST(void) {

}


void AST::setRelationShip(TNode* node){
	for(int i = 0; i < node->getChildren().size(); i++){
		
		TNode* child = node->getChildren()[i];
		if(child->getStmtLine() != -1){
			this->setStmtLine(child, child->getStmtLine());
		}
		this->setRelationShip(child);
	}

}

TNode* AST::createTNode(TType nodeType, std::string value) {
    TNode* node = new TNode();
	node->setTType(nodeType);
	node->setNodeValue(value);
	return node;
}

bool AST::setSibling(TNode* leftNode, TNode* rightNode) {
    if((*leftNode).getRightSibling()->getTType() != EMPTYN) {
        return false;
    }
    if((*rightNode).getLeftSibling()->getTType() != EMPTYN) {
        return false;
    }
    (*leftNode).setRightSibling(rightNode);
    (*rightNode).setLeftSibling(leftNode);
    return true;
}

bool AST::addChildTNode(TNode* parent, TNode* child) {
	if((*child).getParentNode()->getTType() != EMPTYN) {
        return false;
    }

    /* Set two-way link of parent and child */
    (*parent).addChild(child);
    (*child).setParentNode(parent);
    return true;
}

vector<TNode*> AST::getChildren(TNode* parent) {
    return (*parent).getChildren();

}

TNode* AST::getLeftSibling(TNode* node) {
    return (*node).getLeftSibling();
}

TNode* AST::getRightSibling(TNode* node) {
    return (*node).getRightSibling();
}

TNode* AST::getParentTNode(TNode* node) {
    return (*node).getParentNode();
}

TNode* AST::getTNode(STMTLINE line) {
    std::map<STMTLINE,TNode*>::iterator it = stmt2NodeMap.find(line);
    if(it == stmt2NodeMap.end()) {
        return NULL;
    } else {
        return it->second;
    }
}

string AST::getValue(TNode* node) {
    return (*node).getValue();
}

bool AST::setStmtLine(TNode* node, STMTLINE stmtNumber) {
	map<STMTLINE, TNode*>::iterator it = stmt2NodeMap.find(stmtNumber);
	if(it != stmt2NodeMap.end()) return false;
    stmt2NodeMap[stmtNumber] = node;
    return true;
}

bool AST::setRoot(TNode* root) {
    rootNode = root;
	this->setRelationShip(root);
    return true;
}

TNode* AST::getRoot() {
    return rootNode;
}

void AST::addToStmtLineMap(TType type, STMTLINE stmtNumber) {
    TType2StmtMap.insert(std::pair<TType,STMTLINE>(type, stmtNumber));
}

vector<STMTLINE> AST::getStmtLines(TType type) {
    vector<STMTLINE> stmtList;
    if(type == TType::STMTN) {
        for(stmtIt = TType2StmtMap.begin(); stmtIt != TType2StmtMap.end(); ++stmtIt) {
            stmtList.push_back(stmtIt->second);
        }
    } else {
        stmtRet = TType2StmtMap.equal_range(type);
        for(stmtIt = stmtRet.first; stmtIt != stmtRet.second; ++stmtIt) {
            stmtList.push_back(stmtIt->second);
        }
    }
    return stmtList;
}

bool AST::isMatch(TNode* node, TType type) {
    return (*node).getTType() == type;
}

bool AST::matchLeftPattern(STMTLINE stmtRoot, VARINDEX varToMatch) {
    vector<TNode*> childList = getTNode(stmtRoot)->getChildren();
    if(childList.empty() || childList[0]->getTType() != VARN) {
        return false;
    } else {
        return std::atoi(childList[0]->getValue().c_str()) == varToMatch;
    }
	return false;
}

bool AST::matchRightPattern(STMTLINE stmtRoot, std::string expression, bool strict) {

	return false;

    /*vector<TNode*> childList = getTNode(stmtRoot)->getChildren();
    if(childList.size() < 2 || childList[0]->getTType() != VARN) {
        return false;
    }

    TNode* astNode = childList[1];
    TNode* queryExpression = Parser::buildExprAST(expression);
    if(queryExpression == nullptr)
        return false;

    vector<TNode*> depthTraversalOfAstNode = getDFS(astNode);
    vector<TNode*> depthTraversalOfQuery = getDFS(queryExpression);
    int lengthOfTraversedAST = depthTraversalOfAstNode.size();
    int lengthOfPattern = depthTraversalOfQuery.size();
    string traversedASTString = convertTNodeListValueToString(depthTraversalOfAstNode);
    string traversedQueryString = convertTNodeListValueToString(depthTraversalOfQuery);

    if(strict) {
        if(lengthOfTraversedAST != lengthOfPattern || traversedASTString != traversedQueryString ) {
            return false;
        }
        return true;
    } else {
        return traversedASTString.find(traversedQueryString) != string::npos;
    }*/
}

string AST::convertTNodeListValueToString(vector<TNode*> nodes) {
    string result;
    for(int i = 0 ; i < nodes.size(); i++) {
        TNode * currentNode = nodes[i];
        if(currentNode->getTType() == PLUSN) {
            result += "+";
        } else if(currentNode->getTType() == MINUSN) {
            result += "-";
        } else if(currentNode->getTType() == VARN) {
            VARINDEX v = std::atoi(getValue(currentNode).c_str());
            result += "|" + PKB::getPKB()->getVarTable()->getVarName(v) + "|";
        } else if (currentNode->getTType() == CONSTN) {
            result += "|" + getValue(currentNode) + "|";
        } else {
            // error with parsing the expression tree / invalid expression tree
        }
    }
    return result;
}

vector<TNode*> AST::getDFS(TNode* node) {
    vector<TNode*> stack;
    stack.push_back(node);
    vector<TNode*> returnVector;
    vector<TNode*> discoveredNode;
    TNode* currentNode = node;

    while(stack.size() > 0) {
        //getting the last item
        currentNode = stack[stack.size()-1];
        stack.pop_back();
        if(!ifNodeVisited(discoveredNode, currentNode)) {
            discoveredNode.push_back(currentNode);
            returnVector.push_back(currentNode);
        }

        vector<TNode*> childrenList = currentNode->getChildren();
        if(childrenList.size() > 0) {
            TNode *leftChild = childrenList[0];
            TNode *rightChild = childrenList[1];

            //keep pushing right child first so when popping we always retrieve the left child
            if(rightChild != NULL)
                stack.push_back(rightChild);
            if(leftChild != NULL)
                stack.push_back(leftChild);
        }
    }
    return returnVector;
}

bool AST::ifNodeVisited(vector<TNode*> nodeList, TNode* node) {
    for(int i = 0; i < nodeList.size(); i++) {
        if(nodeList[i] == node) {
            return true;
        }
    }
    return false;
}