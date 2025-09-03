#pragma once
#ifndef TICKET_MANAGEMENT_H
#define TICKET_MANAGEMENT_H

#include "EventManagement.h"
#include "VenueManagement.h"
#include <string>
#include <vector>
using namespace std;

// Ticket structure declarations
struct Ticket {
    string custName;
    string custPhone;
    string custIc;
    string ticketId;
    string eventName;
    string eventDate;
    string hallName;
    bool paid = false;
};

// Ticket Management function declarations
string generateTicketID();
void saveTickets(const vector<Ticket>& tickets, const string& filename = "tickets.txt");
void loadTickets(vector<Ticket>& tickets, const string& filename = "tickets.txt");
void purchaseTicket(const vector<Event>& events, const vector<Venue>& venues, vector<Ticket>& tickets);
void viewTickets(const vector<Ticket>& tickets);
int getVenueCapacity(const vector<Event>& events, const vector<Venue>& venues, const string& eventName);
int countTicketsSold(const vector<Ticket>& tickets, const string& eventName);

#endif
