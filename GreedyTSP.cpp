#ifndef GREEDY_TSP_CPP
#define GREEDY_TSP_CPP

#include <iostream>
#include <list>

#include "CountryClass.h"

void Country::greedyTSP(){

	//Get a copy of the priority queue from the class
	priority_queue<Distance, vector<Distance>, CompareDistance> copyOfDistancePQ;
	copyOfDistancePQ = getPQ();

	int numberOfCities = getNumberOfCitiesInTheCountry();

	//Create a vector of nodes for each city that has a starting size of the number of cities.
	vector <Node> vectorOfConnections(numberOfCities);

	//Populates the vector with the correct city number. Ex: position 1 in the vector will be city 1 so on and so forth.
	for(int i = 0; i < vectorOfConnections.size(); i++){
		vectorOfConnections[i].currentCity = i+1;
	}

	//Declared a Distance object to get the distance from the priority queue.
	Distance distanceOfCurrentCities;

	//Initalize every subset to -1. This will be used to check for cycles.
	vector <int> tourVectorToCheckForCycle(numberOfCities);
	for(int i = 0; i < tourVectorToCheckForCycle.size(); i++){
		tourVectorToCheckForCycle[i] = -1;
	}

	//Get the starting city of the tour
	Distance startCity;
	startCity = copyOfDistancePQ.top();

	int startingCityInTour;
	startingCityInTour = startCity.city1;

	//Go through the priority queue until it is empty.
	while(!copyOfDistancePQ.empty()){
		
		//Create a temp variable to hold the top of the priority queue.
		distanceOfCurrentCities = copyOfDistancePQ.top();

		//Put the 2 cities that make up the edge into their own variables.
		int City1 = distanceOfCurrentCities.city1;
		int City2 = distanceOfCurrentCities.city2;

		//Insert the nodes into the vector of nodes to form the connections.
		insertIntoVectorOfNodes(City1, City2, vectorOfConnections, tourVectorToCheckForCycle);

		//Pop off the top element from the PQ
		copyOfDistancePQ.pop();
	}

	//Create the vector tour
	vector <int> tour;
	
	//Initialize the visited cities vector.
	vector <int> visitedCities(numberOfCities);

	//Complete the node connection if at the end of the node insertions, there are 2 cities that are not connected.This completes the 
	//cycle.
	completeTheNodes(vectorOfConnections);

	//Create the tour from the node connections.
	createTheTour(vectorOfConnections, tour, visitedCities, startingCityInTour);

	//Display the greedy tour informtion.
	cout << endl;
	displayCountryInformation();

	//Calculate the total distance.
	double distance;
	distance = calculateDistanceOfTour(tour);

	//Display the distance
	cout << "Distance of tour: " << distance << endl;
	
	//Display the greedy tour
	cout << "The Greedy Tour: " << endl;
	displayTheTour(tour);
	
}

//Insert the nodes into the vector of nodes
void Country::insertIntoVectorOfNodes(int City1, int City2, vector <Node> & vecOfNodeConnections, vector <int> & tourVector){
	//Create a variable to check for cycles.
	bool cycle;
	
	//If any of the cities have 2 edges already, ignore the edge we are looking to insert.
	if(vecOfNodeConnections[City1 - 1].numberOfEdges == 2 || vecOfNodeConnections[City2 - 1].numberOfEdges == 2){
		
	}
	//Else we enter this:
	else{
		//Check to see if the cities have no edges. If they don't, connect the two cities to each other and increase their 
		//number of edges by 1. City - 1 because, for example, city 1 will be in position 0 of the vector. 
		if((vecOfNodeConnections[City1 - 1].numberOfEdges == 0) && (vecOfNodeConnections[City2 - 1].numberOfEdges == 0)){
			
			vecOfNodeConnections[City1 - 1].connectedCity1 = City2;
			vecOfNodeConnections[City2 - 1].connectedCity1 = City1;

			vecOfNodeConnections[City1 - 1].numberOfEdges += 1;
			vecOfNodeConnections[City2 - 1].numberOfEdges += 1;
			
			//Make the subset for both cities that we just connected to one city. This will show that both cities are part of the
			//same subset. This will be useful in checking for cycles.
			tourVector[City1 - 1] = City1;
			tourVector[City2 - 1] = City1;
		}
		//Check if one city has 1 edge and the second city has no edges. If this is the case, set the 2nd connected city to whichever
		//city has 0 edges and set connectedCity1 to the city with an already existing edge.
		else if((vecOfNodeConnections[City1 - 1].numberOfEdges == 1) && (vecOfNodeConnections[City2 - 1].numberOfEdges == 0)){

			vecOfNodeConnections[City1 - 1].connectedCity2 = City2;
			
			vecOfNodeConnections[City2 - 1].connectedCity1 = City1;

			vecOfNodeConnections[City1 - 1].numberOfEdges += 1;
			vecOfNodeConnections[City2 - 1].numberOfEdges += 1;

			//Set the subset of the city with no edges to the same subset as the city with a pre-existing subset.
			tourVector[City2 - 1] = tourVector[City1 - 1];

		}	

		//Do the same as above
		else if((vecOfNodeConnections[City1 - 1].numberOfEdges == 0) && (vecOfNodeConnections[City2 - 1].numberOfEdges == 1)){

			vecOfNodeConnections[City1 - 1].connectedCity1 = City2;
			
			vecOfNodeConnections[City2 - 1].connectedCity2 = City1;

			vecOfNodeConnections[City1 - 1].numberOfEdges += 1;
			vecOfNodeConnections[City2 - 1].numberOfEdges += 1;

			tourVector[City1 - 1] = tourVector[City2 - 1];
		}

		//If both cities have one edge each, then we have to check if connecting the 2 cities will end up forming a cycle or not.
		else if((vecOfNodeConnections[City1 - 1].numberOfEdges == 1) && (vecOfNodeConnections[City2 - 1].numberOfEdges == 1)){
			//Set cycle to w/e the isCycle function returns.
			cycle = isCycle(City1, City2, tourVector);
	
			//If there is no cycle:
			if(!cycle){
				//connect the two cities and increase both of their edges by 1.
				vecOfNodeConnections[City1 - 1].connectedCity2 = City2;
				
				vecOfNodeConnections[City2 - 1].connectedCity2 = City1;

				vecOfNodeConnections[City1 - 1].numberOfEdges += 1;
				vecOfNodeConnections[City2 - 1].numberOfEdges += 1;
				
				//set the subset of city 2 to the subset of city 1.
				for(int  i = 0; i < tourVector.size(); i++){
					//Don't want to change city 2's subset until all cities in city 2's subset are changed.
					if(i == City2 - 1){ }
					//Change the subset of the cities in the same subset as city 2 to the subset of city 1 
					else if(tourVector[i] == tourVector[City2 - 1]){
						tourVector[i] = tourVector[City1 - 1];
					}
					
				}	
				//Set city 2's subset to city 1's subset
				tourVector[City2 - 1] = tourVector[City1 - 1]; 
			}

			//Else ignore
			else{  }

		}

	}
}

//Check for whether a cycle will form or not.
bool Country::isCycle(int City1_p, int City2_p, const vector <int> & tourVector){

	//If the 2 cities are part of the same subset, return true so that program knows a cycle will form
	if(tourVector[City1_p - 1] == tourVector[City2_p - 1]){

		return true;

	}

	//Else return false so the program knows that a cycle will not form
	else 
		return false;

}

//If there are 2 nodes that are not complete in their connection because if they were, a cycle would form. This function connects them after all other nodes are connected.
void Country::completeTheNodes(vector <Node> & nodes){

	//This is to form the complete cycle (finish the tour)
	int tempCity1;
	int tempCity2;
	
	//Find the first city with only 1 edge.
	for(int i = 0; i < nodes.size(); i++){
		if(nodes[i].numberOfEdges == 1){
			tempCity1 = nodes[i].currentCity;
		}
	}

	//Find the 2nd city with only 1 edge
	for(int  i = 0; i < nodes.size(); i++){
		//If we encounter the first city, we ignore it since we are already keeping track of it.
		if(nodes[i].currentCity == tempCity1){ }
		//Once we find the 2nd city with only 1 edge, we keep track of it.
		else if((nodes[i].currentCity != tempCity1) && (nodes[i].numberOfEdges == 1)){
			tempCity2 = nodes[i].currentCity;
		}
	}

	//connect them
	nodes[tempCity1 - 1].connectedCity2 = tempCity2;
	nodes[tempCity2 - 1].connectedCity2 = tempCity1;

}

//Create the tour from the vector of node connections
void Country::createTheTour(const vector <Node> & nodeConnections, vector <int> & tour, vector <int> & pastCities, 
			    int & cityToAddToTour){

	//Keep track of the starting city of the tour
	int startCity = cityToAddToTour;

	//Add the starting city to the beginning of the tour and mark it as visited
	tour.push_back(cityToAddToTour);
	pastCities[cityToAddToTour - 1] = cityToAddToTour;	

	//Add the 2nd city connection to the tour and mark it as visited
	cityToAddToTour = nodeConnections[cityToAddToTour - 1].connectedCity1;
	tour.push_back(cityToAddToTour);
	pastCities[cityToAddToTour - 1] = cityToAddToTour;	
 
	//While the tour size is less than the toal number of cities
	while(tour.size() < nodeConnections.size()){

		//Find the next city to add to the tour by using the find next city function
		int nextCityToAdd = findNextCity(nodeConnections, tour, pastCities, cityToAddToTour);

		//add the nextCityToAdd to the tour and mark it as visited.
		tour.push_back(nextCityToAdd);
		pastCities[nextCityToAdd - 1] = nextCityToAdd;

		//Put the next city in the cityToAddToTour variable to find its connected city to add
		cityToAddToTour = nextCityToAdd;

	}

	//After the while loop is done, add the start city at the end of the tour vector to finish the cycle/tour
	tour.push_back(startCity);
}

//Find the next city to add to the tour
int Country::findNextCity(const vector <Node> & nodeConnections, vector <int> & tour, vector <int> & pastCities, int cityToAddToTour){

	//Initialize a variable to hold the city that will be the next city in the tour
	int nextCityToVisitInTour;	

	//The next city to visit will be connectedCity1
	nextCityToVisitInTour = nodeConnections[cityToAddToTour - 1].connectedCity1;

	//However, if we have already visited connected city 1, go to connected city 2
	if(pastCities[nextCityToVisitInTour - 1] != 0){
		nextCityToVisitInTour = nodeConnections[cityToAddToTour - 1].connectedCity2;
	}
	
	//Return the city
	return nextCityToVisitInTour;
}
//Calculate the distance of the tour
double Country::calculateDistanceOfTour(const vector <int> & tour){
	
	//Initialzie the distance to 0
	double distance = 0;

	//Create variables to gold cities 1 and 2
	int city1;
	int city2;

	//Get a copy of the matrix
	vector< vector<double> > copyOfMatrix = getCountryMatrix();

	//Go through the tour and add up the distance of the current city (city1) and the next city (city) in the tour
	for(int i = 0; i < tour.size() - 1; i++){

		city1 = tour[i] - 1;
		city2 = tour[i+1] - 1;

		//Get the distance of the edge from the adjacency matrix
		distance += copyOfMatrix[city1][city2];

	}

	//return the distance value
	return distance;

}

//Display the tour
void Country::displayTheTour(const vector <int> & theTour){

	//Go through the tour and cout the city at the current position
	for(int  i = 0; i < theTour.size()-1; i++){
		cout << theTour[i] << " -> ";
	}

	//Cout the last position of the tour
	cout << theTour[theTour.size()-1] << endl;
}

#endif //GREDDY_TSP_CPP
