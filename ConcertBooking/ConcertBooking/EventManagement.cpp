// Check if a venue is available

#include "VenueManagement.h"
#include "EventManagement.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <regex>

//text file loader
bool isVenueAvailable(const Venue& venue) {
    return venue.availability;
}

void loadEvents(vector<Event>& events) {
    ifstream fin("events.txt");
    Event e;
    while (fin >> std::ws && getline(fin, e.name, '\t') && getline(fin, e.organizer, '\t') && fin >> e.date >> e.startTime >> e.endTime >> e.ticketPrice) {
        fin >> e.paid; // Read payment status (Updated)
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
        fout << e.name << "\t" << e.organizer << "\t" << e.date << " " << e.startTime << " " << e.endTime << " " << e.ticketPrice << " " << e.paid << "\n" << e.hallName << endl; // Add e.paid = payment status
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
    for (size_t k = 0; k < e.organizer.size(); ++k) {
        e.organizer[k] = toupper(e.organizer[k]);
    }
    while (true) {
        cout << "Enter price per ticket: ";
        cin >> e.ticketPrice;
        if (cin.fail() || e.ticketPrice < 0) {
            cout << "Invalid input. Please enter a non-negative number for price per ticket.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        else {
            cin.ignore(10000, '\n');
            break;
        }
    }
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

    // Venue selection (show only available venues)
    vector<int> availableVenueIndices;
    for (int i = 0; i < venues.size(); i++) {
        if (isVenueAvailable(venues[i])) {
            availableVenueIndices.push_back(i);
        }
    }
    if (availableVenueIndices.empty()) {
        cout << "No available venues. Please add or make a venue available first.\n";
        return;
    }
    cout << "Select a venue for this event:" << endl;
    for (int i = 0; i < availableVenueIndices.size(); i++) {
        int idx = availableVenueIndices[i];
        cout << i + 1 << ". " << venues[idx].venueName << " (" << venues[idx].venueAddress << ")" << endl;
    }
    int venueChoice;
    cout << "Enter venue number: ";
    cin >> venueChoice;
    if (venueChoice < 1 || venueChoice > availableVenueIndices.size()) {
        cout << "Invalid venue selection. Event not added.\n";
        return;
    }
    int selectedIdx = availableVenueIndices[venueChoice - 1];
    e.hallName = venues[selectedIdx].venueName;

    if (isConflict(events, e.date, e.startTime, e.endTime, e.hallName)) {
        cout << "Error: This slot is already booked!\n";
        return;
    }
    events.push_back(e);
    cout << "Event added successfully.\n";
}

//Event listing
void viewEvents(const vector<Event>& events, const vector<Venue>& venues) {
    cout << "===============================================================================================================" << endl;
    cout << "|                                         Event List                                                          |" << endl;
    cout << "===============================================================================================================" << endl;
    cout << "| " << left << setw(20) << "Event Name"
        << setw(15) << "Organizer"
        << setw(12) << "Date"
        << setw(8) << "Start"
        << setw(8) << "End"
        << setw(8) << "Hall ID"
        << setw(10) << "Hall"
        << setw(12) << "Rental"
        << setw(15) << "Price per Ticket" << " |" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < events.size(); i++) {
        double rentalPrice = 0.0;
        string hallID = "";
        for (int j = 0; j < venues.size(); j++) {
            if (venues[j].venueName == events[i].hallName) {
                rentalPrice = venues[j].rentalCost;
                hallID = venues[j].venueID;
                break;
            }
        }
        string hallDisplay = events[i].hallName;
        if (hallDisplay.length() > 9) {
            hallDisplay = hallDisplay.substr(0, 7) + "..";
        }
        cout << "| " << left << setw(20) << events[i].name
            << setw(15) << events[i].organizer
            << setw(12) << events[i].date
            << setw(8) << events[i].startTime
            << setw(8) << events[i].endTime
            << setw(8) << hallID
            << setw(10) << hallDisplay
            << setw(12) << fixed << setprecision(2) << rentalPrice
            << setw(15) << fixed << setprecision(2) << events[i].ticketPrice << " |" << endl;
    }
    cout << "===============================================================================================================" << endl;
}
//event info update
void updateEvent(vector<Event>& events, const vector<Venue>& venues) {
    if (events.empty()) {
        cout << "No events to update.\n";
        return;
    }
    cout << "==========================================================================================\n";
    cout << "|                        Select the event to update                                      |\n";
    cout << "==========================================================================================\n";
    cout << left << "| " << setw(3) << "No" << setw(20) << "Event Name" << setw(12) << "Date" << setw(8) << "Start" << setw(8) << "End" << setw(15) << "Hall" << " |" << endl;
    cout << "------------------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < events.size(); i++) {
        cout << left << "| " << setw(3) << i + 1 << setw(20) << events[i].name << setw(12) << events[i].date << setw(8) << events[i].startTime << setw(8) << events[i].endTime << setw(15) << events[i].hallName << " |" << endl;
    }
    cout << "==========================================================================================\n";
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
    // Convert organizer name to uppercase
    for (size_t k = 0; k < newOrganizer.size(); ++k) {
        newOrganizer[k] = toupper(newOrganizer[k]);
    }
    events[i].organizer = newOrganizer;
    while (true) {
        cout << "Enter new price per ticket: ";
        cin >> events[i].ticketPrice;
        if (cin.fail() || events[i].ticketPrice < 0) {
            cout << "Invalid input. Please enter a non-negative number for price per ticket.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        else {
            cin.ignore(10000, '\n'); // clear any extra input
            break;
        }
    }
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

    if (!venues.empty()) {
        vector<int> availableVenueIndices;
        for (int v = 0; v < venues.size(); v++) {
            if (isVenueAvailable(venues[v])) {
                availableVenueIndices.push_back(v);
            }
        }
        if (availableVenueIndices.empty()) {
            cout << "No available venues to update.\n";
            return;
        }
        cout << "Select a new venue for this event:" << endl;
        for (int v = 0; v < availableVenueIndices.size(); v++) {
            int idx = availableVenueIndices[v];
            cout << v + 1 << ". " << venues[idx].venueName << " (" << venues[idx].venueAddress << ")" << endl;
        }
        int venueChoice;
        cout << "Enter venue number: ";
        cin >> venueChoice;
        if (venueChoice < 1 || venueChoice > availableVenueIndices.size()) {
            cout << "Invalid venue selection. Venue not updated.\n";
            return;
        }
        int selectedIdx = availableVenueIndices[venueChoice - 1];
        events[i].hallName = venues[selectedIdx].venueName;
    }
    else {
        cout << "No venues available to update.\n";
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
    cout << "==========================================================================================\n";
    cout << "|                        Select the event to remove                                      |\n";
    cout << "==========================================================================================\n";
    cout << left << "| " << setw(3) << "No" << setw(20) << "Event Name" << setw(12) << "Date" << setw(8) << "Start" << setw(8) << "End" << setw(15) << "Hall" << " |" << endl;
    cout << "------------------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < events.size(); i++) {
        cout << left << "| " << setw(3) << i + 1 << setw(20) << events[i].name << setw(12) << events[i].date << setw(8) << events[i].startTime << setw(8) << events[i].endTime << setw(15) << events[i].hallName << " |" << endl;
    }
    cout << "==========================================================================================\n";
    int i;
    cout << "Enter event number: ";
    cin >> i;
    if (i < 1 || i > events.size()) {
        cout << "Invalid selection.\n";
        return;
    }
    //delete confirmation
    string confirmation = "";
    cout << "\nPlease enter 'CONFIRM' to confirm schedule removal: ";
    cin >> confirmation;
    if (confirmation == "CONFIRM")
    {
        events.erase(events.begin() + (i - 1));
        cout << "Event removed.\n";
    }
    else
    {
        cout << "Confirmation not confirmed. Returning ....";
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

// Get unique organizers in uppercase
vector<string> getUniqueOrganizers(const vector<Event>& events) {
    vector<string> organizers;
    for (int i = 0; i < events.size(); ++i) {
        string org = events[i].organizer;

        // Convert to uppercase manually
        for (int j = 0; j < org.length(); ++j) {
            org[j] = toupper(org[j]);
        }

        // Check if already added
        bool exists = false;
        for (int k = 0; k < organizers.size(); ++k) {
            if (organizers[k] == org) {
                exists = true;
                break;
            }
        }

        // Add if not found
        if (!exists) {
            organizers.push_back(org);
        }
    }
    return organizers;
}

// View events by organizer
void viewEventsByOrganizer(const vector<Event>& events, const vector<Venue>& venues) {
    vector<string> organizers = getUniqueOrganizers(events);
    if (organizers.empty()) {
        cout << "No organizers found...\n";
        return;
    }

    cout << "Select your organizer:\n";
    for (size_t i = 0; i < organizers.size(); ++i) {
        cout << i + 1 << ". " << organizers[i] << endl;
    }
    cout << "0. Cancel\n";

    int choice;
    while (true) {
        cout << "Enter choice (1-" << organizers.size() << ", 0 to cancel): ";
        cin >> choice;
        if (cin.fail() || choice < 0 || choice >(int)organizers.size()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input... Try again.\n";
        }
        else {
            break;
        }
    }
    if (choice == 0) {
        cout << "Operation cancelled...\n";
        return;
    }
    string organizerName = organizers[choice - 1];

    string organizerUpper = organizerName;
    for (size_t k = 0; k < organizerUpper.size(); ++k) {
        organizerUpper[k] = toupper(organizerUpper[k]);
    }
    cout << "============================================================================================================================" << endl;
    cout << "|                                                   Event List                                                             |" << endl;
    cout << "============================================================================================================================" << endl;
    cout << "| " << left << setw(20) << "Event Name"
        << setw(15) << "Organizer"
        << setw(12) << "Date"
        << setw(8) << "Start"
        << setw(8) << "End"
        << setw(15) << "Hall"
        << setw(12) << "Rental"
        << setw(15) << "Price/Ticket"
        << setw(15) << "Payment Status" << " |" << endl;
    cout << "----------------------------------------------------------------------------------------------------------------------------" << endl;
    bool found = false;
    for (int i = 0; i < events.size(); i++) {
        if (events[i].organizer == organizerUpper) {
            double rentalPrice = 0.0;
            for (int j = 0; j < venues.size(); j++) {
                if (venues[j].venueName == events[i].hallName) {
                    rentalPrice = venues[j].rentalCost;
                    break;
                }
            }
            cout << "| " << left << setw(20) << events[i].name
                << setw(15) << events[i].organizer
                << setw(12) << events[i].date
                << setw(8) << events[i].startTime
                << setw(8) << events[i].endTime
                << setw(15) << events[i].hallName
                << setw(12) << fixed << setprecision(2) << rentalPrice
                << setw(15) << fixed << setprecision(2) << events[i].ticketPrice
                << setw(15) << (events[i].paid ? "Paid" : "Unpaid") << " |" << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "No events found for organizer: " << organizerName << endl;
    }
    cout << "============================================================================================================================" << endl;
}

// Process payment for organizer's unpaid events
void organizerPayment(vector<Event>& events, const vector<Venue>& venues) {
    vector<string> organizers = getUniqueOrganizers(events);
    if (organizers.empty()) {
        cout << "No organizers found...\n";
        return;
    }

    cout << "Select your organizer:\n";
    for (size_t i = 0; i < organizers.size(); ++i) {
        cout << i + 1 << ". " << organizers[i] << endl;
    }
    cout << "0. Cancel\n";

    int choice;
    while (true) {
        cout << "Enter choice (1-" << organizers.size() << ", 0 to cancel): ";
        cin >> choice;
        if (cin.fail() || choice < 0 || choice >(int)organizers.size()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input... Try again.\n";
        }
        else {
            break;
        }
    }
    if (choice == 0) {
        cout << "Operation cancelled...\n";
        return;
    }
    string organizerName = organizers[choice - 1];

    // List unpaid events
    int unpaidIndices[100];
    int unpaidCount = 0;

    cout << "============================================================================================================================" << endl;
    cout << "|                                                   Unpaid Events                                                          |" << endl;
    cout << "============================================================================================================================" << endl;
    cout << "| " << left << setw(20) << "Event Name"
        << setw(15) << "Organizer"
        << setw(12) << "Date"
        << setw(8) << "Start"
        << setw(8) << "End"
        << setw(15) << "Hall"
        << setw(12) << "Rental"
        << setw(15) << "Price/Ticket"
        << setw(15) << "Payment Status" << " |" << endl;
    cout << "----------------------------------------------------------------------------------------------------------------------------" << endl;

    // Display header
    for (int i = 0; i < events.size(); i++) {
        string tempOrganizer = events[i].organizer;
        for (int j = 0; j < tempOrganizer.length(); j++) {
            tempOrganizer[j] = toupper(tempOrganizer[j]);
        }

        // Check for unpaid events
        if (tempOrganizer == organizerName && events[i].paid == false) {
            unpaidIndices[unpaidCount] = i;
            unpaidCount++;

            double rental = 0.0;
            for (int j = 0; j < venues.size(); j++) {
                if (venues[j].venueName == events[i].hallName) {
                    rental = venues[j].rentalCost;
                    break;
                }
            }

            cout << "| " << left << setw(3) << unpaidCount
                << setw(20) << events[i].name
                << setw(15) << events[i].organizer
                << setw(12) << events[i].date
                << setw(8) << events[i].startTime
                << setw(8) << events[i].endTime
                << setw(15) << events[i].hallName
                << setw(12) << fixed << setprecision(2) << rental
                << setw(15) << fixed << setprecision(2) << events[i].ticketPrice
                << setw(12) << "Unpaid" << " |" << endl;
        }
    }

    if (unpaidCount == 0) {
        cout << "All events are already paid.\n";
        return;
    }

    cout << "============================================================================================================================" << endl;

    int payChoice;
    while (true) {
        cout << "\nSelect event to pay (1-" << unpaidCount << ") or 0 to cancel: ";
        cin >> payChoice;

        // Input validation
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input... Please enter a number.\n";
            continue;
        }

        if (payChoice == 0) {
            cout << "Payment cancelled...\n";
            return;
        }

        if (payChoice >= 1 && payChoice <= unpaidCount) {
            break;
        }

        cout << "Invalid selection... Please try again.\n";
    }

    int selectedIndex = unpaidIndices[payChoice - 1];

    int method;
    string methodName;

    while (true) {
        cout << "\nChoose payment method:\n";
        cout << "1) Credit Card\n";
        cout << "2) Online Banking\n";
        cout << "3) E-Wallet\n";
        cout << "Enter choice: ";

        cin >> method;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input... Please enter a number.\n";
            continue;
        }

        if (method == 1) {
            methodName = "Credit Card";
            break;
        }
        else if (method == 2) {
            methodName = "Online Banking";
            break;
        }
        else if (method == 3) {
            methodName = "E-Wallet";
            break;
        }
        else {
            cout << "Invalid selection... Please choose 1, 2, or 3 to pay.\n";
        }
    }

    cout << "\nProcessing payment via " << methodName << "...\n";
    cout << "Please wait...\n";

    events[selectedIndex].paid = true;
    cout << "\nPayment successful for event: " << events[selectedIndex].name << endl;

    saveEvents(events);

    // Calculate rental cost
    double rental = 0.0;
    for (int j = 0; j < venues.size(); j++) {
        if (venues[j].venueName == events[selectedIndex].hallName) {
            rental = venues[j].rentalCost;
            break;
        }
    }

    // Display receipt
    cout << "\n===================================================" << endl;
    cout << "                     RECEIPT                       " << endl;
    cout << "===================================================" << endl;
    cout << "Event Name     : " << events[selectedIndex].name << endl;
    cout << "Organizer      : " << events[selectedIndex].organizer << endl;
    cout << "Date           : " << events[selectedIndex].date << endl;
    cout << "Time           : " << events[selectedIndex].startTime << " - " << events[selectedIndex].endTime << endl;
    cout << "Venue          : " << events[selectedIndex].hallName << endl;
    cout << "Rental Cost    : RM " << fixed << setprecision(2) << rental << endl;
    cout << "Payment Method : " << methodName << endl;
    cout << "Payment Status : PAID" << endl;
    cout << "===================================================" << endl;
    cout << "Thank you for using our service!" << endl;
}

// Refund and delete event for organizer
void refundPaymentEvent(vector<Event>& events, const vector<Venue>& venues) {
    if (events.size() == 0) {
        cout << "No events to refund.\n";
        return;
    }

    cout << "==========================================================================================\n";
    cout << "|                        Select the event to refund                                      |\n";
    cout << "==========================================================================================\n";
    cout << "| " << left
        << setw(3)  << "No"
        << setw(20) << "Event Name"
        << setw(15) << "Organizer"
        << setw(12) << "Date"
        << setw(8)  << "Start"
        << setw(8)  << "End"
        << setw(22) << "Hall"           
        << setw(12) << "Paid Status"    
        << "|\n";
    cout << "------------------------------------------------------------------------------------------\n";
    for (int i = 0; i < events.size(); i++) {
        cout << "| " << left
            << setw(3)  << i + 1
            << setw(20) << events[i].name
            << setw(15) << events[i].organizer
            << setw(12) << events[i].date
            << setw(8)  << events[i].startTime
            << setw(8)  << events[i].endTime
            << setw(22) << events[i].hallName      
            << setw(12) << (events[i].paid ? "Paid" : "Unpaid") 
            << "|\n";
    }
    cout << "==========================================================================================\n";

    int idx;
    while (true) {
        cout << "Enter event number to refund: ";
        cin >> idx;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input... Please enter a number.\n";
            continue;
        }

        if (idx < 1 || idx > events.size()) {
            cout << "Invalid selection... Please choose a number between 1 and " << events.size() << ".\n";
            continue;
        }

        break;
    }

    idx = idx - 1;

    if (events[idx].paid == false) {
        cout << "Refund is only available for paid events.\n";
        return;
    }

    double rental = 0.0;
    for (int i = 0; i < venues.size(); i++) {
        if (venues[i].venueName == events[idx].hallName) {
            rental = venues[i].rentalCost;
            break;
        }
    }

    string confirm;
    cout << "\nPlease enter 'CONFIRM' to proceed with refund and event deletion: ";
    cin >> confirm;

    if (confirm != "CONFIRM") {
        cout << "Confirmation not confirmed. Returning...\n";
        return;
    }

    cout << "\nProcessing refund of RM " << fixed << setprecision(2)
        << rental << " to organizer: " << events[idx].organizer << "...\n";
    cout << "Please wait...\n";
    cout << "Refund successful.\n";

    cout << "Deleting event: " << events[idx].name << "\n";
    events.erase(events.begin() + idx);

    saveEvents(events);

    cout << "Event deleted and refund processed.\n";
}
