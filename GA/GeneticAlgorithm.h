#include <vector>
#include <map>
#include <set>
#include <stdlib.h>
#include <time.h> 
#include <iostream>
#include "Chromosome.h"
#pragma once

class GeneticAlgorithm
{
	std::map<int, std::vector<Chromosome*> > chromosomes;

	std::set<std::string> topSolutions;

	void addChromosome(Chromosome *c) {
		if (c == nullptr)
			return;

		auto it = chromosomes.find(c->Fitness());
		if (it == chromosomes.end()) {
			chromosomes[c->Fitness()] = std::vector<Chromosome*>();
			it = chromosomes.find(c->Fitness());
		}
		it->second.push_back(c);
	}

	Chromosome *rouletteSelect(std::vector<Chromosome*> &selected) const {
		int maxRange = rand() % chromosomes.size() + 1;
		int bucketId = rand() % maxRange;
		auto it = chromosomes.begin();
		std::advance(it, bucketId);
		Chromosome* c = it->second[rand() % it->second.size()];
		if (std::find(selected.begin(), selected.end(), c) != selected.end())
			return rouletteSelect(selected);
		selected.push_back(c);
		return c;
	}

	Chromosome *crossover() {
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		if (r > settings.crossoverRate)
			return nullptr;

		std::vector<Chromosome*> selected;
		Chromosome *c1 = rouletteSelect(selected);
		Chromosome *c2 = rouletteSelect(selected);
		return new Chromosome(*c1, *c2);
	}

public:
	GeneticAlgorithm() {
		srand(static_cast <unsigned> (time(0)));

		for (int i = 0; i < settings.initialPopulation; ++i) {
			addChromosome(new Chromosome());
		}
	}

	~GeneticAlgorithm() {
		for (auto it : chromosomes) {
			for (auto ptr : it.second)
				delete ptr;
		}
	}

	void execute() {		
		int generationsLeft = settings.maxGenerations;
		while (chromosomes.begin()->first != 0 || generationsLeft > 0) {
			addChromosome(crossover());
			generationsLeft--;
		}

		auto topBucket = chromosomes.begin()->second;
		topSolutions.clear();
		for (auto ptr : topBucket) {
			topSolutions.insert(ptr->DecodedString());
		}
	}

	friend std::ostream& operator<<(std::ostream &os, const GeneticAlgorithm &ga) {
		os << "Goal: " << settings.goal << std::endl;
		for (auto it : ga.chromosomes) {
			os << it.first << ": ";

			for (auto it2 = it.second.begin(); it2 != it.second.end(); ++it2) {
				os << **it2;
				if (it2 + 1 < it.second.end())
					os << ", ";
			}
			os << std::endl;
		}

		

		return os;
	}

	std::set<std::string> const &getTopSolutions() const {
		return topSolutions;
	}
};

