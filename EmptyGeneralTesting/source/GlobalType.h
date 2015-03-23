#pragma once
#include<string>

using namespace std;
typedef string PROCNAME, VARNAME;
typedef int PROCINDEX, PROGLINE, STMTLINE, VARINDEX, CONSTINDEX;
enum TType {
	PROCEDURE_NODE,
	STMTLST_NODE,
	ASSIGN_NODE,
	VAR_NODE,
	CONST_NODE,
	PLUS_NODE,
	NONE
};

enum QNodeType {
	QUERY,
	SUCHTHATLIST,
	WITHLIST,
	PATTERNLIST,
	RESULTLIST,
	PATTERN,
	RELATION,
	EXPRESSION,
	BOOLEAN,
	ASSIGN,
	WHILE,
	VAR,
	CONST,
	PROC,
	IF
};