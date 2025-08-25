#include "VenueManagement.h"
#include "EventManagement.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <regex>

//text file loader
void loadEvents(vector<Event>& events) {
    ifstream fin("events.txt");
    Event e;
    while (fin >> std::ws && getline(fin, e.name, '\t') && getline(fin, e.organizer, '\t') && fin >> e.date >> e.startTime >> e.endTime ) {
        fin.ignore();
        getline(fin, e.hallName);
        events.push_back(e);
    }
    fin.close();
}
// save event schedules into text files
void saveEvents(const vector<Event>& events) {
    ofstream fout("events.txt");
    for (int i = 0; i < events.size(); i++) {
        const Event& e = events[i];
        fout << e.name << "\t" << e.organizer << "\t" << e.date << " " << e.startTime << " " << e.endTime << " " << e.hallName << endl;
    }
    fout.close();
}

// schedule creations
void addEvent(vector<Event>& events, const vector<Venue>& venues) {
    //temp var to store structure of <Event>
    Event e;
    cout << "Enter event name: ";
    cin.ignore();
    getline(cin, e.name);
    cout << "Enter organizer name: ";
    getline(cin, e.organizer);
    do {
        cout << "Enter date (YYYY-MM-DD): ";
        cin >> e.date;
        if (!isValidDate(e.date)) {
            cout << "Invalid date format. Please use YYYY-MM-DD.\n";
        }
    } while (!isValidDate(e.date));
    int slotChoice;
    cout << "Choose time slot: 1 for Day (09:00-17:00), 2 for Night (18:00-23:00): ";
    cin >> slotChoice;
    if (slotChoice == 1) {
        e.startTime = "09:00";
        e.endTime = "17:00";
    }
    else if (slotChoice == 2) {
        e.startTime = "18:00";
        e.endTime = "23:00";
    }
    else {
        cout << "Invalid slot. Event not added.\n";
        return;
    }

    // Venue selection
    if (venues.empty()) {
        cout << "No venues available. Please add a venue first.\n";
        return;
    }
    cout << "Select a venue for this event:" << endl;
    for (int i = 0; i < venues.size(); i++) {
        cout << i + 1 << ". " << venues[i].venueName << " (" << venues[i].venueAddress << ")" << endl;
    }
    int venueChoice;
    cout << "Enter venue number: ";
    cin >> venueChoice;
    if (venueChoice < 1 || venueChoice > venues.size()) {
        cout << "Invalid venue selection. Event not added.\n";
        return;
    }
    e.hallName = venues[venueChoice - 1].venueName;

    if (isConflict(events, e.date, e.startTime, e.endTime, e.hallName)) {
        cout << "Error: This slot is already booked!\n";
        return;
    }
    events.push_back(e);
    cout << "Event added successfully.\n";
}

//Event listing
void viewEvents(const vector<Event>& events) {
    cout << "==============================================================================================" << endl;
    cout << "|                                   Event List                                                |" << endl;
    cout << "==============================================================================================" << endl;
    cout << "| " << left << setw(20) << "Event Name"
        << setw(15) << "Organizer"
        << setw(12) << "Date"
        << setw(8) << "Start"
        << setw(8) << "End"
        << setw(15) << "Hall" << " |" << endl;
    cout << "----------------------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < events.size(); i++) {
        cout << "| " << left << setw(20) << events[i].name
            << setw(15) << events[i].organizer
            << setw(12) << events[i].date
            << setw(8) << events[i].startTime
            << setw(8) << events[i].endTime
            << setw(15) << events[i].hallName << " |" << endl;
    }
    cout << "==============================================================================================" << endl;
}
//event info update
void updateEvent(vector<Event>& events) {
    if (events.empty()) {
        cout << "No events to update.\n";
        return;
    }
    cout << "==============================================\n";
    cout << "|         Select the event to update         |\n";
    cout << "==============================================\n";
    cout << left << "| " << setw(3) << "No" << setw(20) << "Event Name" << setw(12) << "Date" << setw(8) << "Start" << setw(8) << "End" << setw(10) << "Hall" << "|" << endl;
    cout << "---------------------------------------------------------------" << endl;
    for (int i = 0; i < events.size(); i++) {
        cout << left << "| " << setw(3) << i + 1 << setw(20) << events[i].name << setw(12) << events[i].date << setw(8) << events[i].startTime << setw(8) << events[i].endTime << setw(10) << events[i].hallName << "|" << endl;
    }
    cout << "==============================================\n";
    int i;
    cout << "Enter event number: ";
    cin >> i;
    if (i < 1 || i > events.size()) {
        cout << "Invalid selection.\n";
        return;
    }
    i--;
    string newName;
    cout << "Enter new event name: ";
    cin.ignore();
    getline(cin, newName);
    events[i].name = newName;
    string newOrganizer;
    cout << "Enter new organizer name: ";
    getline(cin, newOrganizer);
    events[i].organizer = newOrganizer;
    string newDate;
    do {
        cout << "Enter new date (YYYY-MM-DD): ";
        cin >> newDate;
        if (!isValidDate(newDate)) {
            cout << "Invalid date format. Please use YYYY-MM-DD.\n";
        }
    } while (!isValidDate(newDate));
    events[i].date = newDate;
    int slotChoice;
    cout << "Choose new time slot: 1 for Day (09:00-17:00), 2 for Night (18:00-23:00): ";
    cin >> slotChoice;
    if (slotChoice == 1) {
        events[i].startTime = "09:00";
        events[i].endTime = "17:00";
    }
    else if (slotChoice == 2) {
        events[i].startTime = "18:00";
        events[i].endTime = "23:00";
    }
    else {
        cout << "Invalid slot. Event not updated.\n";
        return;
    }
    cout << "Event updated.\n";
}
//remove update
void removeEvent(vector<Event>& events) {
    //display empty if not found
    if (events.empty()) {
        cout << "No events to remove.\n";
        return;
    }
    cout << "==============================================\n";
    cout << "|         Select the event to remove         |\n";
    cout << "==============================================\n";
    cout << left << "| " << setw(3) << "No" << setw(20) << "Event Name" << setw(12) << "Date" << setw(8) << "Start" << setw(8) << "End" << setw(10) << "Hall" << "|" << endl;
    cout << "---------------------------------------------------------------" << endl;
    for (int i = 0; i < events.size(); i++) {
        cout << left << "| " << setw(3) << i + 1 << setw(20) << events[i].name << setw(12) << events[i].date << setw(8) << events[i].startTime << setw(8) << events[i].endTime << setw(10) << events[i].hallName << "|" << endl;
    }
    cout << "==============================================\n";
    int i;
    cout << "Enter event number: ";
    cin >> i;
    if (i < 1 || i > events.size()) {
        cout << "Invalid selection.\n";
        return;
    }
    //delete confirmation
    string confirmation = "";
    cout << "Please enter 'CONFIRM' for schedule remove confirmation :";
    cin >> confirmation;
    if (confirmation == "CONFIRM")
    {
        events.erase(events.begin() + (i - 1));
        cout << "Event removed.\n";
    }
    else
    {
        cout<<"Confirmation not confirmed. Returning ....";
    }
}

//check for date time hall conflict
bool isConflict(const vector<Event>& events, const string& date, const string& startTime, const string& endTime, const string& hallName) {
    for (int i = 0; i < events.size(); i++) {
        if (events[i].date == date && events[i].hallName == hallName) {
            return true;
        }
    }
    return false;
}

//regex date for schedule creation
bool isValidDate(const string& date) {
    regex datePattern(R"(^\d{4}-\d{2}-\d{2}$)");
    return regex_match(date, datePattern);
}