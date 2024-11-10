#include "InboxStack.h"
#include "OutboxQueue.h"
#include "spamDetection.h"
#include "UserAuth.h"

void displayMenu1()
{
    cout << "\n\nMain Menu\n" << endl;
    cout << "1. View Inbox\n" << endl;
    cout << "2. View Sent Emails\n" << endl;
    cout << "3. View Starred Emails\n" << endl;
    cout << "4. View Spam\n" << endl; 
    cout << "5. Search Emails\n" << endl;
    cout << "\n0. Exit\n" << endl;
    cout << "Please Enter your choice: ";
}

int main()
{
    OutboxQueue outboxQueue;
    InboxStack inboxStack;
    
    SpamDetection spamDetection;
    string spamWords[MAX_SPAM_WORDS];

    UserAuth userAuth;
    string username, password;
    string loggedInEmail;

    int choice;
    do{
        cout << "\nEmail Management System\n\n" << endl;
        cout << "1. Login\n";
        // cout << "2. Register\n\n";
        cout << "0. End this program\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice){
            case 1:
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;

                if (userAuth.login(username, password)) {
                    loggedInEmail = userAuth.getLoggedInEmail();  // Get the logged-in email
                    cout << "Login successful! Welcome, " << loggedInEmail << endl;
                    // Continue with email management functionality here
                    do{
                        displayMenu1();
                        cin >> choice;
                        switch (choice) {
                            case 1:
                                // View Inbox
                                break;
                            case 2:
                                // View Sent Emails
                                break;
                            case 3:
                                // View Starred Emails
                                break;
                            case 4:
                                // View Spam
                                break;
                            case 5:
                                // Search Emails
                                break;
                            case 0:
                                // Exit
                                break;
                            default:
                                cout << "Invalid choice. Please try again." << endl;
                        }
                    } while (choice != 0);
                } else {
                    cout << "Invalid credentials! Please Try Again" << endl;
                }
                break;
            case 0:
                cout << "Exiting the system." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }

    }while (choice != 0);
    

    return 0;
}