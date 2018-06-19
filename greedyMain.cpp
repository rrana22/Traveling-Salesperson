//GreedyTSP program. The functions for the GreedyTSP are in GreedyTSP.cpp

#include <iostream>
#include <chrono>

#include "CountryClass.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char *argv[]){


	if(argc != 2){
		cout << "Please enter a file name with the executable call." << endl;
		return 0;
    	}
	
	const string file(argv[1]);

	Country currentCountry;
	currentCountry.readInputFile(file);

	double time;

	auto startTime = steady_clock::now();
	currentCountry.greedyTSP();
	auto endTime = steady_clock::now();
	auto diff = endTime - startTime;
	time = duration <double, milli> (diff).count();

	cout << "Time taken to do Greedy TSP: " << time << " ms" << endl << endl;
	
	return 0;
}
