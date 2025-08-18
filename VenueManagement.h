#ifndef VENUE_MANAGEMENT_H
#define VENUE_MANAGEMENT_H
#include <vector>
#include <string>
using namespace std;
struct Venue
{
	string venueName;
	string venueAddress;
	int venueCapacity;
	double rentalCost;
	bool availability;
};

// DECLARATION only
void createVenue(vector<Venue>& venues);
void listVenue(vector<Venue>& venues);
void updateVenue();
void venueDeletion();

#endif
