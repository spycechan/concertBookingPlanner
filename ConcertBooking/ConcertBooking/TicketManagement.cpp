#include "TicketManagement.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cctype>
#include <algorithm>
using namespace std;

// Name Validation
bool isValidName(const string& name) {
    int len = name.length();
    if (len < 2) {
        return false; // Name too short
    }

    for (int i = 0; i < len; i++) {
        char c = name[i];
        if (!(isalpha(c) || c == ' ')) {
            return false; // Invalid character found
        }
    }

    return true; // All characters valid
}

// Phone Number Validation
bool isValidPhone(const string& phone) {
    int len = phone.length();
    if (len < 10 || len > 12) {
        return false; // Invalid length
    }

    for (int i = 0; i < len; i++) {
        if (!isdigit(phone[i])) {
            return false; // Non-digit found
        }
    }

    return true; // All digits valid
}

// IC Validation
bool isValidIc(const string& ic) {
    if (ic.length() != 12) {
        return false; // Must be exactly 12 digits
    }

    for (int i = 0; i < 12; i++) {
        if (!isdigit(ic[i])) {
            return false; // Non-digit found
        }
    }

    return true; // All digits valid
}

// Get venue capacity by event name
int getVenueCapacity(const vector<Event>& events, const vector<Venue>& venues, const string& eventName) {
    if (eventName.empty()) return 0;
    for (size_t i = 0; i < events.size(); ++i) {
        if (events[i].name == eventName) {
            for (size_t j = 0; j < venues.size(); ++j) {
                if (venues[j].venueName == events[i].hallName) {
                    return venues[j].venueCapacity;
                }
            }
        }
    }
    return 0;
}

// Count tickets sold for an event
int countTicketsSold(const vector<Ticket>& tickets, const string& eventName) {
    if (eventName.empty()) return 0;
    int count = 0;
    for (size_t i = 0; i < tickets.size(); ++i) {
        if (tickets[i].eventName == eventName) {
            ++count;
        }
    }
    return count;
}

// Generate a unique ticket ID
string generateTicketID() {
    int randomNumber = 100000 + rand() % 900000; // Generate a random 6-digit number
    return "T" + to_string(randomNumber);
}

// Save tickets to a file
void saveTickets(const vector<Ticket>& tickets, const string& filename) {
    ofstream fout(filename.c_str());
    if (!fout) {
        cerr << "Error opening file for writing!" << endl;
        return;
    }
    for (size_t i = 0; i < tickets.size(); i++) {
        fout << tickets[i].custName << "\n";
        fout << tickets[i].custPhone << "\n";
        fout << tickets[i].custIc << "\n";
        fout << tickets[i].ticketId << "\n";
        fout << tickets[i].eventName << "\n";
        fout << tickets[i].eventDate << "\n";
        fout << tickets[i].hallName << "\n";
        fout << (tickets[i].paid ? "1" : "0") << "\n";
    }
    fout.close();
}

// Load tickets from a file
void loadTickets(vector<Ticket>& tickets, const string& filename) {
    ifstream fin(filename.c_str());
    if (!fin) {
        cerr << "Error opening file for reading!" << endl;
        return;
    }
    Ticket t;
    string paidStr;
    while (getline(fin, t.custName)) {
        getline(fin, t.custPhone);
        getline(fin, t.custIc);
        getline(fin, t.ticketId);
        getline(fin, t.eventName);
        getline(fin, t.eventDate);
        getline(fin, t.hallName);
        getline(fin, paidStr);
        t.paid = (paidStr == "1");
        tickets.push_back(t);
    }
    fin.close();
}

// Purchase ticket function
void purchaseTicket(const vector<Event>& events, const vector<Venue>& venues, vector<Ticket>& tickets) {
    if (events.empty()) {
        cout << "No events available for ticket purchase.\n";
        return;
    }

    // Display available events
    cout << "\n============================================================================================================\n";
    cout << "|                                         AVAILABLE EVENTS                                                |\n";
    cout << "============================================================================================================\n";
    cout << "| No | Event Name              | Date        | Venue               | Tickets Left |\n";
    cout << "------------------------------------------------------------------------------------------------------------\n";

    for (int i = 0; i < (int)events.size(); i++) {
        int sold = countTicketsSold(tickets, events[i].name);
        int capacity = getVenueCapacity(events, venues, events[i].name);

        cout << "| " << right << setw(2) << (i + 1)
            << " | " << left << setw(22) << events[i].name
            << " | " << left << setw(11) << events[i].date
            << " | " << left << setw(19) << events[i].hallName
            << " | " << right << setw(12) << (capacity - sold)
            << " |\n";
    }

    cout << "============================================================================================================\n";

    // User selects event
    int choice = -1;
    while (true) {
        cout << "Select event number to purchase ticket (1-" << events.size() << ") or 0 to cancel: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice == 0) {
            cout << "Purchase cancelled.\n";
            return;
        }

        if (choice >= 1 && choice <= (int)events.size()) {
            break; // valid selection
        }

        cout << "Invalid selection. Please enter a number between 1 and " << events.size() << ".\n";
    }

    const Event& selectedEvent = events[choice - 1];

    // Check ticket availability
    int sold = 0;
    for (int i = 0; i < (int)tickets.size(); i++) {
        if (tickets[i].eventName == selectedEvent.name) {
            sold++;
        }
    }

    // Get venue capacity
    int capacity = 0;
    for (int i = 0; i < (int)venues.size(); i++) {
        if (venues[i].venueName == selectedEvent.hallName) {
            capacity = venues[i].venueCapacity;
            break;
        }
    }

    if (sold >= capacity) {
        cout << "Sorry, tickets for this event are sold out :)\n";
        return;
    }

    // Collect customer details
    Ticket t;
    cin.ignore();

    cout << "Enter your name: ";
    getline(cin, t.custName);
    while (!isValidName(t.custName)) {
        cout << "Invalid name. Please enter again: ";
        getline(cin, t.custName);
    }

    cout << "Enter your phone number (E.g. 0123456789): ";
    getline(cin, t.custPhone);
    while (!isValidPhone(t.custPhone)) {
        cout << "Invalid phone number. Please enter again: ";
        getline(cin, t.custPhone);
    }

    cout << "Enter your IC (E.g. 012345678912): ";
    getline(cin, t.custIc);
    while (!isValidIc(t.custIc)) {
        cout << "Invalid IC. Please enter again: ";
        getline(cin, t.custIc);
    }

    t.ticketId = generateTicketID();
    t.eventName = selectedEvent.name;
    t.eventDate = selectedEvent.date;
    t.hallName = selectedEvent.hallName;

    // Payment method selection
    int payMethod = 0;
    string payMethodName;
    while (true) {
        cout << "Choose payment method:\n1) Credit Card\n2) Online Banking\n3) E-Wallet\nEnter choice: ";
        cin >> payMethod;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }

        if (payMethod == 1) { payMethodName = "Credit Card"; break; }
        if (payMethod == 2) { payMethodName = "Online Banking"; break; }
        if (payMethod == 3) { payMethodName = "E-Wallet"; break; }

        cout << "Invalid selection. Try again.\n";
    }

    t.paid = true;
    tickets.push_back(t);
    saveTickets(tickets, "tickets.txt");

    // Display receipt
    cout << "\n==================== TICKET RECEIPT ====================\n";
    cout << left << setw(18) << "Customer Name" << ": " << t.custName << "\n";
    cout << left << setw(18) << "Phone Number" << ": " << t.custPhone << "\n";
    cout << left << setw(18) << "IC" << ": " << t.custIc << "\n";
    cout << left << setw(18) << "Ticket ID" << ": " << t.ticketId << "\n";
    cout << left << setw(18) << "Event" << ": " << t.eventName << "\n";
    cout << left << setw(18) << "Date" << ": " << t.eventDate << "\n";
    cout << left << setw(18) << "Venue" << ": " << t.hallName << "\n";
    cout << left << setw(18) << "Payment Method" << ": " << payMethodName << "\n";
    cout << left << setw(18) << "Status" << ": PAID\n";
    cout << "=========================================================\n";
	cout << "Thank you for your purchase! Enjoy the event!\n";
}

void viewTickets(const vector<Ticket>& tickets) {
    cin.ignore(); // Clear leftover newline

    string name, phone;

    // Name input and validation
    cout << "Enter your name: ";
    getline(cin, name);
    while (!isValidName(name)) {
        cout << "Invalid name. Please enter again: ";
        getline(cin, name);
    }

    // Phone input and validation
    cout << "Enter your phone number (E.g. 0123456789): ";
    getline(cin, phone);
    while (!isValidPhone(phone)) {
        cout << "Invalid phone number. Please enter again: ";
        getline(cin, phone);
    }

    // Convert input to lowercase manually
    for (int i = 0; i < (int)name.length(); i++) {
        name[i] = tolower(name[i]);
    }
    for (int i = 0; i < (int)phone.length(); i++) {
        phone[i] = tolower(phone[i]);
    }

    bool found = false;

    // Display Ticket List
    cout << "\n================================================================================================================\n";
    cout << "|                                             Your Ticket List                                                 |\n";
    cout << "================================================================================================================\n";
    cout << "| Ticket ID  | Event Name              | Date        | Venue               | Status   |\n";
    cout << "----------------------------------------------------------------------------------------------------------------\n";

    for (int i = 0; i < (int)tickets.size(); i++) {
        string storedName = tickets[i].custName;
        string storedPhone = tickets[i].custPhone;

        // Convert stored data to lowercase manually
        for (int j = 0; j < (int)storedName.length(); j++) {
            storedName[j] = tolower(storedName[j]);
        }
        for (int j = 0; j < (int)storedPhone.length(); j++) {
            storedPhone[j] = tolower(storedPhone[j]);
        }

        if (storedName == name && storedPhone == phone) {
            cout << "| " << left << setw(10) << tickets[i].ticketId
                << " | " << left << setw(22) << tickets[i].eventName
                << " | " << left << setw(11) << tickets[i].eventDate
                << " | " << left << setw(19) << tickets[i].hallName
                << " | " << left << setw(8) << (tickets[i].paid ? "PAID" : "UNPAID")
                << " |\n";
            found = true;
        }
    }

    if (!found) {
        cout << "| No tickets found for the provided details...                                                                |\n";
    }

    cout << "================================================================================================================\n";
}