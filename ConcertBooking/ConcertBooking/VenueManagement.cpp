#include "VenueManagement.h"
#include "EventManagement.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

void createVenue(vector<Venue>& venues, const string& venueFile)
{
	Venue V;
	//declare a temporary variable to store the struct 

	//ask for venue creation information
	cout << "=== Venue Creation ===" << endl;
	cout << "Please Enter the name of the Venue: ";
	cin.ignore();
	getline(cin, V.venueName);
	cout << "Please Enter the address of the Venue: ";
	getline(cin, V.venueAddress);
	cout << "Please Enter the capacity of the venue: ";
	cin >> V.venueCapacity;
	cout << "Please Enter the Rental Cost of the Venue Per Session(half day): ";
	cin >> V.rentalCost;
	V.availability = true;


	//check if the name and address has conflict
	if (isVenueConflict(venues, V.venueName)) {
		cout << "Error: Venue with the same name and address already exists.\n";
		return;
	}

	//add into the vector first then save into text file
	venues.push_back(V);
	saveVenues(venues, venueFile);
}
void listVenue(const vector<Venue>& venues)
{
	const int WIDTH = 115; // i set a border so i dont do "=" x75
	cout << "\n" << string(WIDTH, '=') << "\n";
	cout << "| " << setw(WIDTH - 4) << left << "Venue List" << " |\n";
	cout << string(WIDTH, '=') << "\n";

	//display empty if no venue
	if (venues.empty()) {
		cout << "| " << setw(WIDTH - 4) << left << "No venues available." << "|\n";
		cout << string(WIDTH, '=') << "\n";
		return;
	}
	//display venues
	int index = 1;
	for (const auto& i : venues) {
		cout << string(WIDTH, '-') << "\n";
		cout << "| [" << index++ << "] " << setw(WIDTH - 6) << left << i.venueName << "|\n";
		cout << string(WIDTH, '-') << "\n";
		cout << "| Address       : " << setw(WIDTH - 20) << left << i.venueAddress << " |\n";
		cout << "| Capacity      : " << setw(WIDTH - 20) << left << i.venueCapacity << " |\n";
		cout << "| Rental Cost/hr: $" << setw(WIDTH - 22) << fixed << setprecision(2) << i.rentalCost << "  |\n";
		cout << "| Availability  : " << setw(WIDTH - 20) << left
			<< (i.availability ? "Available" : "Not Available") << " |\n";
		cout << string(WIDTH, '=') << "\n";
	}
}

void updateVenue(vector<Venue>& venues) {
	//display empty if no venue found
	if (venues.empty()) {
		cout << "No venues available to update.\n";
		return;
	}

	// Show list of venues with index
	cout << "\nAvailable Venues:\n";
	for (int i = 0; i < venues.size(); i++) {
		cout << i + 1 << ". " << venues[i].venueName << "\n";
	}

	int index;
	cout << "Enter the index of the venue to update (1-" << venues.size() << "): ";
	cin >> index;

	// validate index 
	if (index < 1 || index > venues.size()) {
		cout << "Invalid index.\n";
		return;
	}

	// convert index back to 0 to select 
	int i = index - 1;

	int choice;
	cout << "==============================================\n";
	cout << "Venue Name          : " << venues[i].venueName << "\n";
	cout << "Venue Address       : " << venues[i].venueAddress << "\n";
	cout << "Venue Capacity      : " << venues[i].venueCapacity << "\n";
	cout << "Venue Rental Cost   : " << venues[i].rentalCost << "\n";
	cout << "Venue Availability  : " << (venues[i].availability ? "Available" : "Not Available") << "\n";
	cout << "==============================================\n";
	cout << "1. Update Name\n";
	cout << "2. Update Address\n";
	cout << "3. Update Capacity\n";
	cout << "4. Update Rental Cost\n";
	cout << "5. Update Availability\n";
	cout << "0. Go Back\n";
	cout << "Choose field to update (1-5): ";
	cin >> choice;
	cin.ignore(); // flush newline

	// can choose to update which field
	switch (choice) {
	case 0:
		break;
	case 1:
		cout << "Enter new Name: ";
		getline(cin, venues[i].venueName);
		break;
	case 2:
		cout << "Enter new Address: ";
		getline(cin, venues[i].venueAddress);
		break;
	case 3:
		cout << "Enter new Capacity: ";
		cin >> venues[i].venueCapacity;
		break;
	case 4:
		cout << "Enter new Rental Cost: ";
		cin >> venues[i].rentalCost;
		break;
	case 5: {
		int avail;
		cout << "Enter Availability (1 = Available, 0 = Not Available): ";
		cin >> avail;
		venues[i].availability = (avail == 1);
		break;
	}
	default:
		cout << "Invalid choice.\n";
		return;
	}

	//save updated info
	saveVenues(venues, "venues.txt");
	cout << "Venue updated successfully!\n";
}

void venueDeletion(vector<Venue>& venues, const string& venueFile)
{
	//display empty when no venue found
	if (venues.empty())
	{
		cout << "no venues available to delete.\n";
		return;
	}
	//loop for list venue
	cout << "=== Venue List ===\n";
	for (int i = 0; i < venues.size(); i++)
	{
		cout << i + 1 << ". " << venues[i].venueName << " (" << venues[i].venueAddress << ")\n";
	}
	//select and delete
	int index;
	cout << "Select the venue you want to delete: ";
	cin >> index;
	index -= 1;
	if (index < 0 || index >= venues.size())
	{
		cout << "Invalid options!\n";
		return;
	}
	cin.ignore();
	string confirmation;
	cout << "Please enter 'CONFIRM'to confirm deletion :";
	getline(cin, confirmation);

	// Load events and check if any use this venue
	vector<Event> events;
	loadEvents(events);
	bool venueInUse = false;
	for (size_t i = 0; i < events.size(); ++i) {
		const Event& event = events[i];
		if (event.hallName == venues[index].venueName) {
			venueInUse = true;
			break;
		}
	}
	if (venueInUse) {
		cout << "Cannot remove venue. There are events scheduled at this venue.\n";
		return;
	}

	if (confirmation == "CONFIRM")
	{
		venues.erase(venues.begin() + index);
		cout << "\nVenues Removed.\n";
		saveVenues(venues, venueFile);

	}
	else {
		cout << "Deletion Cancelled!\n";
	}
}

//text file loader 
void loadVenuesFromFile(vector<Venue>& venues, const string& venueFile)
{
	ifstream inFile(venueFile);
	Venue V;
	while (getline(inFile, V.venueName))
	{
		getline(inFile, V.venueAddress);
		inFile >> V.venueCapacity;
		inFile >> V.rentalCost;
		inFile >> V.availability;
		inFile.ignore();
		venues.push_back(V);

	}
}

//check conflicts
bool isVenueConflict(vector<Venue>& venues, const string& venueName) {
	for (int i = 0; i < venues.size(); i++) {
		if (venues[i].venueName == venueName) {
			return true; // found conflict
		}
	}
	return false;//proceed with whatever the func doing
}

//save venue into files
void saveVenues(vector<Venue>& venues, const string& venueFile)
{
	ofstream fout(venueFile);
	for (int i = 0; i < venues.size(); i++)
	{
		const Venue& V = venues[i];
		fout << V.venueName << endl
			<< V.venueAddress << endl
			<< V.venueCapacity << endl
			<< V.rentalCost << endl
			<< V.availability << endl;
	}
	fout.close();
}

