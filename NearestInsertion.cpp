#ifndef NEAREST_INSERTION_CPP
#define NEAREST_INSERTION_CPP

#include <iostream>
#include <algorithm>

#include "CountryClass.h"

using namespace std; 

void Country::nearestInsertion(){

	//Create a copy of the matrix from the class.
	vector< vector<double> > copyOfOriginalMatrix = getCountryMatrix();

	//Keeping track of the distance to each node.
	int closest;
	
	//Create a vector of cities to be visited
	list <Closest> citiesToBeVisited;

	//Create a vector to keep track of the cities that we have already visited
	list <int> visitedCities;

	//Get the total number of cities
	int totalCities = getNumberOfCitiesInTheCountry();

	//Populate the vector with the city numbers
	for(int i = 0; i < totalCities; i++){
		Closest thisCity;
		thisCity.currentCity = i;
		citiesToBeVisited.push_back(thisCity);
	}

	//Create a list for the tour
	list <int> nearestInsertionTour;

	//Get the degenerate tour
	findStartingDegenerateTour(nearestInsertionTour, citiesToBeVisited, visitedCities, copyOfOriginalMatrix);

	//create the rest of the tour.
	createTheTour(nearestInsertionTour, citiesToBeVisited, visitedCities, copyOfOriginalMatrix);

	cout << endl;
	//display the country information
	displayCountryInformation();

	//Calculate the distance of the tour and display it.
	double distance = 0;
	calculateDistanceOfTour(nearestInsertionTour, copyOfOriginalMatrix, distance);
	cout << "Total Distance of tour: " << distance << endl;

	//Display the final tour.
	displayTour(nearestInsertionTour);

}

//Finds the starting degenerate tour.
void Country::findStartingDegenerateTour(list <int> & currentTour, list <Closest> & citiesToVisit, list <int> & visitedCities,
					 const vector< vector<double> > & copyOfOriginalMatrix){

	//Set the smallest distance to a large number so that will get changed later on.
	double smallestDistance = 999999.99999;
	
	//Temp variables to hold the cities
	int tempStart;
	int tempEnd;

	//Get the total number of cities.
	int totalCities = getNumberOfCitiesInTheCountry();

	//We go through the adjacency matrix and look for the connection of two cities that have the samllest distance.
	for(int i = 0; i < totalCities; i++){
		for(int j = 0; j < totalCities; j++){
			///Checks for the initial smallest edge. The 2 cities that create this edge will make the degenerate subtour
			if((copyOfOriginalMatrix[i][j] != 0.0) && (copyOfOriginalMatrix[i][j] < smallestDistance)){
				//If it is less, make smallestDistance = to the distance at the current position in the matrix.
				smallestDistance = copyOfOriginalMatrix[i][j];
				//Set tempStart to i.
				tempStart = i;
				//Set tempEnd to j.
				tempEnd = j;
			}
		}
	}

	//We push back the start city into the subtour and update the list of cities that we have not yet visited as well as the 
	//list of cities that we have already visited.
	currentTour.push_back(tempStart);
	visitedCities.push_back(tempStart);
	updateToVisitCityList(citiesToVisit, tempStart);

	//Do the same here as above.
	currentTour.push_back(tempEnd);
	visitedCities.push_back(tempEnd);
	updateToVisitCityList(citiesToVisit, tempEnd);

	//We put push the first city back into the subtour to close the subtour
	currentTour.push_back(tempStart);

	//Set, at each unvisited city, the distance to the closest city that we have already visited.
	setClosestDistanceForEachNonVisitedCity(citiesToVisit, visitedCities, copyOfOriginalMatrix);

}

//Updates the list of cities that we have to visit
void Country::updateToVisitCityList(list <Closest> & citiesToVisit, int visitedCity){

	list<Closest>::iterator it;
	//Goes through the list of cities
	for(it = citiesToVisit.begin(); it != citiesToVisit.end(); it++){
		//If we at the current iterator position, it is the city that we have already visited, we erase it from the list.
		if(it -> currentCity == visitedCity){
			citiesToVisit.erase(it++);
		}
	}
}

//This function will set the shortest distance to every city already in the tour
void Country::setClosestDistanceForEachNonVisitedCity(list <Closest> & citiesToBeVisited, list <int> citiesVisited,
						      const vector< vector<double> > & copyOfOriginalMatrix){

	//initialize the iterator positions
	list <Closest>::iterator it1;
	list <int>::iterator it2;

	//Make the distance a large number so that it gets changed during the first comparison
	double shortestDistance = 9999999.99999;
	
	//Keep track of the cities.
	int currentCityNotInSubtour;
	int currentCityInSubtour;
	
	//Set tempCity to -1
	int tempCity = -1;

	//Go through the cities that still need to be visited.
	for(it1 = citiesToBeVisited.begin(); it1 != citiesToBeVisited.end(); it1++){
		//set the current city that is not in the subtour to whatever it is pointing to.
		currentCityNotInSubtour = (it1->currentCity);
		//Go through the cities that we have already visited.
		for(it2 = citiesVisited.begin(); it2 != citiesVisited.end(); it2++){
			//Set the current city in the subtour to w/e the 2nd iterator is pointing to.
			currentCityInSubtour = *(it2);
			//Find the smallest distance and set that as current the shortest distance
			if(copyOfOriginalMatrix[currentCityNotInSubtour][currentCityInSubtour] < shortestDistance){
				shortestDistance = copyOfOriginalMatrix[currentCityNotInSubtour][currentCityInSubtour];
				tempCity = currentCityInSubtour;
			}
		}

		//After the 2nd for loop is done, set the distance to the next closest city to w/e the shortest distance was from the
		//for loop.
		it1 -> distanceToNextClosestCity = shortestDistance;
		
		//Reset Values
		shortestDistance = 9999999.99999;
		tempCity = -1;
	}
	
}

//Finds the closest city.
int Country::findClosestCity(list <Closest> citiesToVisit){

	//Initialize the iterator
	list<Closest>::iterator it;

	//Initialize the closest city container and the current shortest distance container.
	int closestCity;
	double currentShortestDistance = 999999.9999;

	//Go through the list. 
	for(it = citiesToVisit.begin(); it != citiesToVisit.end(); it++){
		//If the distance at the currrent iterator position is less than the current shortest distance, set current
		//shortest distance to w/e distance the iterator is pointing to. Also, set closest city to the city of the 
		//iterator.
		if(it->distanceToNextClosestCity < currentShortestDistance){
			currentShortestDistance = it->distanceToNextClosestCity;
			closestCity = it->currentCity;
		}
	}

	//At the end, return the closest city
	return closestCity;
}

//Inserts a city into the tour.
void Country::insertACityIntoTheTour(int theClosestCity, list <int> & currentTour, list <Closest> & citiesToVisit, list <int> & visitedCities, 
				     const vector< vector<double> > & copyOfOriginalMatrix){

	//Initialize the to iterators.
	list <int>::iterator tourIT;
	list <int>::iterator tourIT2;

	//The distances are set to a high number so that they are changed during the first iteration through the loop.
	double totalDistanceChange = 99999.999999;
	double currentTotalDistanceChange = 99999.99999;
	
	//This is the city that we will be inserting in front of
	int vertexToKeepTrackOf;

	//Initialize variables for current visited city and next city in the edge.
	int currentVisitedCity;
	int nextCityInTheEdge;

	//Initialize the distances.
	double distance1;
	double distance2;
	double distanceOfEdge;

	//Go through the current tour.
	for(tourIT = currentTour.begin(); tourIT != currentTour.end(); tourIT++){
		//Set the second tour iterator to the next position after the iterator.
		tourIT2 = tourIT;
		++tourIT2;
		//Check if the next position is the end or not
		if(tourIT2 != currentTour.end()){
			//Set currentVisitedCity to the value that iterator 1 is pointing to.
			currentVisitedCity = *(tourIT);
			//Set the nextCityInTheEdge to what iterator 2 is pointing to.
			nextCityInTheEdge = *(tourIT2);

			//Get the 3 distances from the matrix.
			distance1 = copyOfOriginalMatrix[currentVisitedCity][theClosestCity];
			distance2 = copyOfOriginalMatrix[nextCityInTheEdge][theClosestCity];
			distanceOfEdge = copyOfOriginalMatrix[currentVisitedCity][nextCityInTheEdge];

			//Calculate the total change in the matrix.
			totalDistanceChange = calculateTotalChangeInDistance(distance1, distance2, distanceOfEdge);

			//If the current total dIstance change is greater than the new one we just obtained, change it to the 
			//new one that we just obtained.  
			if(currentTotalDistanceChange > totalDistanceChange){
				currentTotalDistanceChange = totalDistanceChange;
				//We will inserting in front of this vertex in the tour.
				vertexToKeepTrackOf = nextCityInTheEdge;
			}
		}
	}
	
	//Push back, into the list of cities that we have already visited, the closest city that we obtained.
	visitedCities.push_back(theClosestCity);

	//Update the tour to include the closest city
	updateTour(theClosestCity, vertexToKeepTrackOf, currentTour);

	//Update the list of cities that we still need to visit.
	updateToVisitCityList(citiesToVisit, theClosestCity);

	//Update the closest distances to the new tour from the cities that we have yet to visit.
	setClosestDistanceForEachNonVisitedCity(citiesToVisit, visitedCities, copyOfOriginalMatrix);
}

//we have to do the formula AX + AY - XY
double Country::calculateTotalChangeInDistance(double d1, double d2, double d3){
	
	double changeInDistance;

	changeInDistance = d1 + d2 - d3;

	return changeInDistance;
}

//We put the closest city into the tour.
void Country::updateTour(int cityToAddInToTour, int nextCityInTheEdge, list <int> & tour){

	list<int>::iterator it = find(tour.begin(), tour.end(), nextCityInTheEdge);
	int currentCity;

	if(it != tour.end()){
		tour.insert(it, cityToAddInToTour);
	}
}

//We create the tour.
void Country::createTheTour(list <int> & currentTour, list <Closest> & citiesToVisit, list <int> & visitedCities,
			    const vector< vector<double> > & copyOfOriginalMatrix){

	while(!citiesToVisit.empty()){

		//We find the closest city.
		int closestCity = findClosestCity(citiesToVisit);
	
		//We call the insertACityIntoTheTour function.
		insertACityIntoTheTour(closestCity, currentTour, citiesToVisit, visitedCities, copyOfOriginalMatrix);

	}
}


//Just goes through the tour list and couts the value at the current iterator position + 1. +1 because there is no city 0 in the
//file.
void Country::displayTour(list <int> currentTour){

	cout << "Nearest Insertion Tour: " << endl;
	//Declare the iterators. it will go through the tour while checkIT will check the next position in the list
	list <int>::iterator it;
	list <int>::iterator checkIT;
	
	//Go through the list to the end
	for(it = currentTour.begin(); it != currentTour.end(); it++){
		//Make checkIT point to w/e it is pointing to and then increment checkIT by 1 to check what the next thing in the list is
		checkIT = it;
		checkIT++;
		//If the next thing is not the end of the list cout the thing at the iterator position iwth the '->' addition
		if(checkIT != currentTour.end()){
			cout << *(it)+1 << " -> ";
		}
		//Else cout the thing at the iterator position without the '->' position
		else 
			cout << *(it)+1 << endl;
	}

}

//We calculate the total distance of the tour.
void Country::calculateDistanceOfTour(list <int> currentTour, const vector< vector<double> > & theMatrix, double & distance){

	//initialize the iterators.
	list <int>::iterator it;
	list <int>::iterator it2;

	//keep track of the cities.
	double city1;
	double city2;

	//going through the tour, let city1 be w/e it is pointing to.
	for(it = currentTour.begin(); it != currentTour.end(); it++){
		
		city1 = *(it);
		it2 = it;
		++it2;
		
		//Make sure that it2 doesn't go past the end of the list. If it is not past the end of the list, add the distance.
		if(it2 != currentTour.end()){
			city2 = *(it2);

			distance += theMatrix[city1][city2];
		}
	
	}
}

#endif //NEAREST_INSERTION_CPP
