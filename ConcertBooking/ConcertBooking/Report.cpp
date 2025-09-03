#include<iostream>
#include<iomanip>
#include<vector>
#include<string>
#include<fstream>
#include<set>
#include<algorithm>
#include "VenueManagement.h"
#include "EventManagement.h"
#include "TicketManagement.h"

using namespace std;

void summaryReport(const vector<Event>& events, const vector<Ticket>& tickets) {
    string venueData[100][2];
    int venueCount = 0;

    cout << "\n===============================================================================================================" << endl;
    cout << "|                                   Summary Report: Revenue by Venue                                          |" << endl;
    cout << "===============================================================================================================" << endl;
    cout << "| " << left << setw(40) << "Venue"
        << "| " << setw(30) << "Total Revenue (RM)" << "|" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------" << endl;

    // Sum revenue per venue
    for (size_t i = 0; i < events.size(); i++) {
        int sold = 0;
        for (size_t j = 0; j < tickets.size(); j++) {
            if (tickets[j].eventName == events[i].name) {
                sold++;
            }
        }

        double revenue = events[i].ticketPrice * sold;
        bool found = false;

        // check if venue aldy exists in array
        for (int k = 0; k < venueCount; k++) {
            if (venueData[k][0] == events[i].hallName) {
                double current = stod(venueData[k][1]);
                venueData[k][1] = to_string(current + revenue);
                found = true;
                break;
            }
        }

        // if new venue, add to array
        if (!found) {
            venueData[venueCount][0] = events[i].hallName;
            venueData[venueCount][1] = to_string(revenue);
            venueCount++;
        }
    }

    double total = 0.0;
    for (int i = 0; i < venueCount; i++) {
        double revenue = stod(venueData[i][1]); // convert back for printing and total
        cout << "| " << left << setw(40) << venueData[i][0]
            << "| " << setw(30) << fixed << setprecision(2) << revenue << " |" << endl;
        total += revenue;
    }

    cout << "---------------------------------------------------------------------------------------------------------------" << endl;
    cout << "| " << setw(40) << left << "TOTAL"
        << "| " << setw(30) << fixed << setprecision(2) << total << " |" << endl;
    cout << "===============================================================================================================" << endl;
}

void detailedReport(const vector<Event>& events, const vector<Ticket>& tickets) {
    cout << "===============================================================================================================" << endl;
    cout << "|                                         Detailed Report                                                     |" << endl;
    cout << "===============================================================================================================" << endl;
    cout << "| " << left << setw(20) << "Event Name"
        << "| " << setw(20) << "Venue"
        << "| " << setw(20) << "Ticket Sold"
        << "| " << setw(20) << "Price"
        << "| " << setw(20) << "Revenue" << "|" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------" << endl;

    double total = 0.0;

    for (size_t i = 0; i < events.size(); i++) {
        int sold = 0;
        for (size_t j = 0; j < tickets.size(); j++) {
            if (tickets[j].eventName == events[i].name) {
                sold++;
            }
        }

        double revenue = events[i].ticketPrice * sold;
        total += revenue;

        cout << "| " << left << setw(20) << events[i].name
            << "| " << setw(20) << events[i].hallName
            << "| " << setw(20) << sold
            << "| " << setw(20) << fixed << setprecision(2) << events[i].ticketPrice
            << "| " << setw(20) << fixed << setprecision(2) << revenue << "|" << endl;
    }

    cout << "---------------------------------------------------------------------------------------------------------------" << endl;
    cout << "| " << setw(84) << left << "TOTAL REVENUE:"
        << "RM " << fixed << setprecision(2) << total << " |" << endl;
    cout << "===============================================================================================================" << endl;
}

void Top5(const vector<Event>& events, const vector<Ticket>& tickets) {
    set<string> uniqueLocations;
    for (size_t i = 0; i < events.size(); i++) {
        uniqueLocations.insert(events[i].hallName);
    }

    // Show location
    cout << "\nAvailable Locations:\n";
    for (const string& loc : uniqueLocations) {
        cout << " - " << loc << endl;
    }

    //user input
    string location;
    cout << "\nEnter location: ";
    getline(cin, location);

    // Chanege to lowercase
    string inputLower = location;
    transform(inputLower.begin(), inputLower.end(), inputLower.begin(), ::tolower);

    vector<string> names;
    vector<double> revenues;
    string matchedVenue = "";  //save venue name

    for (size_t i = 0; i < events.size(); i++) {
        string hallLower = events[i].hallName;
        transform(hallLower.begin(), hallLower.end(), hallLower.begin(), ::tolower);

        // Cross check if input match data
        if (hallLower.rfind(inputLower, 0) == 0) {
            if (matchedVenue.empty()) {
                matchedVenue = events[i].hallName;
            }

            int sold = 0;
            for (size_t j = 0; j < tickets.size(); j++) {
                if (tickets[j].eventName == events[i].name) {
                    sold++;
                }
            }

            double revenue = events[i].ticketPrice * sold;
            names.push_back(events[i].name);
            revenues.push_back(revenue);
        }
    }

    //Validation
    if (names.empty()) {
        cout << "\nNo venue found starting with \"" << location << "\". Please try again.\n";
        return;
    }

    cout << "\n===============================================================================================================" << endl;
    cout << "|                                   Top 5 Events in " << setw(45) << left << matchedVenue << " |" << endl;
    cout << "===============================================================================================================" << endl;
    cout << "| " << setw(5) << left << "Rank"
        << "| " << setw(40) << "Event Name"
        << "| " << setw(30) << "Revenue (RM)" << "|" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------" << endl;

    // Sort by revenue
    for (size_t i = 0; i < revenues.size(); i++) {
        for (size_t j = i + 1; j < revenues.size(); j++) {
            if (revenues[i] < revenues[j]) {
                swap(revenues[i], revenues[j]);
                swap(names[i], names[j]);
            }
        }
    }

    int limit = (revenues.size() < 5) ? revenues.size() : 5;
    for (int i = 0; i < limit; i++) {
        cout << "| " << setw(5) << i + 1
            << "| " << setw(40) << names[i]
            << "| " << setw(30) << fixed << setprecision(2) << revenues[i] << "|" << endl;
    }

    cout << "===============================================================================================================" << endl;
}

void Top5Organizers(const vector<Event>& events, const vector<Ticket>& tickets) {
    cout << "\n===============================================================================================================" << endl;
    cout << "|                                   Top 5 Organizers by Revenue                                               |" << endl;
    cout << "===============================================================================================================" << endl;
    cout << "| " << setw(5) << left << "Rank"
        << "| " << setw(40) << "Organizer"
        << "| " << setw(30) << "Revenue (RM)" << "|" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------" << endl;

    vector<string> organizers;
    vector<double> revenues;

    for (size_t i = 0; i < events.size(); i++) {
        int sold = 0;
        for (size_t j = 0; j < tickets.size(); j++) {
            if (tickets[j].eventName == events[i].name) {
                sold++;
            }
        }

        double revenue = events[i].ticketPrice * sold;
        bool found = false;
        for (size_t k = 0; k < organizers.size(); k++) {
            if (organizers[k] == events[i].organizer) {
                revenues[k] += revenue;
                found = true;
                break;
            }
        }
        if (!found) {
            organizers.push_back(events[i].organizer);
            revenues.push_back(revenue);
        }
    }

    // Sort
    for (size_t i = 0; i < revenues.size(); i++) {
        for (size_t j = i + 1; j < revenues.size(); j++) {
            if (revenues[i] < revenues[j]) {
                swap(revenues[i], revenues[j]);
                swap(organizers[i], organizers[j]);
            }
        }
    }

    int limit = (revenues.size() < 5) ? revenues.size() : 5;
    for (int i = 0; i < limit; i++) {
        cout << "| " << setw(5) << i + 1
            << "| " << setw(40) << organizers[i]
            << "| " << setw(30) << fixed << setprecision(2) << revenues[i] << "|" << endl;
    }

    cout << "===============================================================================================================" << endl;
}

void saveReport(const vector<Event>& events, const vector<Venue>& venues, const vector<Ticket>& tickets) {
    ofstream fout("Report.txt");//Open File writing
    if (!fout) {
        cerr << "Error opening file for writing!" << endl;
        return;
    }

    fout << "================= Event Report =================\n\n";

    for (size_t i = 0; i < events.size(); i++) {
        const Event& e = events[i];

        // Find matching venue
        double rentalCost = 0;
        for (size_t j = 0; j < venues.size(); j++) {
            if (venues[j].venueName == e.hallName) {
                rentalCost = venues[j].rentalCost;
                break;
            }
        }

        // Count sold tickets
        int ticketsSold = 0;
        for (size_t t = 0; t < tickets.size(); t++) {
            if (tickets[t].eventName == e.name) {
                ticketsSold++;
            }
        }
        double revenue = ticketsSold * e.ticketPrice;

        fout << "Event Name   : " << e.name << "\n";
        fout << "Organizer    : " << e.organizer << "\n";
        fout << "Date         : " << e.date << "\n";
        fout << "Time         : " << e.startTime << " - " << e.endTime << "\n";
        fout << "Hall         : " << e.hallName << "\n";
        fout << "Ticket Price : " << e.ticketPrice << "\n";
        fout << "Tickets Sold : " << ticketsSold << "\n";
        fout << "Revenue      : " << revenue << "\n";
        fout << "---------------------------------------------\n";
    }

    fout.close();
    cout << "Report saved to Report.txt" << endl;
}

void removeReport(const string& filename) {
    // Load file content first
    ifstream fin(filename);
    if (!fin) {
        cout << "No report file found.\n";
        return;
    }

    cout << "==========================================================================================\n";
    cout << "|                           Current Report Content                                        |\n";
    cout << "==========================================================================================\n";

    string line;
    bool empty = true;
    while (getline(fin, line)) {
        cout << line << endl;
        empty = false;
    }
    fin.close();

    if (empty) {
        cout << "Report file is already empty.\n";
        return;
    }

    cout << "==========================================================================================\n";
    string confirmation;
    cout << "\nPlease enter 'CONFIRM' to delete the content of the report: ";
    cin >> confirmation;

    if (confirmation == "CONFIRM") {
        ofstream fout(filename, ios::trunc); // overwrite 
        fout.close();
        cout << "Report cleared successfully.\n";
    }
    else {
        cout << "Confirmation not confirmed. Returning....\n";
    }
}
