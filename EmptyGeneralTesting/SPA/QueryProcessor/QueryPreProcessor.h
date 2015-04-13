 #pragma once
#include <string>
#include <vector>
#include "QueryData.h"

class QueryPreProcessor
{
	friend class QueryPreProcessorTest;

protected:
	//for debugging purpose
	enum FUNCTION {
		TOKENIZER,
		VALIDATEQUERY
	};

	static const std::string de[];
	static const std::string rel[];
		
	bool Tokenize(std::string, std::vector<std::string>&);

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

	bool IsValidAttrRef(std::string rhs , Synonym& syn , AttrName& attrN);

	void DebugMessage(std::string msg , FUNCTION);

public:
	QueryPreProcessor(void);

	bool ValidateQuery(std::string, QueryData&);
	static void Tokenize(std::string, std::vector<std::string>&, std::string);
};

