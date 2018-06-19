#ifndef COUNTRY_CLASS_H
#define COUNTRY_CLASS_H

#include <iostream> 
#include <vector>
#include <queue>
#include <list>

using namespace std;

class Country{

public:
	//The Cities struct
	struct Cities{
		int cityNumber;
		float xCoordinate;
		float yCoordinate;
	};

	//For Nearest Insertion
	struct Closest{
		int currentCity = 0;
		double distanceToNextClosestCity = 0.0;
	};

	//Distance struct. Will contain the distance for the 2 cities in the struct.
	struct Distance{
		double distance;
		int city1;
		int city2;
	};

	//Comparison for the primary queue
	class CompareDistance{
	public:

		bool operator()(Distance d1, Distance d2){
			return(d1.distance > d2.distance);
		}
	};

	//Node struct
	struct Node{

		int currentCity;
		int numberOfEdges = 0;
		int connectedCity1 = 0;
		int connectedCity2 = 0;
		
	};

	//Function to read the input file
	void readInputFile(string file);
	
	//Create the adjacency matrix
	void createAdjacencyMatrix();

	//Function used to find the distance between two cities
	double distanceBetweenTwoCities(double city1XCoordinate, double city1YCoordinate, 
					double city2XCoordinate, double city2YCoordinate);

	//Remove any duplicates from the priority queue
	void removeDuplicates(priority_queue<Distance, vector<Distance>, CompareDistance> tempPQ);

	//Display Function
	void displayCountryInformation();
	void displayCountryMatrix();
	void displayPriorityQueue();
	void displayVectorOfCities();

	//Setter functions
	void setCountryName(string line);
	void setNumberOfCitiesInCountry(string line);
	void setCityInCitiesVector(Cities currentCity){ citiesInTheCountry.push_back(currentCity); }
	void setCountryMatrix(vector< vector<double> > matrix){ countryMatrix = matrix; }
	void setPriorityQueue(priority_queue<Distance, vector<Distance>, CompareDistance> tempPQ){ distancesPQ = tempPQ; }
	
	//Getter functions
	string getCountryName(){ return countryName; }
	int getNumberOfCitiesInTheCountry(){ return numberOfCitiesInTheCountry; }
	vector <Cities> getCitiesInTheCountryVector(){ return citiesInTheCountry; }
	vector < vector<double> > getCountryMatrix(){ return countryMatrix; }
	priority_queue<Distance, vector<Distance>, CompareDistance> getPQ(){ return distancesPQ; }

	//Called in rana1.cpp 
	void greedyTSP();

	//Called in rana2.cpp
	void nearestInsertion();

	//Called in rana3.cpp
	void nearestNeighbor();

private:

	string countryName;
	int numberOfCitiesInTheCountry;
	vector <Cities> citiesInTheCountry;
	vector < vector<double> > countryMatrix; 
	priority_queue<Distance, vector<Distance>, CompareDistance> distancesPQ;

	///////////////////////////////////////////Greedy TSP (Part 1)////////////////////////////////////////////////////////////////////////////
	
	void insertIntoVectorOfNodes(int City1, int City2, vector <Node> & vecOfNodeConnections, vector <int> & tourVector);

	bool isCycle(int City1_p, int City2_p, const vector <int> & tourVector);

	void createTheTour(const vector <Node> & nodeConnections, vector <int> & tour, vector <int> & pastCities, int & cityToAddToTour);

	void displayTheTour(const vector <int> & theTour);

	void completeTheNodes(vector <Node> & nodes);

	double calculateDistanceOfTour(const vector <int> & tour);

	///////////////////////////////////////////Nearest Insertion (Part 2)/////////////////////////////////////////////////////////////////////

	void findStartingDegenerateTour(list <int> & currentTour, list <Closest> & citiesToVisit, list <int> & visitedCities,
					const vector< vector<double> > & copyOfOriginalMatrix);

	void setClosestDistanceForEachNonVisitedCity(list <Closest> & citiesToBeVisited, list <int> citiesVisited,
						     const vector< vector<double> > & copyOfOriginalMatrix);

	void createTheTour(list <int> & currentTour, list <Closest> & citiesToVisit, list <int> & visitedCities,
			   const vector< vector<double> > & copyOfOriginalMatrix);

	int findClosestCity(list <Closest> citiesToVisit);

	void insertACityIntoTheTour(int theClosestCity, list <int> & currentTour, list <Closest> & citiesToVisit, list <int> & visitedCities, 
				    const vector< vector<double> > & copyOfOriginalMatrix);

	void updateTour(int cityToAddInToTour, int nextCityInTheEdge, list <int> & tour);

	double calculateTotalChangeInDistance(double d1, double d2, double d3);

	void updateToVisitCityList(list <Closest> & citiesToVist, int visitedCity);

	void displayTour(list <int> currentTour);

	void calculateDistanceOfTour(list <int> currentTour, const vector< vector<double> > & theMatrix, double & distance);

	int findNextCity(const vector <Node> & nodeConnections, vector <int> & tour, vector <int> & pastCities, int cityToAddToTour);

	///////////////////////////////////////////Nearest Neighbor (Part 3)//////////////////////////////////////////////////////////////////////

	void getNearestNeighborTour(list <int> & citiesNotYetVisited, list <int> & citiesThatWeHaveAlreadyVisited, 
			   	    const vector< vector<double> > & copyOfOriginalMatrix, int startCity);

	int findNearestCityToCurrentCity(int ourCurrentCity, list <int> citiesNotYetVisited, 
					 const vector< vector<double> > & copyOfOriginalMatrix);

	double getTotalDistance(list <int> citiesThatWeHaveAlreadyVisited, const vector< vector<double> > countryMatrix);

	void displayNearestNeighborTour(list <int> visitedCities);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

};


#endif //COUNTRY_CLASS_H
