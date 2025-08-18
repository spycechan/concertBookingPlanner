#include <iostream>
#include "VenueManagement.h"
using namespace std;
void VenueManagementMenu();
vector<Venue> venues;
void main() {
	
	VenueManagementMenu();
	
}

void VenueManagementMenu()
{
	//Venue Management Menu
	int options = 0;
	cout << "1.Create Venue" << endl;
	cout << "2.List Venue" << endl;
	cout << "3.Update Venue details" << endl;
	cout << "4.Remove Venue" << endl;
	cout << "Please enter desire action (0-4) :";
	while (!(cin >> options)) {
		cout << "Invalid Input . Please enter an integer within (0-4)! :";
		cin.clear();
		cin.ignore(10000, '\n');
	}

	switch (options)
	{
	case 0:
		//return
		break;
	case 1:
		createVenue(venues);
		break;
	case 2:
		listVenue(venues);
		break;
	case 3:
		updateVenue();
		break;
	case 4:
		venueDeletion();
		break;
	default:
		cout << "Please input action within 0-4";
	}
}