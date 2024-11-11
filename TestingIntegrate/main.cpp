// #include <iostream>
// #include <fstream>
// #include <string>
// #include <set>
// #include <sstream>
// #include "delete.h"
// #include "inboxS.h"
// #include "outboxQ.h"
// #include "priority.h"

// using namespace std;

// std::string getCurrentTimestamp() {
//     time_t now = time(0);
//     tm* localtm = localtime(&now);
//     char buffer[20];
//     strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", localtm);
//     return std::string(buffer);
// }

// // Fix: Pass the correct parameters, removing the invalid references
// int main() {
//     InboxStack inbox;
//     OutboxQueue outbox;
//     std::set<int> existingIds;
//     std::string currentUser = "charlie.johnson@demo.com";

//     loadEmailsFromFile("../Data/dummy_emails.txt", inbox, outbox, existingIds);

//     bool running = true;
//     int choice;

//     while (running) {
//         displayMainMenu();
//         cin >> choice;

//         switch (choice) {
//             case 1: // View Inbox
//                 inbox.displayAllForUser(currentUser);
//                 break;

//             case 2: // View Outbox
//                 outbox.displayAllForUser(currentUser);
//                 break;

//             case 3: // View Deleted Emails
//                 viewDeletedEmailsFIFO("../Data/recycle_bin.txt", currentUser);
//                 break;

//             case 4: // Delete Email
//                 deleteEmailFromInbox(inbox, "../Data/recycle_bin.txt", currentUser);  // No reference for OutboxQueue
//                 break;

//             case 5: // Restore Email
//                 restoreEmails(inbox, "../Data/recycle_bin.txt", currentUser);  // No reference for OutboxQueue
//                 break;

//             case 6: // View Prioritized Emails
//                 displayEmailsByPriority(inbox, outbox, currentUser);  // Make sure displayEmailsByPriority is declared correctly
//                 break;

//             case 7: // Edit Email Priority
//                 {
//                     int emailID, newPriority;
//                     cout << "Enter email ID to edit priority: ";
//                     cin >> emailID;
//                     cout << "Enter new priority: ";
//                     cin >> newPriority;
//                     editEmailPriority(inbox, outbox, emailID, newPriority);
//                 }
//                 break;

//             case 8: // Search Emails by Priority
//                 {
//                     int searchPriority;
//                     cout << "Enter priority level to search: ";
//                     cin >> searchPriority;
//                     searchEmailsByPriority(inbox, outbox, searchPriority, currentUser);
//                 }
//                 break;

//             case 9: // Exit
//                 cout << "Exiting system. Goodbye!" << endl;
//                 running = false;
//                 break;

//             default:
//                 cout << "Invalid choice. Please try again." << endl;
//                 break;
//         }
//     }

//     return 0;
// }
