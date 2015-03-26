#include "RelTable.h"
#include <iostream>

/*
Modifies(Synonym | Integer , Synonym | _ | "Ident")
Uses	(Synonym | Integer , Synonym | _ | "Ident")
Argument 1 synonym - stmt, assign, while, prog_line
Argument 2 synonym - variable

Parent (Synonym | _ | Integer , Synonym | _ | Integer)
Parent*(Synonym | _ | Integer , Synonym | _ | Integer)
Argument 1 synonym - stmt, while, prog_line
Argument 2 synonym - stmt, assign, while, prog_line

Follows (Synonym | _ | Integer , Synonym | _ | Integer)
Follows*(Synonym | _ | Integer , Synonym | _ | Integer)
Argument 1 synonym - stmt, assign, while, prog_line
Argument 2 synonym - stmt, assign, while, prog_line
*/

const ArgumentType RelTable::a1_modifies[] = {SYNONYM, INTEGER};
const ArgumentType RelTable::a2_modifies[] = {SYNONYM, UNDERSCORE, IDENT};
const SynonymType RelTable::s1_modifies[] = {STMT, ASSIGN, WHILE, PROG_LINE};
const SynonymType RelTable::s2_modifies[] = {VARIABLE};

const ArgumentType RelTable::a1_parent[] = {SYNONYM, UNDERSCORE, INTEGER};
const ArgumentType RelTable::a2_parent[] = {SYNONYM, UNDERSCORE, INTEGER};
const SynonymType RelTable::s1_parent[] = {STMT, WHILE, PROG_LINE};
const SynonymType RelTable::s2_parent[] = {STMT, ASSIGN, WHILE, PROG_LINE};

const ArgumentType RelTable::a1_follows[] = {SYNONYM, UNDERSCORE, INTEGER};
const ArgumentType RelTable::a2_follows[] = {SYNONYM, UNDERSCORE, INTEGER};
const SynonymType RelTable::s1_follows[] = {STMT, ASSIGN, WHILE, PROG_LINE};
const SynonymType RelTable::s2_follows[] = {STMT, ASSIGN, WHILE, PROG_LINE};

const Relationship RelTable::modifiesTable = RelTable::CreateRelationship(MODIFIES);
const Relationship RelTable::parentTable = RelTable::CreateRelationship(PARENT);
const Relationship RelTable::followsTable = RelTable::CreateRelationship(FOLLOWS);

const std::map<RelationshipType,Relationship> RelTable::relationshipTable = RelTable::CreateTable();

RelTable::RelTable(void)
{}

Relationship RelTable::CreateRelationship(RelationshipType relType) 
{
	std::vector<ArgumentType> a1,a2;
	std::vector<SynonymType> s1,s2;

	switch(relType)
	{
	case MODIFIES: 
		a1.assign(a1_modifies, a1_modifies + sizeof(a1_modifies) / sizeof(a1_modifies[0]));
		a2.assign(a2_modifies, a2_modifies + sizeof(a2_modifies) / sizeof(a2_modifies[0]));
		s1.assign(s1_modifies, s1_modifies + sizeof(s1_modifies) / sizeof(s1_modifies[0]));
		s2.assign(s2_modifies, s2_modifies + sizeof(s2_modifies) / sizeof(s2_modifies[0]));
		break;

	case PARENT: 
		a1.assign(a1_parent, a1_parent + sizeof(a1_parent) / sizeof(a1_parent[0]));
		a2.assign(a2_parent, a2_parent + sizeof(a2_parent) / sizeof(a2_parent[0]));
		s1.assign(s1_parent, s1_parent + sizeof(s1_parent) / sizeof(s1_parent[0]));
		s2.assign(s2_parent, s2_parent + sizeof(s2_parent) / sizeof(s2_parent[0]));
		break;

	case FOLLOWS: 
		a1.assign(a1_follows, a1_follows + sizeof(a1_follows) / sizeof(a1_follows[0]));
		a2.assign(a2_follows, a2_follows + sizeof(a2_follows) / sizeof(a2_follows[0]));
		s1.assign(s1_follows, s1_follows + sizeof(s1_follows) / sizeof(s1_follows[0]));
		s2.assign(s2_follows, s2_follows + sizeof(s2_follows) / sizeof(s2_follows[0]));
		break;

	default:
		break;
	}

	return Relationship(a1, a2, s1, s2);
}


std::map<RelationshipType,Relationship> RelTable::CreateTable() 
{
	std::map<RelationshipType,Relationship> table;
	table[MODIFIES] = modifiesTable;
	//table[USES] = modifiesTable;
	table[PARENT] = parentTable;
	//table[PARENTT] = parentTable;
	table[FOLLOWS] = followsTable;
	//table[FOLLOWST] = followsTable;
	return table;
}

int RelTable::GetTableSize()
{
	return relationshipTable.size();
}

std::vector<ArgumentType> RelTable::GetRelArgType(RelationshipType relType, ArgumentNumber argNum)
{
	if(relType >= INVALID_RELATIONSHIP_TYPE)
		throw std::invalid_argument("Invalid argument : RelationshipType");

	//USES shares MODIFIES table, PARENT* shares PARENT table, FOLLOWS* shares FOLLOWS table
	if(relType == USES)				relType = MODIFIES;
	else if(relType == PARENTT)		relType = PARENT;
	else if(relType == FOLLOWST)	relType = FOLLOWS;

	switch(argNum)
	{
		case ARG1:	return relationshipTable.at(relType).arg1Type;
		case ARG2:	return relationshipTable.at(relType).arg2Type;
		default: break;
	}

	throw std::invalid_argument("Invalid argument : ArgumentNumber");
}

std::vector<SynonymType> RelTable::GetRelArgSynonymType(RelationshipType relType, ArgumentNumber argNum)
{
	if(relType >= INVALID_RELATIONSHIP_TYPE)
		throw std::invalid_argument("Invalid argument : RelationshipType");

	//USES shares MODIFIES table, PARENT* shares PARENT table, FOLLOWS* shares FOLLOWS table
	if(relType == USES)				relType = MODIFIES;
	else if(relType == PARENTT)		relType = PARENT;
	else if(relType == FOLLOWST)	relType = FOLLOWS;

	switch(argNum)
	{
		case ARG1:	return relationshipTable.at(relType).arg1SynonymType;
		case ARG2:	return relationshipTable.at(relType).arg2SynonymType;
		default: break;
	}

	throw std::invalid_argument("invalid argument number specified");
}