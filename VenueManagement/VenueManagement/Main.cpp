#include "VenueManagement.h"
#include "EventManagement.h"
#include "OrganizerManagement.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <regex>

using namespace std;
//Menu and functions declaration
void adminMenu(vector<Venue>& venues, vector<Event>& events, const string& filename);
void eventManagementMenu(vector<Event>& events, const vector<Venue>& venues);
void VenueManagementMenu(vector<Venue>& venues, const string& filename);
void showReport();
void organizerMenu(vector<Event>& events, const vector<Venue>& venues);


int main() {
    //vector declaration 
    vector<Venue> venues;
    vector<Event> events;

    //venue text file declaration
    string filename = "venues.txt";

    //textfile loader
    loadVenuesFromFile(venues, filename);
    loadEvents(events);

    //Main Menu
    int roleChoice = -1;
    do {
        cout << "\n==============================================\n";
        cout << "|         Welcome to the System!             |\n";
        cout << "==============================================\n";
        cout << "| 1. Admin                                  |\n";
        cout << "| 2. Organizer                              |\n";
        cout << "| 3. Customer                               |\n";
        cout << "| 0. Exit                                   |\n";
        cout << "==============================================\n";
        cout << "Enter your role: ";
        cin >> roleChoice;
        cout << endl;
        switch (roleChoice) {
        case 1:
            adminMenu(venues, events, filename);
            break;
        case 2:
            organizerMenu(events, venues);
            break;

        case 3:

            break;
        case 0:
            cout << "\n==============================================\n";
            cout << "|      Thank you for using the system!       |\n";
            cout << "==============================================\n" << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (roleChoice != 4);
    return 0;
}

void adminMenu(vector<Venue>& venues, vector<Event>& events, const string& filename) {
    int mainChoice = -1;
    do {
        cout << "\n==============================================\n";
        cout << "|         Concert Management System          |\n";
        cout << "==============================================\n";
        cout << "| 1. Venue Management                       |\n";
        cout << "| 2. Event Management                       |\n";
        cout << "| 3. Report                                 |\n";
        cout << "| 0. Exit                                   |\n";
        cout << "==============================================\n";
        cout << "Enter choice: ";
        cin >> mainChoice;
        cout << endl;
        switch (mainChoice) {
        case 1:
            VenueManagementMenu(venues, filename);
            break;
        case 2:
            eventManagementMenu(events, venues);
            saveEvents(events);
            break;
        case 3:
            showReport();
            break;
        case 0:
            cout << "\n==============================================\n";
            cout << "|      Thank you for using the system!       |\n";
            cout << "==============================================\n" << endl;
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (mainChoice != 0);
}

void eventManagementMenu(vector<Event>& events, const vector<Venue>& venues) {
    int eventChoice = -1;
    do {
        cout << "\n==============================================\n";
        cout << "|           Event Management Menu            |\n";
        cout << "==============================================\n";
        cout << "| 1. Add Event                              |\n";
        cout << "| 2. View Events                            |\n";
        cout << "| 3. Update Event                           |\n";
        cout << "| 4. Remove Event                           |\n";
        cout << "| 0. Go Back                                |\n";
        cout << "==============================================\n";
        cout << "Enter choice: ";
        cin >> eventChoice;
        cout << endl;
        switch (eventChoice) {
        case 1:
            addEvent(events, venues);
            saveEvents(events);
            break;
        case 2:
            viewEvents(events, venues);
            break;
        case 3: updateEvent(events, venues); saveEvents(events); break;
        case 4:
            removeEvent(events);
            saveEvents(events);
            break;
        case 0: break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (eventChoice != 0);
}

void VenueManagementMenu(vector<Venue>& venues, const string& filename)
{
    //Venue Management Menu
    int options = -1;
    do {
        cout << "==============================================\n";
        cout << "|     Welcome to Concert Venue Management!    |\n";
        cout << "==============================================\n";
        cout << "|     Manage your Venue and availability      |\n";
        cout << "==============================================\n" << endl;
        cout << "\n==============================================\n";
        cout << "|          Venue Management Menu              |\n";
        cout << "==============================================\n";
        cout << "| 1. Add Venue                              |\n";
        cout << "| 2. View Venue                             |\n";
        cout << "| 3. Update Venue                           |\n";
        cout << "| 4. Remove Venue                           |\n";
        cout << "| 0. Go Back                                |\n";
        cout << "==============================================\n";
        cout << "Please enter desire action (0-5) :";
        while (!(cin >> options)) {
            cout << "Invalid Input . Please enter an integer within (0-4)! :";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        switch (options)
        {
        case 0:
            cout << "Going back .....";
            break;
        case 1:
            createVenue(venues, filename);
            break;
        case 2:
            listVenue(venues);
            break;
        case 3:
            updateVenue(venues);
            break;
        case 4:
            venueDeletion(venues, filename);
            break;
        default:
            cout << "Please input action within 0-4";
        }
    } while (options != 0);
}

//if report has too many functions , its better to open new file , consult even
void showReport() {
    cout << "\n==============================================\n";
    cout << "|              Report Section                |\n";
    cout << "==============================================\n" << endl;
    cout << "(Report functionality to be implemented)\n";
}


void organizerMenu(vector<Event>& events, const vector<Venue>& venues) {
    int choice = -1;
    do {
        cout << "\n==============================================\n";
        cout << "|           Organizer Menu                   |\n";
        cout << "==============================================\n";
        cout << "| 1. Schedule Event                         |\n";
        cout << "| 2. View My Events                         |\n";
        cout << "| 3. Make Payment                           |\n";
        cout << "| 0. Go Back                                |\n";
        cout << "==============================================\n";
        cout << "Enter choice: ";
        cin >> choice;
        cout << endl;
        switch (choice) {
        case 1:
            addEvent(events, venues);
            saveEvents(events);
            break;
        case 2: {
            string organizerName;
            cout << "Enter your organizer name: ";
            cin.ignore();
            getline(cin, organizerName);
            for (size_t k = 0; k < organizerName.size(); ++k) {
                organizerName[k] = toupper(organizerName[k]);
            }
            viewEventsByOrganizer(events, venues, organizerName);
            break;
        }
        case 3:
            // Make Payment (empty for now)
            cout << "Payment feature coming soon!\n";
            break;
        case 0:
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);
}