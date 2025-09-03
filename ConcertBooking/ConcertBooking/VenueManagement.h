#pragma once

//include Guard , to prevent same header file from being included multiple times
#ifndef VENUE_MANAGEMENT_H
#define VENUE_MANAGEMENT_H
#include <vector>
#include <string>
using namespace std;

//struct declaration
struct Venue
{
	string venueName;
	string venueAddress;
	int venueCapacity;
	double rentalCost;
	bool availability;
};

// VenueManagement function declaration
void createVenue(vector<Venue>& venues, const string& venueFile);
void listVenue(const vector<Venue>& venues);
void updateVenue(vector<Venue>& venues);
void venueDeletion(vector<Venue>& venues, const string& venueFile);
void loadVenuesFromFile(vector<Venue>& venues, const string& venueFile);
bool isVenueConflict(vector<Venue>& venues, const string& venueName);
void saveVenues(vector<Venue>& venues, const string& venueFile);
#endif

