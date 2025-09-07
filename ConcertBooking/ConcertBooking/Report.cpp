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

void summaryReport(const vector<Event>& events, const vector<Ticket>& tickets, const vector<Venue>& venues) {
    string venueData[100][4]; // [0] = hallID [1] = hallName, [2] = revenue, [3] = event count
    int Count = 0;

    cout << "\n===============================================================================================================" << endl;
    cout << "|                                   Summary Report: Revenue by Venue                                          |" << endl;
    cout << "===============================================================================================================" << endl;
    cout << "| " << left << setw(10) << "Hall ID"
         << "| " << left << setw(40) << "Venue"
         << "| " << setw(30) << "Total Revenue (RM)"
         << "| " << setw(22) << "Total Events" << "|" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------" << endl;

    // Sum revenue per venue
    for (size_t i = 0; i < events.size(); i++) {
        int sold = 0;
        for (size_t j = 0; j < tickets.size(); j++) {
            if (tickets[j].eventName == events[i].name) {
                sold++;
            }
        }
        string hallID = "";
        for (const auto& v : venues) {
            if (v.venueName == events[i].hallName) {
                hallID = v.venueID;
                break;
            }
        }

        double revenue = events[i].ticketPrice * sold;
        bool found = false;

        // check if venue aldy exists in array
        for (int k = 0; k < Count; k++) {
            if (venueData[k][1] == events[i].hallName) {
                double current = stod(venueData[k][2]);
                venueData[k][2] = to_string(current + revenue);
                int count = stoi(venueData[k][3]);
                venueData[k][3] = to_string(count + 1);
                found = true;
                break;
            }
        }

        // if new venue, add to array
        if (!found) {
            venueData[Count][0] = hallID;
            venueData[Count][1] = events[i].hallName;
            venueData[Count][2] = to_string(revenue);
            venueData[Count][3] = "1"; // first event at this venue
            Count++;
        }
    }

    double total = 0.0;
    int totalEvents = 0;
    for (int i = 0; i < Count; i++) {
        double revenue = stod(venueData[i][2]); // convert back for printing and total
        int count = stoi(venueData[i][3]);
        cout 
            << "| " << left << setw(10) << venueData[i][0]
            << "| " << left << setw(40) << venueData[i][1]
            << "| " << setw(30) << fixed << setprecision(2) << revenue
            << "| " << setw(21) << count << " |" << endl;
        total += revenue;
        totalEvents += count;
    }

    cout << "---------------------------------------------------------------------------------------------------------------" << endl;
    cout << "| " << setw(52) << left << "TOTAL"
        << "| " << setw(30) << fixed << setprecision(2) << total
        << "| " << setw(21) << totalEvents << " |" << endl;
    cout << "===============================================================================================================" << endl;
}


void detailedReport(const vector<Event>& events, const vector<Ticket>& tickets, const vector<Venue>& venues) {
    cout << "===========================================================================================================================" << endl;
    cout << "|                                              Detailed Report                                                            |" << endl;
    cout << "===========================================================================================================================" << endl;
    cout << "| " << left << setw(20) << "Event Name"
        << setw(15) << "Organizer"
        << setw(12) << "Date"
        << setw(8) << "Start"
        << setw(8) << "End"
        << setw(7) << "HallID"
        << setw(10) << "Hall"
        << setw(15) << "Price/Ticket"
        << setw(13) << "Tickets Sold"
        << setw(12) << "Revenue" << "|" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;

    double total = 0.0;
    string hallID ="";
    for (size_t i = 0; i < events.size(); i++) {
        int sold = 0;
        for (size_t j = 0; j < tickets.size(); j++) {
            if (tickets[j].eventName == events[i].name) {
                sold++;
            }
        }

        // Find the venueID from venues vector
        for (const auto& v : venues) {
            if (v.venueName == events[i].hallName) {
                hallID = v.venueID;
                break;
            }
        }
        string hallDisplay = events[i].hallName;
        if (hallDisplay.length() > 9) {
            hallDisplay = hallDisplay.substr(0, 7) + "..";
        }
        
        double revenue = events[i].ticketPrice * sold;
        total += revenue;

        cout << "| " << left << setw(20) << events[i].name
            << setw(15) << events[i].organizer
            << setw(12) << events[i].date
            << setw(8) << events[i].startTime
            << setw(8) << events[i].endTime
            << setw(7) << hallID
            << setw(10) << left << hallDisplay
            << setw(15) << fixed << setprecision(2) << events[i].ticketPrice
            << setw(13) << sold
            << setw(12) << fixed << setprecision(2) << revenue << "|" << endl;
    }

    cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "| " << setw(108) << left << "TOTAL REVENUE:"
        << "RM " << fixed << setprecision(2) << total << " |" << endl;
    cout << "===========================================================================================================================" << endl;
}


void TopEvents(const vector<Event>& events, const vector<Ticket>& tickets) {
    set<string> uniqueLocations;
    for (size_t i = 0; i < events.size(); i++) {
        uniqueLocations.insert(events[i].hallName);
    }

    // Show location
    cout << "\nAvailable Locations:\n";
    for (const string& loc : uniqueLocations) {
        cout << " - " << loc << endl;
    }

    // User input 
    string location;
    cout << "\nEnter location: ";
    getline(cin, location);

    // Change to lowercase
    string inputLower = location;
    transform(inputLower.begin(), inputLower.end(), inputLower.begin(), ::tolower);

    vector<string> names;
    vector<double> revenues;
    vector<int> ticketsSold;
    vector<double> ticketPrices;
    string matchedVenue = "";  // save venue name

    double totalVenueRevenue = 0.0;

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
            ticketsSold.push_back(sold);
            ticketPrices.push_back(events[i].ticketPrice);
            totalVenueRevenue += revenue;
        }
    }

    // Validation
    if (names.empty()) {
        cout << "\nNo venue found starting with \"" << location << "\". Please try again.\n";
        return;
    }

    int topX;
    cout << "Enter number of top events to display: ";
    cin >> topX;
    cin.ignore();

    cout << "\n==============================================================================================================================" << endl;
    cout << "|                                           Top " << topX << " Events in " << setw(40) << left << matchedVenue << " |" << endl;
    cout << "==============================================================================================================================" << endl;
    cout << "| " << setw(5) << left << "Rank"
        << "| " << setw(40) << "Event Name"
        << "| " << setw(15) << "Ticket Price"
        << "| " << setw(15) << "Tickets Sold"
        << "| " << setw(20) << "Revenue (RM)"
        << "| " << setw(15) << "% of revenue" << "|" << endl;
    cout << "------------------------------------------------------------------------------------------------------------------------------" << endl;

    // Sort by revenue (descending)
    for (size_t i = 0; i < revenues.size(); i++) {
        for (size_t j = i + 1; j < revenues.size(); j++) {
            if (revenues[i] < revenues[j]) {
                swap(revenues[i], revenues[j]);
                swap(names[i], names[j]);
                swap(ticketsSold[i], ticketsSold[j]);
                swap(ticketPrices[i], ticketPrices[j]);
            }
        }
    }

    int limit = (revenues.size() < (size_t)topX) ? revenues.size() : topX;
    for (int i = 0; i < limit; i++) {
        double percentage = (totalVenueRevenue > 0) ? (revenues[i] / totalVenueRevenue) * 100.0 : 0.0;
        cout << "| " << setw(5) << i + 1
            << "| " << setw(40) << names[i]
            << "| " << setw(15) << fixed << setprecision(2) << ticketPrices[i]
            << "| " << setw(15) << ticketsSold[i]
            << "| " << setw(20) << fixed << setprecision(2) << revenues[i]
            << "| " << setw(15) << fixed << setprecision(2) << percentage << "%" << "|" << endl;
    }

    cout << "==============================================================================================================================" << endl;
    cout << "Total Revenue for " << matchedVenue << ": RM " << fixed << setprecision(2) << totalVenueRevenue << endl;
    cout << "==============================================================================================================================" << endl;
}



void Top5Organizers(const vector<Event>& events, const vector<Ticket>& tickets) {
    cout << "\n=================================================================================================" << endl;
    cout << "|                                Top 5 Organizers by Revenue                                   |" << endl;
    cout << "=================================================================================================" << endl;
    cout << "| " << setw(5) << left << "Rank"
        << "| " << setw(40) << left << "Organizer"
        << "| " << setw(15) << right << "Revenue (RM)"
        << "| " << setw(10) << right << "Events" << " |" << endl;
    cout << "-------------------------------------------------------------------------------------------------" << endl;

    vector<string> organizers;
    vector<double> revenues;
    vector<int> event;  //number of events organized

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
                event[k]++;   
                found = true;
                break;
            }
        }
        if (!found) {
            organizers.push_back(events[i].organizer);
            revenues.push_back(revenue);
            event.push_back(1);  //first event for this organizer
        }
    }

    // Sort by revenue (descending)
    for (size_t i = 0; i < revenues.size(); i++) {
        for (size_t j = i + 1; j < revenues.size(); j++) {
            if (revenues[i] < revenues[j]) {
                swap(revenues[i], revenues[j]);
                swap(organizers[i], organizers[j]);
                swap(event[i], event[j]);
            }
        }
    }

    int limit = (revenues.size() < 5) ? revenues.size() : 5;
    for (int i = 0; i < limit; i++) {
        cout << "| " << setw(5) << left << i + 1
            << "| " << setw(40) << left << organizers[i]
            << "| " << setw(15) << right << fixed << setprecision(2) << revenues[i]
            << "| " << setw(10) << right << event[i] << " |" << endl;
    }

    cout << "=================================================================================================" << endl;
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
