//Nearest Neighbor program. The functions for nearest neighbor are in NearestNeighbor.cpp
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

	//To contain the time it took to get the tour.
	double time;

	//Finds the tour as well as the time it took to find the tour.
	auto startTime = steady_clock::now();
	currentCountry.nearestNeighbor();
	auto endTime = steady_clock::now();
	auto diff = endTime - startTime;
	time = duration <double, milli> (diff).count();

	cout << "Time taken to do Nearest Neighbor: " << time << " ms" << endl << endl;

	return 0;
}
