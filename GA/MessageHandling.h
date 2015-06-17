#include <string> 
#include "ppapi/cpp/var.h"
#include "ppapi/cpp/var_dictionary.h"
#include "ppapi/cpp/var_array.h"
#include "GeneticAlgorithm.h"
#pragma once

class Message
{
protected:
	pp::VarDictionary dictionary;

public:	
	Message(const int &action) {
		dictionary.Set("action", action);
		dictionary.Set("status", "Ok");
	}

	Message(const std::string &status) {
		dictionary.Set("action", 0);
		dictionary.Set("status", status);		
	}

	Message(const std::string &status, const int &action) {
		dictionary.Set("action", action);
		dictionary.Set("status", status);
	}

	const pp::VarDictionary &getResult() {
		return dictionary;
	}
};

class ExecuteMessage : public Message {
	GeneticAlgorithm ga;

public:
	ExecuteMessage(const pp::VarDictionary &dictionaryIn) : Message(1) {
		if (dictionaryIn.HasKey("initialPopulation"))
			settings.initialPopulation = dictionaryIn.Get("initialPopulation").AsInt();
		
		if (dictionaryIn.HasKey("mutationRate"))
			settings.mutationRate = dictionaryIn.Get("mutationRate").AsDouble();

		if (dictionaryIn.HasKey("goal"))
			settings.goal = dictionaryIn.Get("goal").AsInt();

		if (dictionaryIn.HasKey("maxGenerations"))
			settings.maxGenerations = dictionaryIn.Get("maxGenerations").AsInt();

		if (dictionaryIn.HasKey("crossoverRate"))
			settings.crossoverRate = dictionaryIn.Get("crossoverRate").AsDouble();
				
		ga.execute();

		dictionary.Set("initialPopulation", settings.initialPopulation);
		dictionary.Set("mutationRate", settings.mutationRate);
		dictionary.Set("goal", settings.goal);
		dictionary.Set("maxGenerations", settings.maxGenerations);
		dictionary.Set("crossoverRate", settings.crossoverRate);

		pp::VarArray results;
		std::set<std::string> solutions = ga.getTopSolutions();
		for (size_t i = 0; i < solutions.size(); i++) {
			results.Set(i, *std::next(solutions.begin(), i));
		}
		dictionary.Set("results", results);
	}
};

Message messageFactory(const pp::Var& var) {
	if (!var.is_dictionary()) {
		return Message("Var must be a dictionary");
	}

	pp::VarDictionary dictionary(var);

	if (!dictionary.HasKey("action"))
		return Message("Missing message action");

	if (dictionary.Get("action") == 1)
		return ExecuteMessage(dictionary);

	return Message("Unknown action: " + dictionary.Get("action").AsString());
}