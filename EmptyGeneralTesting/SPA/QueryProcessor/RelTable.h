#pragma once
#include "Grammar.h"
#include <vector>
#include <map>

//Define argument rules for relationship
const struct Relationship {
	std::vector<ArgumentType> arg1Type,arg2Type;
	std::vector<SynonymType> arg1SynonymType, arg2SynonymType;
		
	Relationship() : arg1Type(), arg2Type(), arg1SynonymType(), arg2SynonymType() {}

	Relationship(const std::vector<ArgumentType> a1, const std::vector<ArgumentType> a2, 
					const std::vector<SynonymType> s1, const std::vector<SynonymType> s2) :
		arg1Type(a1) ,
		arg2Type(a2) ,
		arg1SynonymType(s1) ,
		arg2SynonymType(s2) {}
};

const enum ArgumentNumber {
	ARG1,
	ARG2
};

class RelTable
{
private:
	//modifies argument rules
	static const ArgumentType a1_modifies[];
	static const ArgumentType a2_modifies[];
	static const SynonymType s1_modifies[];
	static const SynonymType s2_modifies[];

	//parent argument rules
	static const ArgumentType a1_parent[]; 
	static const ArgumentType a2_parent[];
	static const SynonymType s1_parent[];
	static const SynonymType s2_parent[];

	//follows argument rules
	static const ArgumentType a1_follows[];
	static const ArgumentType a2_follows[];
	static const SynonymType s1_follows[];
	static const SynonymType s2_follows[];

	//calls argument rules
	static const ArgumentType a1_calls[];
	static const ArgumentType a2_calls[];
	static const SynonymType s1_calls[];
	static const SynonymType s2_calls[];

	//next argument rules
	static const ArgumentType a1_next[];
	static const ArgumentType a2_next[];
	static const SynonymType s1_next[];
	static const SynonymType s2_next[];

	//affects argument rules
	static const ArgumentType a1_affects[];
	static const ArgumentType a2_affects[];
	static const SynonymType s1_affects[];
	static const SynonymType s2_affects[];

	//individual relationship
	static const Relationship modifiesTable;	//Modifies and Uses share same table
	static const Relationship parentTable;
	static const Relationship followsTable;
	static const Relationship callsTable;
	static const Relationship nextTable;
	static const Relationship affectsTable;

	//table for relationships
	static const std::map<RelationshipType,Relationship> relationshipTable;
	
	static std::map<RelationshipType,Relationship> CreateTable();
	static Relationship CreateRelationship(RelationshipType); 
	 
public:
	RelTable(void);
	~RelTable(void);

	static std::vector<ArgumentType> GetRelArgType(RelationshipType, ArgumentNumber);
	static std::vector<SynonymType> GetRelArgSynonymType(RelationshipType, ArgumentNumber);
	static int GetTableSize();
};