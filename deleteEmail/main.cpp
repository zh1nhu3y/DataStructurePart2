// #include <iostream>
// #include "deleteEmail.h" // Include deleteEmail functionality

// using namespace std;

// void displayDeleteMenu() {
//     cout << "\n--- Email Management System ---\n";
//     cout << "1. View emails\n";
//     cout << "2. Delete an email\n";
//     cout << "3. Exit\n";
//     cout << "Enter your choice: ";
// }

// void viewEmails(PriorityQueue &pq, const string &currentUser) {
//     cout << "\nEmails relevant to: " << currentUser << "\n";
//     bool found = false;
//     Email *current = pq.head;

//     // Display emails and their IDs where currentUser is either sender or recipient
//     while (current) {
//         if (current->sender == currentUser || current->recipient == currentUser) {
//             found = true;
//             cout << "ID: " << current->id 
//                  << " | From: " << current->sender 
//                  << " | To: " << current->recipient 
//                  << " | Subject: " << current->subject 
//                  << " | Date: " << current->datetime 
//                  << " | Status: " << current->status 
//                  << " | Priority: " << current->priority 
//                  << " | Spam Status: " << current->spamStatus << "\n";
//         }
//         current = current->next;
//     }

//     if (!found) {
//         cout << "No emails found for this user.\n";
//     }
// }

// void deleteEmailById(PriorityQueue &pq, const string &emailFile, const string &recycleBinFile, const string &currentUser) {
//     string emailID;
//     cout << "Enter email ID of the email to delete: ";
//     cin.ignore();
//     getline(cin, emailID);

//     if (deleteEmail(emailFile, recycleBinFile, currentUser, emailID)) {
//         cout << "Email with ID " << emailID << " deleted successfully.\n";
//         // Clear and reload the emails in PriorityQueue
//         pq.clear();
//         pq.loadUserEmails(emailFile, currentUser);
//     } else {
//         cout << "Failed to delete email with ID " << emailID << ". It may not exist or you may not have permission to delete it.\n";
//     }
// }

// int main() {
//     const string emailFile = "../Data/dummy_emails.txt";  // Path to main email file
//     const string recycleBinFile = "../Data/recycle_bin.txt";  // Path to recycle bin file
//     const string currentUser = "charlie.johnson@demo.com"; // Placeholder for logged-in user's email

//     PriorityQueue pq;
    
//     // Load emails relevant to the current user for viewing
//     pq.loadUserEmails(emailFile, currentUser);

//     int choice;
//     bool running = true;

//     while (running) {
//         displayDeleteMenu();
//         cin >> choice;

//         switch (choice) {
//             case 1:
//                 // View emails relevant to the current user
//                 viewEmails(pq, currentUser);
//                 break;

//             case 2:
//                 // Delete an email by ID
//                 deleteEmailById(pq, emailFile, recycleBinFile, currentUser);
//                 break;

//             case 3:
//                 // Exit
//                 running = false;
//                 cout << "Exiting the email management system. Goodbye!\n";
//                 break;

//             default:
//                 cout << "Invalid choice. Please try again.\n";
//         }
//     }

//     return 0;
// }

// main.cpp
#include <iostream>
#include "deleteEmail.h" // Include deleteEmail functionality

using namespace std;

void displayDeleteMenu() {
    cout << "\n--- Email Management System ---\n";
    cout << "1. View emails\n";
    cout << "2. Delete an email\n";
    cout << "3. View deleted emails\n";
    cout << "4. Restore an email\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
}

void viewEmails(PriorityQueue &pq, const string &currentUser) {
    cout << "\nEmails relevant to: " << currentUser << "\n";
    bool found = false;
    Email *current = pq.head;

    while (current) {
        if (current->sender == currentUser || current->recipient == currentUser) {
            found = true;
            cout << "ID: " << current->id 
                 << " | From: " << current->sender 
                 << " | To: " << current->recipient 
                 << " | Subject: " << current->subject 
                 << " | Date: " << current->datetime 
                 << " | Status: " << current->status 
                 << " | Priority: " << current->priority 
                 << " | Spam Status: " << current->spamStatus << "\n";
        }
        current = current->next;
    }

    if (!found) {
        cout << "No emails found for this user.\n";
    }
}

void restoreEmails(PriorityQueue &pq, const string &emailFile, const string &recycleBinFile, const string &currentUser) {
    if (restoreEmailById(recycleBinFile, emailFile, currentUser)) {
        cout << "The selected email has been restored successfully.\n";
        pq.clear(); // Clear the current emails in memory
        pq.loadUserEmails(emailFile, currentUser); // Reload the emails after the restoration
    } else {
        cout << "No email restored.\n";
    }
}

void deleteEmailById(PriorityQueue &pq, const string &emailFile, const string &recycleBinFile, const string &currentUser) {
    string emailID;
    cout << "Enter email ID of the email to delete: ";
    cin.ignore();
    getline(cin, emailID);

    if (deleteEmail(emailFile, recycleBinFile, currentUser, emailID)) {
        cout << "Email with ID " << emailID << " deleted successfully.\n";
        pq.clear();
        pq.loadUserEmails(emailFile, currentUser);
    } else {
        cout << "Failed to delete email with ID " << emailID << ". It may not exist or you may not have permission to delete it.\n";
    }
}

int main() {
    const string emailFile = "../Data/dummy_emails.txt";
    const string recycleBinFile = "../Data/recycle_bin.txt";
    const string currentUser = "charlie.johnson@demo.com"; 

    PriorityQueue pq;
    pq.loadUserEmails(emailFile, currentUser);

    int choice;
    bool running = true;

    while (running) {
        displayDeleteMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                viewEmails(pq, currentUser);
                break;

            case 2:
                deleteEmailById(pq, emailFile, recycleBinFile, currentUser);
                break;

            case 3:
                viewDeletedEmailsLIFO(recycleBinFile);
                break;

            case 4:
                restoreEmails(pq, emailFile, recycleBinFile, currentUser);
                break;

            case 5:
                running = false;
                cout << "Exiting the email management system. Goodbye!\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
