#include "QueryProcessor.h"
#include "QueryData.h"
#include "QueryEvaluator.h"
#include "QueryPreProcessor.h"

#include <string>
#include <list>
#include <iostream>

QueryProcessor::QueryProcessor(void) {}

std::list<std::string> QueryProcessor::ProcessQuery(std::string query) {
	QueryData queryData;
	std::list<std::string> result;
	QueryEvaluator QE;
	QueryPreProcessor QV;

	queryData.ClearData();
	//std::cout << "\nHERE\n";
	if(QV.ValidateQuery(query, queryData)) {
		//std::cout << "\nInside validate Query.\n";
		QE.EvaluateQuery(queryData, result);
		//std::cout << "\nafter evaluate Query.\n";
		return result;
	}
	
	//std::cout << "\nLOL Invalid Query.\n";

	return result;
}