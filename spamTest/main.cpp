#include "InboxAndOutboxMain.h"
#include "spamDetection.h"
#include "delete.h"
#include "priority.h"

using namespace std;

int main()
{
    InboxStack inbox;
    OutboxQueue outbox;
    InboxandOutboxManagement emailSystem;
    SpamDetection spamDetection;
    string spamWords[MAX_SPAM_WORDS];

    const string spamFile = "../Data/spam_words.txt";
    const string emailFile = "../Data/dummy_emails.txt";
    // const string userEmail = "mj@gmail.com";
    const string userEmail = "charlie.johnson@demo.com";

    emailSystem.loadEmailsFromFile(emailFile, inbox, outbox);

    // Load spam words from a file
    int spamWordCount = spamDetection.loadSpamWords(spamFile, spamWords, MAX_SPAM_WORDS);

    // Sort spam words for binary search
    spamDetection.quickSort(spamWords, 0, spamWordCount - 1);

    // emailSystem.displayLatestEmail();

    int mainChoice;
    int choice, id, emailID, newPriority;
    do
    {
        cout << "\nChoose an option:\n"
             << "1. Inbox Management\n"
             << "2. Outbox Management\n"
             << "3. Spam Management\n"
             << "4. View Emails by Priority\n"
             << "5. Edit Email Priority\n"
             << "6. Delete Email\n"
             << "7. Search Emails by Priority\n"
             << "8. View Deleted Emails\n"
             << "9. Restore Deleted Email\n"
             << "10. Exit\n"
             << "Enter your choice (1-10): ";
        cin >> mainChoice;
        cin.ignore();
        cout << endl;

        switch (mainChoice)
        {
        case 1:
            emailSystem.handleInboxManagement(inbox);
            break;
        case 2:
            emailSystem.handleOutboxManagement(outbox, inbox, emailFile, spamWords, spamWordCount);
            break;
        case 3:
            do
            {
                cout << "\nSpam Management:\n"
                     << "1. Display Spam Emails\n"
                     << "2. Mark Email as Spam by ID\n"
                     << "3. Unmark Email as Spam by ID\n"
                     << "4. Back to Main Menu\n"
                     << "Enter your choice: ";
                cin >> choice;
                cin.ignore();

                switch (choice)
                {
                case 1:
                    spamDetection.displaySpamEmails(inbox, userEmail);
                    break;
                case 2:
                    cout << "Enter Email ID to mark as spam: ";
                    cin >> id;
                    spamDetection.markAsSpam(outbox, id, emailFile, userEmail, inbox);
                    break;
                case 3:
                    cout << "Enter Email ID to unmark as spam: ";
                    cin >> id;
                    spamDetection.unmarkAsSpam(outbox, id, emailFile, userEmail, inbox);
                    break;
                case 4:
                    cout << "Returning to main menu...\n";
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
                }
            } while (choice != 4);
            break;
        case 4: // View Emails by Priority
            displayEmailsByPriority(inbox, outbox, userEmail);
            break;
        case 5: // Edit Email Priority
            cout << "Enter email ID to edit priority: ";
            cin >> emailID;
            cout << "Enter new priority: ";
            cin >> newPriority;
            editEmailPriority(inbox, outbox, emailFile, emailID, newPriority);
            break;
        case 6: // Delete Email
             deleteEmail(inbox, outbox, "../Data/recycle_bin.txt", "../Data/dummy_emails.txt", userEmail);
            break;
        case 7: // Search Emails by Priority
            searchEmailsByPriority(inbox, outbox, userEmail);
            break;
        case 8: // View Deleted Emails
            viewDeletedEmailsFIFO("../Data/recycle_bin.txt", userEmail);
            break;
        case 9: // Restore Deleted Email
            restoreEmail(inbox, outbox, "../Data/recycle_bin.txt", emailFile, userEmail);
            break;
        case 10:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid option. Please choose a number between 1 and 10.\n";
            break;
        }
    } while (mainChoice != 10);

    return 0;
}
