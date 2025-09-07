#pragma once
#ifndef EVENT_MANAGEMENT_H
#define EVENT_MANAGEMENT_H
#include "VenueManagement.h"

#include <vector>
#include <string>
using namespace std;

//struct declarations
struct Event {
    string name;
    string date;
    string startTime;
    string endTime;
    string hallName;
    string organizer;
    double ticketPrice;
    bool paid = false;
};

//Event Management functions declarations
void loadEvents(vector<Event>& events);
void saveEvents(const vector<Event>& events);
void addEvent(vector<Event>& events, const vector<Venue>& venues);
void viewEvents(const vector<Event>& events, const vector<Venue>& venues);
void updateEvent(vector<Event>& events, const vector<Venue>& venues);
void removeEvent(vector<Event>& events);
bool isConflict(const vector<Event>& events, const string& date, const string& startTime, const string& endTime, const string& hallName);
bool isValidDate(const string& date);
void viewEventsByOrganizer(const vector<Event>& events, const vector<Venue>& venues);
void organizerPayment(vector<Event>& events, const vector<Venue>& venues);
void refundPaymentEvent(vector<Event>& events, const vector<Venue>& venues);


#endif
