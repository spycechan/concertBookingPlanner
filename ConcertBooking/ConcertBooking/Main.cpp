#include "VenueManagement.h"
#include "EventManagement.h"
#include "OrganizerManagement.h"
#include "TicketManagement.h"
#include "Report.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <regex>
#include <cctype>
using namespace std;

//Menu and functions declaration
void adminMenu(vector<Venue>& venues, vector<Event>& events, const string& filename, vector<Ticket>& tickets);
void eventManagementMenu(vector<Event>& events, const vector<Venue>& venues);
void VenueManagementMenu(vector<Venue>& venues, const string& filename);
void showReport(vector<Event>& events, vector<Venue>& venues, vector<Ticket>& tickets);
void organizerMenu(vector<Event>& events, const vector<Venue>& venues);
void customerMenu(const vector<Event>& events, const vector<Venue>& venues, vector<Ticket>& tickets);
int getValidatedChoice(int min, int max);

// Function to get validated integer input within a specified range
int getValidatedChoice(int min, int max) {
    int choice;
    string input;
    bool valid;

    while (true) {
        cout << "Enter your choice (" << min << "-" << max << "): ";
        cin >> input;

        valid = true;

        // Check if all characters are digits
        for (int i = 0; i < input.length(); i++) {
            if (!isdigit(input[i])) {
                valid = false;
                break;
            }
        }

        if (valid) {
            // Convert string to integer
            choice = 0;
            for (int i = 0; i < input.length(); i++) {
                choice = choice * 10 + (input[i] - '0');
            }

            // Check if within range
            if (choice >= min && choice <= max) {
                return choice;
            }
        }

        // If input is invalid or out of range
        cout << "Invalid input. Please enter an integer between " << min << " and " << max << ".\n";
    }
}

int main() {
    //vector declaration 
    vector<Venue> venues;
    vector<Event> events;
    vector<Ticket>tickets;

    //venue text file declaration
    string filename = "venues.txt";

    //textfile loader
    loadVenuesFromFile(venues, filename);
    loadEvents(events);
    loadTickets(tickets, "tickets.txt");

    //Main Menu
    int roleChoice = -1;
    do {
        cout << "\n==============================================\n";
        cout << "|         Welcome to the System!             |\n";
        cout << "==============================================\n";
        cout << "| 1. Admin                                   |\n";
        cout << "| 2. Organizer                               |\n";
        cout << "| 3. Customer                                |\n";
        cout << "| 0. Exit                                    |\n";
        cout << "==============================================\n";
        roleChoice = getValidatedChoice(0, 3);
        cout << endl;
        switch (roleChoice) {
        case 1:
            adminMenu(venues, events, filename, tickets);
            break;
        case 2:
            organizerMenu(events, venues);
            break;

        case 3:
            customerMenu(events, venues, tickets);
            break;

        case 0:
            cout << "\n==============================================\n";
            cout << "|      Thank you for using the system!       |\n";
            cout << "==============================================\n" << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (roleChoice != 0);
    return 0;
}

void adminMenu(vector<Venue>& venues, vector<Event>& events, const string& filename, vector<Ticket>& tickets) {
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
        mainChoice = getValidatedChoice(0, 3);
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
            showReport(events, venues, tickets);
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
        eventChoice = getValidatedChoice(0, 4);
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
        options = getValidatedChoice(0, 4);

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

void showReport(vector<Event>& events, vector<Venue>& venues, vector<Ticket>& tickets) {
    int choice = -1;
    string input;

    do {
        cout << "\n==============================================\n";
        cout << "|              Revenue Report Menu           |\n";
        cout << "==============================================\n";
        cout << "| 1. Summary Report (Revenue by Venue)       |\n";
        cout << "| 2. Detailed Report (All Events)            |\n";
        cout << "| 3. Top Events by Venue                     |\n";
        cout << "| 4. Top 5 Organizers by Revenue             |\n";
        cout << "| 5. Delete Report                           |\n";
        cout << "| 0. Back                                    |\n";
        cout << "==============================================\n";

        // input validation
        while (true) {
            cout << "Enter your choice (0-5): ";
            cin >> input;

            bool isNumber = true;
            for (char c : input) {
                if (!isdigit(c)) {
                    isNumber = false;
                    break;
                }
            }

            if (isNumber) {
                choice = stoi(input);
                if (choice >= 0 && choice <= 5) {
                    break;
                }
            }

            cout << "Invalid input. Please enter an integer (0-5).\n";
        }

        switch (choice) {
        case 0:
            cout << "Returning to Main Menu...\n";
            break;

        case 1:
            summaryReport(events, tickets,venues);
            saveReport(events, venues, tickets);
            break;

        case 2:
            detailedReport(events, tickets,venues);
            saveReport(events, venues, tickets);
            break;

        case 3: {
            cin.ignore();
            TopEvents(events, tickets);
            saveReport(events, venues, tickets);
            break;
        }

        case 4: {
            Top5Organizers(events, tickets);
            saveReport(events, venues, tickets);
            break;
        }

        case 5:
            removeReport("Report.txt");
            break;

        default:
            cout << "Invalid choice. Please choose 0-5.\n";
        }

    } while (choice != 0);
}



void organizerMenu(vector<Event>& events, const vector<Venue>& venues) {
    int choice = -1;
    do {
        cout << "\n==============================================\n";
        cout << "|           Organizer Menu                   |\n";
        cout << "==============================================\n";
        cout << "| 1. Schedule Event                          |\n";
        cout << "| 2. View My Events                          |\n";
        cout << "| 3. Make Payment                            |\n";
        cout << "| 4. Refund Payment                          |\n";
        cout << "| 0. Go Back                                 |\n";
        cout << "==============================================\n";
        choice = getValidatedChoice(0, 4);
        cout << endl;
        switch (choice) {
        case 1:
            addEvent(events, venues);
            saveEvents(events);
            break;
        case 2:
            viewEventsByOrganizer(events, venues);
            break;
        case 3:
            organizerPayment(events, venues);
            break;
        case 4:
            refundPaymentEvent(events, venues);
            saveEvents(events);
            break;
        case 0:
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);
}

void customerMenu(const vector<Event>& events, const vector<Venue>& venues, vector<Ticket>& tickets) {
    int choice = -1;
    do {
        cout << "\n==============================================\n";
        cout << "|           Customer Menu                    |\n";
        cout << "==============================================\n";
        cout << "| 1. Purchase Ticket                         |\n";
        cout << "| 2. View My Tickets                         |\n";
        cout << "| 3. Refund Ticket                           |\n";
        cout << "| 0. Go Back                                 |\n";
        cout << "==============================================\n";
        choice = getValidatedChoice(0, 3);
        switch (choice) {
        case 1:
            purchaseTicket(events, venues, tickets);
            break;
        case 2:
            viewTickets(tickets);
            break;
        case 3:
            refundTicket(tickets);
            break;
        case 0:
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);
}

