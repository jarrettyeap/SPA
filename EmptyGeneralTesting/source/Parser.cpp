#include "Parser.h"
#include <stack>

class SyntaxErrorException : public exception {
};

class InvalidNameException : public exception {
};

class InvalidProcedureException : public SyntaxErrorException {
};

class InvalidExpressionException : public SyntaxErrorException {
};

class InvalidWhileStmtException: public SyntaxErrorException {
};

/*
	Remove comment in a line and return a list of tokens from that line
*/
vector<ParsingToken*> Parser::tokenizeLine(string line) {
    vector<ParsingToken*> tokenList;

    string currStr = "";
    for (size_t i=0; i<line.size(); i++) {
        char nextChar = line.at(i);
        if (nextChar == ' ' || nextChar == '\t') {
            if (currStr.size() > 0) {
                tokenList.push_back(Parser::convertStringToToken(currStr));
                currStr = "";
            }
        } else if (nextChar == '=' || nextChar == '+' || nextChar == '-' || nextChar == ';' || nextChar == '{' || nextChar == '}') {
            if (currStr.size() > 0) {
                tokenList.push_back(Parser::convertStringToToken(currStr));
            }
			currStr = string(1, nextChar);
            tokenList.push_back(Parser::convertStringToToken(currStr));
            currStr = "";
        } else if (nextChar == '\\') {
            if (i == line.size() - 1) {
                throw SyntaxErrorException();
            } else if (line.at(i+1) == '\\') {  // Ignore comments
                return tokenList;
            } else {
                throw SyntaxErrorException();
            }
        } else if ((nextChar >= 'a' && nextChar <= 'z') || (nextChar >= 'A' && nextChar <= 'Z') || (nextChar >= '0' && nextChar <= '9')) {
            currStr.push_back(nextChar);
        }
    }

    return tokenList;
}

ParsingToken* Parser::convertStringToToken(string aString) {
	ParsingToken* token = new ParsingToken();
	if (aString.compare("procedure") == 0) {
		token->setTokenType(TokenType::PROCEDURE_TOKEN);
	} else if (aString.compare("while") == 0) {
		token->setTokenType(TokenType::WHILE_TOKEN);
	} else if (aString.compare("if") == 0) {
		token->setTokenType(TokenType::IF_TOKEN);
	} else if (aString.compare("else") == 0) {
		token->setTokenType(TokenType::ELSE_TOKEN);
	} else if (aString.compare("=") == 0) {
		token->setTokenType(TokenType::ASSIGNMENT_TOKEN);
	} else if (aString.compare("+") == 0) {
		token->setTokenType(TokenType::PLUS);
	} else if (aString.compare("-") == 0) {
		token->setTokenType(TokenType::MINUS);
	} else if (aString.compare(";") == 0) {
		token->setTokenType(TokenType::SEMICOLON);
	} else if (aString.compare("{") == 0) {
		token->setTokenType(TokenType::OPEN_CURLY_BRACKET);
	} else if (aString.compare("}") == 0) {
		token->setTokenType(TokenType::CLOSE_CURLY_BRACKET);
	} else if (Parser::isNumeric(aString)) {
		token->setTokenType(TokenType::CONSTANT);
		token->setIntValue(atoi(aString.c_str()));
	} else if (Parser::isValidName(aString)) {
		token->setTokenType(TokenType::NAME);
		token->setStringValue(aString);
	} else {
		throw InvalidNameException();
	}

	return token;
}

bool Parser::isNumeric(string aString) {
	for (int i=0; i<aString.size(); i++) {
		if (aString.at(i) < '0' || aString.at(i) > '9')
			return false;
	}
	return true;
}

bool Parser::isValidName(string aString) {
	if (aString.size() == 0)
		return false;
	else if (aString.compare("while") == 0 || aString.compare("if") == 0 
		|| aString.compare("else") == 0 || aString.compare("procedure") == 0) {
		// name cannot be one of reserved words
		return false;
	} else {
		if (aString.at(0) < 'A' || (aString.at(0) > 'Z' && aString.at(0) < 'a') || (aString.at(0) > 'z')) { 
			// first character is not a letter 
			return false;
		}
		else {
			for (int i=1; i<aString.size(); i++) {
				if ((aString.at(i) >= '0' && aString.at(i) <= '9') || (aString.at(i) >= 'a' && aString.at(i) <= 'z') 
					|| (aString.at(i) >= 'A' && aString.at(i) <= 'Z')) {
					// if ith character is a digit or a letter, then continue
				} else {
					return false;
				}
			}
			return true;
		}
	}
}

// build the AST for a procedure
AST* Parser::buildProcedureAST(vector<ParsingToken> tokenList) {
	AST *ast = new AST();

	if (tokenList.size() < 8) {  // if the number of tokens in the procedure is less than 8, then the procedure is invalid
		throw InvalidProcedureException();
	}

	// if the procedure does not start with the format procedure proc_name {, then return invalid
	if (tokenList.at(0).getTokenType() != TokenType::PROCEDURE_TOKEN || tokenList.at(1).getTokenType() != TokenType::NAME
		|| tokenList.at(2).getTokenType() != TokenType::OPEN_CURLY_BRACKET) {
		throw InvalidProcedureException();
	}

	// set up the AST 
	TNode *procedureNode = new TNode(TType::PROCEDUREN, tokenList.at(1).getStringValue());
	ast->setRoot(procedureNode);
	TNode *stmtLstNode = new TNode(TType::STMTLSTN, "");
	ast->addChildTNode(procedureNode, stmtLstNode);
	TNode *prevNode = stmtLstNode;
	TNodeRelation expectedRelation = TNodeRelation::CHILD;

	int i=3;
	int stmtLine = 1;
	while (i < tokenList.size()) {
		if (tokenList.at(i).getTokenType() == TokenType::NAME) { // assignment statement  
			// if the fisrt token of the statement is a name, then the statement is an assignment
			if (tokenList.at(i+1).getTokenType() != TokenType::ASSIGNMENT_TOKEN)
				throw SyntaxErrorException();
			
			// parse the expression on the right hand side of the assignment
			int j = i+2;
			vector<ParsingToken> exprTokenList;
			while (tokenList.at(j).getTokenType() != TokenType::SEMICOLON) {
				exprTokenList.push_back(tokenList.at(j));
				j++;
			}
			i = j+1; // move i to after the SEMICOLON position
			TNode *exprNode = Parser::buildExprAST(exprTokenList);
			TNode *varNode = new TNode(TType::VARN, tokenList.at(i).getStringValue());
			TNode *assignNode = new TNode(TType::ASSIGNN, "");
			assignNode->setStmtLine(stmtLine);
			stmtLine++;
			Parser::linkTNodes(assignNode, varNode, exprNode);
			
			// link assignNode to previous nodes on the AST
			Parser::linkTNodeToPrevNodes(assignNode, prevNode, expectedRelation);
			
			// prevNode now points to assignNode to parse the next statement
			prevNode = assignNode;
			expectedRelation = TNodeRelation::RIGHT_SIBLING;
		} else if (tokenList.at(i).getTokenType() == TokenType::WHILE_TOKEN) { // while statement
			if (tokenList.at(i+1).getTokenType() != TokenType::NAME || tokenList.at(i+2).getTokenType() != TokenType::OPEN_CURLY_BRACKET) {
				// if the statement does not follow the format 'while var_name {' then thow exception
				throw InvalidWhileStmtException();
			} else {
				i = i+3; // move i to after the open curly bracket
				TNode *whileNode = new TNode(TType::WHILEN, "");
				whileNode->setStmtLine(stmtLine);
				stmtLine++;
				
				// link whileNode to varNode
				TNode *varNode = new TNode(TType::VARN, tokenList.at(i).getStringValue());
				whileNode->addChild(varNode);
				varNode->setParentNode(whileNode);

				// link whileNode to previous nodes on the AST
				linkTNodeToPrevNodes(whileNode, prevNode, expectedRelation);

				// prevNode now points to whileNode to parse the next statement
				prevNode = whileNode;
				expectedRelation = TNodeRelation::CHILD;
			}
		} else if (tokenList.at(i).getTokenType() == TokenType::CLOSE_CURLY_BRACKET) { // end of a while loop
			// prevNode now points to its parent
			prevNode = prevNode->getParentNode();
			expectedRelation = TNodeRelation::RIGHT_SIBLING;
		}
	}

	return ast;
}

// build the AST for an expression, e.g. x + y + z
TNode* Parser::buildExprAST(vector<ParsingToken> exprTokenList) {
	if (exprTokenList.size() == 0)
		throw InvalidExpressionException();

	stack<TNode*> operandStack;
	stack<OperatorType> operatorStack;

	for (int i=0; i<exprTokenList.size(); i++) {
		ParsingToken currToken = exprTokenList.at(i);
		if (currToken.getTokenType() == TokenType::PLUS) // if token is + then put it to the operator stack
			operatorStack.push(OperatorType::PLUS_OP);
		else if (currToken.getTokenType() == TokenType::NAME) {
			if (!operatorStack.empty()) { 
				OperatorType opType = operatorStack.top();
				operatorStack.pop();

				if (operandStack.empty())
					throw InvalidExpressionException();
				TNode* fNode = operandStack.top();
				operandStack.pop();

				if (opType == OperatorType::PLUS_OP) {
					TNode *plusNode = new TNode(TType::PLUSN, "");
					TNode *sNode = new TNode(TType::VARN, "");
					Parser::linkTNodes(plusNode, fNode, sNode);
					operandStack.push(plusNode);
				}
			} else {
				TNode *varNode = new TNode(TType::VARN, currToken.getStringValue());
				operandStack.push(varNode);
			}
		}
	}

	if (!operatorStack.empty()) // if there are more operators than necessary, then the expression is invalid
		throw InvalidExpressionException();

	if (operandStack.empty()) // after finishing parsing, there should be only one operand in the operand stack
		throw InvalidExpressionException();

	TNode *exprRootNode = operandStack.top();
	operandStack.pop();

	if (!operandStack.empty())
		throw InvalidExpressionException();

	return exprRootNode;
}

void Parser::linkTNodes(TNode *parentNode, TNode *leftNode, TNode *rightNode)
{
	parentNode->addChild(leftNode);
	parentNode->addChild(rightNode);
	leftNode->setParentNode(parentNode);
	rightNode->setParentNode(parentNode);
	leftNode->setRightSibling(rightNode);
	rightNode->setLeftSibling(leftNode);
}

void Parser::linkTNodeToPrevNodes(TNode *currNode, TNode *prevNode, TNodeRelation expectedRelation)
{
	if (expectedRelation == TNodeRelation::CHILD) {
		prevNode->addChild(currNode);
		currNode->setParentNode(prevNode);
	} else if (expectedRelation == TNodeRelation::RIGHT_SIBLING) {
		prevNode->setRightSibling(currNode);
		currNode->setLeftSibling(prevNode);
		TNode *parentNode = prevNode->getParentNode();
		parentNode->addChild(currNode);
		currNode->setParentNode(parentNode);
	}
}