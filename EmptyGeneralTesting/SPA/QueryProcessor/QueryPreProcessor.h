 #pragma once
#include <string>
#include <vector>
#include "QueryData.h"

class QueryPreProcessor
{
	friend class QueryPreProcessorTest;

protected:
	static const std::string de[];
	static const std::string rel[];
		
	bool Tokenize(std::string, std::vector<std::string>&);

	//Validate Clause
	bool ValidateDeclaration(Synonym&, std::string);
	bool ValidateSelect(Synonym&);
	bool ValidateRelationship(std::string, RelationshipType&, Argument&, Argument&);
	bool ValidatePattern(Synonym, Argument&, Argument&);

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

	//Convert string to enum
	bool GetEnumSynonymType(std::string, SynonymType&); 
	bool GetEnumRelationshipType(std::string, RelationshipType&);

public:
	QueryPreProcessor(void);

	bool ValidateQuery(std::string, QueryData&);
	static void Tokenize(std::string, std::vector<std::string>&, std::string);
};

