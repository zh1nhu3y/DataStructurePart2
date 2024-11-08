#include <iostream>
#include "priorityHandling.h"

using namespace std;

void displayMenu() {
    cout << "\n--- Email System Menu ---\n";
    cout << "1. Display emails by priority\n";
    cout << "2. Edit email priority\n";
    cout << "3. Search emails by priority\n";
    cout << "4. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    const string emailFile = "../Data/dummy_emails.txt";
    const string currentUser = "charlie.johnson@demo.com"; // Placeholder for logged-in user

    PriorityQueue pq;
    pq.loadEmailsFromFile(emailFile);

    int choice;
    bool running = true;

    while (running) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                // Display emails by priority for the logged-in user
                pq.displayEmailsByPriority(currentUser);
                break;

            case 2: {
                // Edit email priority
                string emailID;
                int newPriority;

                cout << "Enter email ID to edit priority: ";
                cin.ignore();
                getline(cin, emailID);
                cout << "Enter new priority (1-5): ";
                cin >> newPriority;

                pq.editEmailPriority(emailFile, currentUser, emailID, newPriority);
                break;
            }

            case 3: {
                // Search emails by a specific priority
                int searchPriority;
                cout << "Enter priority to search for: ";
                cin >> searchPriority;
                pq.searchEmailsByPriorityForUser(currentUser, searchPriority);
                break;
            }

            case 4:
                // Exit
                running = false;
                cout << "Exiting the email system. Goodbye!\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}