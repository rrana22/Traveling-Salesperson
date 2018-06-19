//Nearest Insertion program. The functions for nearest insertion are in NearestInsertion.cpp

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

	//start keeping track of the time.
	auto startTime = steady_clock::now();
	currentCountry.nearestInsertion();
	auto endTime = steady_clock::now();
	auto diff = endTime - startTime;
	time = duration <double, milli> (diff).count();
	//finish keep tracking of time.
	
	//Cout how long the program took to create the tour
	cout << "Time taken to do Nearest Insertion: " << time << " ms" << endl << endl;

	return 0;
}
