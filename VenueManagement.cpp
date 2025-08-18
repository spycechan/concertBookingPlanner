#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "VenueManagement.h"
using namespace std;

//template to define the Venue information

//declare a dynamic number of venues

void createVenue(vector<Venue>& venues)
{
	Venue V; 
	//declare a temporary variable to store the struct 

	//ask for venue creation information
	cout << "=== Venue Creation ===" << endl;
	cout << "Please Enter the name of the Venue: ";
	cin.ignore();
	getline(cin, V.venueName);
	cout << "Please Enter the address of the Venue: ";
	cin.ignore();
	getline(cin, V.venueAddress);
	cout << "Please Enter the capacity of the venue: ";
	cin >> V.venueCapacity;
	cout << "Please Enter the Rental Cost of the Venue Per Hour ($/h): ";
	cin >> V.rentalCost;
	V.availability = true;

	venues.push_back(V);
	//take the V and push_back into the venues vector
	cout << "Venues added successfully!\n";

}
void listVenue(vector<Venue>& venues)
{
	for (auto i : venues)
	{
		cout << "Venue Name :" << i.venueName;
		cout << "Venue Address :" << i.venueAddress;
		cout << "Venue Capacity :" << i.venueCapacity;
		cout << "Rental Cost per Hour" << i.rentalCost;
		cout << "Availability :" << i.availability;
	}
}
void updateVenue()
{
	cout << "To be implement";
}
void venueDeletion()
{
	cout << "To be implement";
}