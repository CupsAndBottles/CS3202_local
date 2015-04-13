 #pragma once
#include <string>
#include <vector>
#include "QueryData.h"

typedef unsigned int UINT;
typedef unsigned long long ULL;

//for general output
#define DEBUG_MSG 

//#define DEBUG_MSG_TOKENIZER
//#define DEBUG_MSG_VALIDATEQUERY
//#define DEBUG_MSG_ISVALIDATTRREF
//#define DEBUG_MSG_VALIDATERELATIONSHIP
#define DEBUG_MSG_VALIDATEPATTERN
//#define DEBUG_MSG_VALIDATEWITH


class QueryPreProcessor
{
	friend class QueryPreProcessorTest;

protected:
	//for debugging purpose
	enum FUNCTION {
		TOKENIZER,
		ISVALIDATTRREF,

		VALIDATEQUERY,
		VALIDATEDECLARATION,
		VALIDATESELECT,		
		VALIDATERELATIONSHIP,
		VALIDATEPATTERN,
		VALIDATEWITH
	};

	static const std::string de[];
	static const std::string rel[];
		
	bool Tokenize(std::string, std::vector<std::string>&);
	void TokenizeExpression(std::string exp, std::vector<std::string> &token, std::string &delim);

	//Validate Clause
	bool ValidateDeclaration(Synonym&, std::string);
	bool ValidateSelect(Synonym&);
	bool ValidateRelationship(std::string, RelationshipType&, Argument&, Argument&);
	bool ValidatePattern(Synonym synonym, Argument& arg1, Argument& arg2);
	bool ValidatePattern(Synonym synonym, Argument& arg1, Argument& arg2, Argument& arg3);
	bool ValidateWith(Argument& arg1, Argument& arg2, std::string lhs, std::string rhs);

	//Validate Argument
	bool IsInteger(const std::string&);
	bool IsExpression(std::string);
	bool IsIdent(std::string);
	bool IsName(std::string);
	bool IsOperator(std::string s);

	//string matching
	bool IsDeclaration(std::string);
	bool IsRelationship(std::string);
	bool IsSelect(std::string);
	bool IsSemiColon(std::string);
	bool IsSuchThat(std::string);
	bool IsPattern(std::string);
	bool IsUnderscore(std::string);
	bool IsWith(std::string);
	bool IsOpenBracket(std::string str);
	bool IsCloseBracket(std::string str);
	bool IsAnd(std::string);
	bool IsBoolean(std::string str);
	bool IsDot(std::string str);
	bool IsEqual(std::string str);

	//Convert string to enum
	bool GetEnumSynonymType(std::string, SynonymType&); 
	bool GetEnumRelationshipType(std::string, RelationshipType&);
	AttrNameType GetEnumAttrNameType(std::string type);

	bool IsValidAttrRef(std::string rhs , Synonym& syn , AttrNameType& attrN);
	bool IsValidExpression(std::vector<std::string>& exp , int prevOpenBracket = -1, int prevCloseBracket = -1);

	void DebugMessage(std::string msg);
	void DebugMessage(std::string msg , FUNCTION);

public:
	QueryPreProcessor(void);

	bool ValidateQuery(std::string, QueryData&);
	static void Tokenize(std::string, std::vector<std::string>&, std::string);
};

