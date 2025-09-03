#pragma once
#ifndef REPORT_H
#define REPORT_H
#include "VenueManagement.h"
#include "EventManagement.h"
#include "TicketManagement.h"

#include <vector>
#include <string>
using namespace std;

void summaryReport(const vector<Event>& events, const vector<Ticket>& tickets);
void detailedReport(const vector<Event>& events, const vector<Ticket>& tickets);
void Top5(const vector<Event>& events, const vector<Ticket>& tickets);
void Top5Organizers(const vector<Event>& events, const vector<Ticket>& tickets);
void saveReport(const vector<Event>& events, const vector<Venue>& venues, const vector<Ticket>& tickets);
void loadTickets(vector<Ticket>& tickets, const string& filename);
void removeReport(const string& filename);

#endif