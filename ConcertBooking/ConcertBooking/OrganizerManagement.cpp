#include<iostream>
#include<vector>
#include<iomanip>
#include "EventManagement.h"

using namespace std;

//lets organizer see what they have scheduled by inputting their own organizer name
//somewhat safe as u dont easily guess other's organizer's name
void viewEventsByOrganizer(const vector<Event>& events) {
    string organizerName;
    cout << "Enter your organizer name: ";
    cin.ignore();
    getline(cin, organizerName);
    cout << "=============================================================================================" << endl;
    cout << "|                               Your Scheduled Events                                       |" << endl;
    cout << "=============================================================================================" << endl;
    cout << "| " << left << setw(20) << "Event Name"
        << setw(12) << "Date"
        << setw(8) << "Start"
        << setw(8) << "End"
        << setw(15) << "Hall" << " |" << endl;
    cout << "---------------------------------------------------------------------------------------------" << endl;
    bool found = false;
    for (int i = 0; i < events.size(); i++) {
        if (events[i].organizer == organizerName) {
            found = true;
            cout << "| " << left << setw(20) << events[i].name
                << setw(12) << events[i].date
                << setw(8) << events[i].startTime
                << setw(8) << events[i].endTime
                << setw(15) << events[i].hallName << " |" << endl;
        }
    }
    if (!found) {
        cout << "| No events found for organizer: " << organizerName << setw(60) << " " << "|" << endl;
    }
    cout << "=============================================================================================" << endl;
}