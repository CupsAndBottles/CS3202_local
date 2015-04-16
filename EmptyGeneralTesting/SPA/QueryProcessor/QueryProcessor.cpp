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

	if(QV.ValidateQuery(query, queryData)) {
		//std::cout << "\nInside validate Query.\n";
		if(QE.EvaluateQuery(queryData, result)) 
			std::cout << "After evaluation, valid.\n";
		else 
			std::cout << "After evaluation, invalid.\n";
		//std::cout << "\nafter evaluate Query.\n";

		cout<< "\nFinal result list: ";
		for(list<string>::iterator it = result.begin(); it != result.end(); ++it)
			cout << *it << " ";
		cout<< "\n";

		return result;
	}

	return result;
}