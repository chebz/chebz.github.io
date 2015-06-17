#include <vector> 
#include <map>
#include <string> 
#include <stdlib.h>
#include <iostream>
#pragma once
#define GENE_SIZE 4

class GeneDecoder
{
public:
	static GeneDecoder& getInstance()
	{
		static GeneDecoder instance;
		return instance;
	}

private:
	std::map<std::string, std::string> data;

	GeneDecoder() {
		data["0000"] = "0";
		data["0001"] = "1";
		data["0010"] = "2";
		data["0011"] = "3";
		data["0100"] = "4";
		data["0101"] = "5";
		data["0110"] = "6";
		data["0111"] = "7";
		data["1000"] = "8";
		data["1001"] = "9";
		data["1010"] = "+";
		data["1011"] = "-";
		data["1100"] = "*";
		data["1101"] = "/";
	};

	GeneDecoder(GeneDecoder const&) = delete;

	void operator=(GeneDecoder const&) = delete;

public:
	std::string decode(const std::string &bits) const {
		auto it = data.find(bits);
		if (it != data.end())
			return it->second;
		return "";
	}
};

class Gene
{
	friend class GeneDecoder;

private:
	std::string bits = "";

	std::string decodedString = "";

	bool shouldMutate(float mutationRate) const {
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		return r <= mutationRate;
	}

	void decode() { 
		decodedString = GeneDecoder::getInstance().decode(bits); 
	}
		 
public:
	Gene() {		
		for (int i = 0; i < GENE_SIZE; ++i) {
			bits.push_back(rand() % 2 == 1 ? '1' : '0');
		}
		decode();
	}

	Gene(const std::string &bits) : bits(bits) { decode(); }

	Gene(const Gene &other) : bits(other.bits), decodedString(other.decodedString) {}

	Gene(const Gene &other, float mutationRate) {
		for (int i = 0; i < GENE_SIZE; ++i) {
			char b = other.bits[i];
			if (shouldMutate(mutationRate))
				b = b == '0' ? '1' : '0';
			bits.push_back(b);
		}
		decode();
	}

	friend std::ostream& operator<<(std::ostream &os, const Gene &gene) {
		os << gene.decodedString;
		return os;
	}

	std::string const &DecodedString() const {
		return decodedString;
	}

	std::string const &Bits() const {
		return bits;
	}
};

