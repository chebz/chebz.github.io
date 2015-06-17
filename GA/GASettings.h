#include <string>
#include <sstream>
#define settings GASettings::getInstance()

class GASettings {
public:
	static GASettings& getInstance()
	{
		static GASettings instance;
		return instance;
	}

	int initialPopulation = 1000;
	float mutationRate = 0.01f;
	int goal = 10;
	int maxGenerations = 1000;
	float crossoverRate = 0.7f;

	std::string print() {
		std::stringstream ostr;
		ostr << "initialPopulation: " << initialPopulation << 
			", mutationRate: " << mutationRate << 
			", goal: " << goal <<
			", maxGenerations: " << maxGenerations <<
			", crossoverRate: " << crossoverRate;
		return ostr.str();
	}

private:
	GASettings() {}

	GASettings(GASettings const&) = delete;

	void operator=(GASettings const&) = delete;
};