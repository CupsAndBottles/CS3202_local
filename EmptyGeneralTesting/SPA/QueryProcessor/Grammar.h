#pragma once
#include <string>

enum RelationshipType {
	MODIFIES,
	USES,
	PARENT,
	PARENTT,
	FOLLOWS,
	FOLLOWST,
	CALLS,
	CALLST,
	NEXT,
	NEXTT,
	AFFECTS,
	AFFECTST,
	INVALID_RELATIONSHIP_TYPE
};

enum ArgumentType {
	SYNONYM,
	UNDERSCORE,
	IDENT,
	EXPRESSION,
	INTEGER,
	INVALID_ARGUMENT_TYPE
};

enum SynonymType {
	ASSIGN,
	STMT,
	WHILE,
	IF,
	CALL,		//for parser, not query
	VARIABLE,
	CONSTANT,
	PROCEDURE,
	PROG_LINE,
	BOOLEAN,
	INVALID_SYNONYM_TYPE
};

enum AttrNameType {
	STMTNUM,
	VALUE,
	PROCNAME,
	VARNAME,
	INVALID_ATTRNAME_TYPE
};

enum ClauseType {
	DECLARATION,
	SELECT,
	SUCHTHAT,
	WITH,
	PATTERN,
	INVALID_CLAUSE_TYPE
};

struct Synonym {
	std::string value;
	SynonymType type;

	Synonym() : value(""), type(INVALID_SYNONYM_TYPE) {};
	Synonym(std::string s, SynonymType t) : value(s) , type(t) {}
};

struct Argument {
	std::string value;
	ArgumentType type;
	Synonym syn;

	Argument() : value(""), type(INVALID_ARGUMENT_TYPE), syn() {}
	Argument(std::string v, ArgumentType t) : value(v) , type(t) ,syn() {}
	Argument(std::string v, ArgumentType t, Synonym s) : value(v) , type(t) ,syn(s) {}
	Argument(Synonym s) : syn(s) {}
	Argument(std::string v) : value(v) {}

	void Clear() {
		value = "";
		type = INVALID_ARGUMENT_TYPE;
		syn.value = "";
		syn.type = INVALID_SYNONYM_TYPE;
	}
};

struct Declaration {
	Synonym synonym;

	Declaration(Synonym s) : synonym(s) {}
};

struct SelectClause {
	Synonym synonym;

	SelectClause(Synonym s) : synonym(s) {}
};

struct PatternClause {
	Synonym synonym;
	Argument arg1, arg2;	//since arg2 and arg3 of if are _, just ignore, only need 1

	PatternClause(Synonym s, Argument a1, Argument a2) : synonym(s) , arg1(a1) , arg2(a2) {}
};

struct SuchThatClause {
	RelationshipType relationship;
	Argument arg1, arg2;

	SuchThatClause(RelationshipType r, Argument a1, Argument a2) : relationship(r) , arg1(a1), arg2(a2) {}
};

struct WithClause {
	Argument arg1,arg2;

	WithClause(Argument a1, Argument a2) : arg1(a1), arg2(a2) {}
};