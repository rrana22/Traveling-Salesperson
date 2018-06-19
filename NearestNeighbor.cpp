#ifndef NEAREST_NEIGHBOR_CPP
#define NEAREST_NEIGHBOR_CPP

#include <iostream>
#include <list>
#include <cstdlib>

#include "CountryClass.h"

using namespace std; 

//Main funtion call that will be used in the main function.
void Country::nearestNeighbor(){	

	//The size is the the number of cities in the country.
	int sizeOfCountry = getNumberOfCitiesInTheCountry();
  
	//Makes a copy of the adjacency matrix.
	vector < vector<double> > copyOfOriginalCityMatrix = getCountryMatrix();

	//Initializing the citiesVisited vector as well as the distance.
	vector <int> citiesVisited;
	double distance;


	//Initialize the cities that we have/haven't visited yet.
	list <int> citiesNotYetVisited;
	list <int> citiesThatWeHaveAlreadyVisited;

	//Fill the list of cities with all the cities
	for(int  i = 0; i < sizeOfCountry; i++){
		citiesNotYetVisited.push_back(i);
	}  

	//Find a random starting city. Its sizeOfCountry - 1 because when I do only sizeOfCountry, Luxembourg gives me a segementation fault
	//for some reason.
	int startingCity = rand() % sizeOfCountry-1;
	
	//Need to keep track of the initial starting city to add back into the list of visited cities at the end.	
	int tracker = startingCity;

	//We put the starting city into the list of cities that we have already visited since it is the city we start at and we we remove
	//it from the list of cities that we have not yet visited.
	citiesThatWeHaveAlreadyVisited.push_back(startingCity);
	citiesNotYetVisited.remove(startingCity);
	
	//Find the tour
	getNearestNeighborTour(citiesNotYetVisited, citiesThatWeHaveAlreadyVisited, copyOfOriginalCityMatrix, startingCity);
	
	//Push back the first city again to complete the tour.
	citiesThatWeHaveAlreadyVisited.push_back(tracker);
	
	//Get the distance of the tour.
	distance = getTotalDistance(citiesThatWeHaveAlreadyVisited, copyOfOriginalCityMatrix);

	//Display the information of the tour.
	cout << endl;
	displayCountryInformation();
	cout << "Distance of the tour: " << distance << endl; 
	displayNearestNeighborTour(citiesThatWeHaveAlreadyVisited);
	
}

//Finds the nearest city to the current city that is passed into the function.
int Country::findNearestCityToCurrentCity(int ourCurrentCity, list <int> citiesNotYetVisited, const vector< vector<double> > & 						  copyOfOriginalMatrix){

	//Initialize the shortest distance to a lard number so that we are sure that it changes.
	double shortestDistance = 999999.99999;

	//Initalize the city to return so that we can return the nearest city
	int cityToReturn;

	//The iterator to go through the list of cities we have not yet visited
	list <int>::iterator notVisitedIT;

	//Goes through the list of cities not visited yet
	for(notVisitedIT = citiesNotYetVisited.begin(); notVisitedIT != citiesNotYetVisited.end(); notVisitedIT++){

		//I put the value that the iterator is pointing to in the int variable to make the code look cleaner.
		int currentNotVisitedCity = *(notVisitedIT);
		
		//Put the current distance at the position in the matrix into a variable to make code look cleaner.
		double currentDistance = copyOfOriginalMatrix[ourCurrentCity][currentNotVisitedCity];
		
		//If the current distance is not 0 (not the same city) and is less than the previous shortest distance, 
		if((currentDistance != 0.0) && (currentDistance < shortestDistance)){
			//Update the shortest distance and the city to return
			shortestDistance = copyOfOriginalMatrix[ourCurrentCity][currentNotVisitedCity];
			cityToReturn = currentNotVisitedCity;
		}
	}
	
	//return the city that is the closest.
	return cityToReturn;
}

//This function will help create the tour
void Country::getNearestNeighborTour(list <int> & citiesNotYetVisited, list <int> & citiesThatWeHaveAlreadyVisited, 
			   	     const vector< vector<double> > & copyOfOriginalMatrix, int startCity){

	//Initalize to help keep track of the closest city.
	int closestCity;
	//The start city will be the city from the function call. We will be updating this variable later, however.
	int startingCity = startCity;

	//Go through the cities not visited. Find the city with the shortest distance to the current city that we have already visited,
	//add that to the list of cities that we have visited, remove it from the list of cities we haven't visited and then do it again.	
	while(!citiesNotYetVisited.empty()){

		//Set the closest city to the city that is returned from the findNearestCityToCurrentCity function.
		closestCity = findNearestCityToCurrentCity(startingCity, citiesNotYetVisited, copyOfOriginalMatrix);

		//Remove the city from the cities that we have not yet visited and add the city into the list of cities that we have visited.
		citiesNotYetVisited.remove(closestCity);
		citiesThatWeHaveAlreadyVisited.push_back(closestCity);
			
		//Set the starting city to the closest city (the city we got from the function).
		startingCity = closestCity;

	}

}

//Gets the total distance of the tour.
double Country::getTotalDistance(list <int> citiesThatWeHaveAlreadyVisited, const vector< vector<double> > countryMatrix){

	//Declare the iterators to traverse through the list of cities that we have isited (tour).
	list <int>::iterator currentCityIT;
	list <int>::iterator nextCityIT;

	//Declare the city variables. The will hold the values that the iterators will point to.
	int city1;
	int city2;
	
	//Initialize the distance to 0.
	double distance = 0;
	
	//Go through the list of list of cities that we hae visited
	for(currentCityIT = citiesThatWeHaveAlreadyVisited.begin(); currentCityIT != citiesThatWeHaveAlreadyVisited.end(); currentCityIT++){
		//Make the nextCity iterator point to w/e currentCity iterator is pointing to then increment it by 1 to get the next city
		//in the tour..
		nextCityIT = currentCityIT;
		nextCityIT++;
	
		//Make city1 = to w/e currentCityIT is pointing to and make city2 = to w/e nextCityIT is pointing to.
		city1 = *(currentCityIT);
		city2 = *(nextCityIT);

		if(nextCityIT != citiesThatWeHaveAlreadyVisited.end()){
			//Get the distance of the edge between the two cities and add that edge into the distance.
			distance += countryMatrix[city1][city2];
		}
	}
	//return the distance
	return distance;
}

//Displays the tour 
void Country::displayNearestNeighborTour(list <int> visitedCities){
	//Cout statement
	cout << "The Nearest Neighbor Tour: " << endl;

	//Declare the iterators to go through the list
	list <int>::iterator visitedCitiesIT;
	list <int>::iterator checkIT;

	//Go through the entire list
	for(visitedCitiesIT = visitedCities.begin(); visitedCitiesIT != visitedCities.end(); visitedCitiesIT++){
		
		//Make the check iterator point to w/e visitedCitiesIT is pointing to and then increment checkIT by 1.
		//This will check if the next position is the last element in the list or not.
		checkIT = visitedCitiesIT;
		checkIT++;
		
		//If the next position is not the end of the list, cout the element in the list + 1. EX: it would make city 0 = 1
		if(checkIT != visitedCities.end()){
			cout << *(visitedCitiesIT)+1 << " -> ";
		}

		//Else we cout the city at the end position without the '->' symbol
		else
			cout << *(visitedCitiesIT)+1 << endl;
	
	}

}

#endif //NEAREST_NEIGHBOR_CPP
