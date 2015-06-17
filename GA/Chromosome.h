#include <iostream>
#include <string> 
#include <vector>
#include <stdlib.h>
#include <cmath>
#include "Gene.h"
#include "GASettings.h"
#pragma once

#define MAX_GENES 9

class ChromosomeDecoder
{
public:
	static ChromosomeDecoder& getInstance()
	{
		static ChromosomeDecoder instance;
		return instance;
	}
private:
	std::string numbers = "0123456789";

	std::string operators = "+-*/";

private:
	ChromosomeDecoder() {}

	ChromosomeDecoder(ChromosomeDecoder const&) = delete;

	void operator=(ChromosomeDecoder const&) = delete;

	bool isNumber(char c) const { return operators.find(c) == std::string::npos; }

	std::string processString(const std::string &str) const {
		std::string r = "";
		bool nextNumber = true;

		for (auto c : str) {
			bool n = isNumber(c);
			if (nextNumber == n) {
				r.push_back(c);
				nextNumber = !nextNumber;
			}
		}

		if (nextNumber && r.size() > 0)
			r.pop_back();

		if (r == "")
			r = "0";

		return r;
	}

	bool processOperation(const std::string &str, int &a, int &b, std::size_t &left, std::size_t &right, char op) const {
		using std::string;

		std::size_t found = str.find(op);
		if (found != string::npos && found != 0) {
			left = found - 1;
			while (left > 0 && isNumber(str[left-1])) left--;
			string lhs = str.substr(left, found - left);

			right = found + 1;
			while (right < str.size()-1 && isNumber(str[right+1])) right++;
			string rhs = str.substr(found+1, right - found);

			a = std::stoi(lhs);
			b = std::stoi(rhs);

			return true;
		}
		return false;
	}

	int evaluate(std::string str) const {
		using std::string;

		int a, b, r;
		std::size_t left, right;

		while (processOperation(str, a, b, left, right, '*')) {
			r = a*b;
			str.replace(left, right-left+1, std::to_string(r));
		}

		while (processOperation(str, a, b, left, right, '/')) {
			if (b != 0)
				r = a/b;
			else r = 0;
			str.replace(left, right - left + 1, std::to_string(r));
		}

		while (processOperation(str, a, b, left, right, '+')) {
			r = a+b;
			str.replace(left, right - left + 1, std::to_string(r));
		}

		while (processOperation(str, a, b, left, right, '-')) {
			r = a-b;
			str.replace(left, right - left + 1, std::to_string(r));
		}

		return std::stoi(str);
	}

public:
	void decode(std::vector<Gene> genes, std::string &decodedString, int &value) {
		using std::string;
		
		string unprocessedString = "";
		
		for (auto &g : genes) {
			unprocessedString += g.DecodedString();
		}

		decodedString = processString(unprocessedString);

		value = evaluate(decodedString);
	}
};



class Chromosome
{
private:
	std::vector<Gene> genes;

	std::string decodedString = "";

	int value;

	int fitness;

	void decode() {
		ChromosomeDecoder::getInstance().decode(genes, decodedString, value);
		fitness = abs(settings.goal - value);
	}

public:
	Chromosome() {
		for (int i = 0; i < MAX_GENES; ++i) {
			genes.push_back(Gene());
		}
		decode();
	}	

	Chromosome(const std::vector<Gene> &genes) : genes(genes) {
		decode();
	}

	Chromosome(const Chromosome &lhs, const Chromosome& rhs) {
		int gIndex = rand() % MAX_GENES;
		
		for (int i = 0; i < MAX_GENES; ++i) {
			if (i < gIndex)
				genes.push_back(Gene(lhs.genes[i], settings.mutationRate));
			else
				genes.push_back(Gene(rhs.genes[i], settings.mutationRate));
		}
		decode();
	}

	friend std::ostream& operator<<(std::ostream &os, const Chromosome &chromosome) {
		os << chromosome.decodedString << "=" << chromosome.value;
		return os;
	}

	std::string const &DecodedString() const {
		return decodedString;
	}

	int const &Fitness() const {
		return fitness;
	}
};

