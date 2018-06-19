#ifndef COUNTRY_CLASS_CPP
#define COUNTRY_CLASS_CPP

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <stdlib.h>
#include <queue>

#include "CountryClass.h"

using namespace std;

//This function will find the name of the country from line 2 and set it in the Country class.
void Country::setCountryName(string line){

	//Will hold the name of the country.
	string nameOfCountry;

	//First I look for the word 'in'. Since all of the lines for every city have the same sentence structure after
	//the city number, I decided to look for the word 'in' since it is the last word before the country name. 
	//The position of the start of the word 'in' changes depending on how many cities there are in the country. 
	//So this way, this line of code will take care of all cases where the word 'in' might be. 
	int positionOfIn = line.find("in");
	
	//Once I find the word 'in', the word 'in' takes up 2 spaces and then the space after the word 'in' and the start of the
	//next word takes up another space. So altogether, will get everything after the start of the word 'in'.
	nameOfCountry = line.substr(positionOfIn + 3);

	//Sets the name of the country in the class.
	countryName = nameOfCountry;
}

//This function will find the number of cities in the country.
void Country::setNumberOfCitiesInCountry(string line){

	//Temp variable will hold the number of cities in string form.
	string temp;

	//Number of cities variable will hold the number of cities in int form.
	int numberOfCities;

	//Finds the number of countries in string form first. I hard coded 12 into the substring function because the 
	//line structure is the same for all of the countries and there is no change to the line until the start of the number
	//countries, which is at the end of the line. It will take 12 positions to get to the number.
	temp = line.substr(12);

	//This converts the number in string form to into int form.
	numberOfCities = atoi(temp.c_str());

	//Sets the number of cities in the class
	numberOfCitiesInTheCountry = numberOfCities;
}

//Removes the duplicates from the priority queue so that there is only one of every distance in the queue. Then sets the new
//queue as the primary queue of the class.
void Country::removeDuplicates(priority_queue<Distance, vector<Distance>, CompareDistance> tempPQ){

	priority_queue<Distance, vector<Distance>, CompareDistance> tempForClassPQ;
	Distance tempDistance;

	while(!tempPQ.empty()){
		tempDistance = tempPQ.top();
		tempPQ.pop();
		tempPQ.pop();

		tempForClassPQ.push(tempDistance);
	}

	setPriorityQueue(tempForClassPQ);
}
//Function to calculate the distance between 2 cities. 
double Country::distanceBetweenTwoCities(double city1XCoordinate, double city1YCoordinate, double city2XCoordinate, double city2YCoordinate){

	//Initalized variables 
	double xBase, yBase, xTemp, yTemp, distance, sumOfXAndY;
	double power = 2.0;

	//xTemp will hold the subtraction of x-coordinate of city 2 and city1 and xBase will hold the squared value of xTemp.
	xTemp = (city2XCoordinate - city1XCoordinate);
	xBase = pow(xTemp, power);

	//yTemp will hold the subtraction of y-coordinate of city 2 and city1 and yBase will hold the squared value of yTemp.
	yTemp = (city2YCoordinate - city1YCoordinate);
	yBase = pow(yTemp, power);

	//Add together the 2 base values.
	sumOfXAndY = xBase + yBase;

	//Find the square root of the two base values. This will be the distance.
	distance = sqrt(sumOfXAndY);

	//return the distance.
	return distance;
}


//This function will create the adjacency matrix. The adjacency matrix will hold all of the distances between the cities.
void Country::createAdjacencyMatrix(){

	//Self explanatory containers variables. c1 = city 1 and c2 = city 2.
	double c1XCoordinate, c1YCoordinate, c2XCoordinate, c2YCoordinate, distance, temp1, temp2;
	
	//Number of columns and rows
	int numberOfColumns = getNumberOfCitiesInTheCountry();
	int numberOfRows = getNumberOfCitiesInTheCountry();

	//Copy the vector of cities from the private section into citiesInTheCountry
	vector <Cities> citiesInCountry;
	citiesInTheCountry = getCitiesInTheCountryVector();

	Distance newDist;
	priority_queue<Distance, vector<Distance>, CompareDistance> tempPQ;

	//Create a 2 dimensional vector
	vector < vector<double> > citiesMatrix;
	//resize the 2 dimensional vector so that it has the same amount of rows and columns.
	citiesMatrix.resize(numberOfColumns, vector<double>(numberOfRows));
	
	//Go through the number of rows
	for(int i = 0; i < numberOfRows; i++){
		//Go through the number of columns
		for(int j = 0; j < numberOfColumns; j++){
			
			//This if statement checks for the case where the i and j values are the same and sets the distance of that to
			//0. The i and j values will be the same when the 2 cities that are being checked are actually the same city.
			if(i == j){
				citiesMatrix[i][j] = 0.0;
			}

			//Else do the following:
			else{

				//Get the x-coordinate and y-coordinate of city 1
				c1XCoordinate = citiesInTheCountry[i].xCoordinate;
				c1YCoordinate = citiesInTheCountry[i].yCoordinate;

				//Get the x-coordinate and y-coordinate of city 2
				c2XCoordinate = citiesInTheCountry[j].xCoordinate;
				c2YCoordinate = citiesInTheCountry[j].yCoordinate;

				//Calculate the distance between the 2 cities.
				distance = distanceBetweenTwoCities(c1XCoordinate, c1YCoordinate, c2XCoordinate, c2YCoordinate);
				
				//Push the cities into a priority queue. I do +1 because there should be no city 0 and since we traverse through
				//the matrix from position 0 to < size, we have to add 1 to each city number to get the correct corresponding
				//city
				newDist.distance = distance;
				temp1 = i;
				temp2 = j;
				newDist.city1 = temp1 + 1;
				newDist.city2 = temp2 + 1;

				if(newDist.distance != 0){
					tempPQ.push(newDist);
				}

				//Set the distance between the two cities into the adjacency matrix.
				citiesMatrix[i][j] = distance;
			}
		}
	}
	//After all that is done, copy the matrix from this function into the initialized matrix of the country class 
	//that was passed into this function.
	setCountryMatrix(citiesMatrix);

	//Since we are putting the distances as we are getting them from the adjacency matrix, there will be duplicates of the 
	//numbers. This function will remove the duplicates.
	removeDuplicates(tempPQ);

}

//Displays the adjacency matrix in the class
void Country::displayCountryMatrix(){

	int rows = getNumberOfCitiesInTheCountry();
	int columns = getNumberOfCitiesInTheCountry();

	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			cout << countryMatrix[i][j] << " ";
		}
		cout << endl;
	}
}

//Displays the country information
void Country::displayCountryInformation(){

	cout << "Country name: " << countryName << endl;
	cout << "Cities in the country: " << numberOfCitiesInTheCountry << endl;

}

//Displays the top element of the PQ and then pops it off.
void Country::displayPriorityQueue(){

	priority_queue <Distance, vector<Distance>, CompareDistance> temp;
	temp = getPQ();

	while(!temp.empty()){
		cout << temp.top().distance << " ";
		cout << temp.top().city1 << " ";
		cout << temp.top().city2 << endl;
		temp.pop();
	}

}
//Displays the cities you have in the vector of cities. I used this for bug fixing
void Country::displayVectorOfCities(){

	for(int i = 0; i < citiesInTheCountry.size(); i++){
		cout << citiesInTheCountry[i].cityNumber << endl;
	}
}

//This function will read the input file and make changes to the Country variable that is passed in.
void Country::readInputFile(string file){
	
	//Open the file.
	ifstream readFile;
	readFile.open(file);

	if(!readFile.is_open()){
		cout << "File failed to open." << endl;
	}

	//The containers for the lines, country name, number of cities, city number and the Euclidean Coordinates.
	string line1, line2, line3, line4, line5, line6, line7, nameOfCountry, eof;
	int numberOfCitiesInTheCountry, cityNumber;
	float xPosition, yPosition;

	//Read line 1
	getline(readFile, line1);
	
	//Read line 2 and get the name of the country from line 2.
	getline(readFile, line2);
	setCountryName(line2);

	//Read lines 3 and 4.
	getline(readFile, line3);
	getline(readFile, line4);

	//Read line 5 and get the number of cities in the country from line 5.
	getline(readFile, line5);
	setNumberOfCitiesInCountry(line5);

	//Read lines 6 and 7.
	getline(readFile, line6);
	getline(readFile, line7);

	//While the program is reading the city number and the Euclidean coordinates:
	while(readFile >> cityNumber >> xPosition >> yPosition){
		
		//Create a Cities object.
		Cities currentCity;
		
		//Set the data members in the Cities object to their respective data memebers in the Cities struct.
		currentCity.cityNumber = cityNumber; 
		currentCity.xCoordinate = xPosition;
		currentCity.yCoordinate = yPosition;

		//cout << cityNumber << endl;
		//Push back the Cities object into the vector of Cities inside the Country class.
		setCityInCitiesVector(currentCity);
	}

	//Read the "EOF" line of the file.
	readFile >> eof;

	//Create the adjacency matrix.
	createAdjacencyMatrix();
	
	//Close the file once done using it.
	readFile.close();
}

#endif //COUNTRY_CLASS_CPP
