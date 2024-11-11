#include "InboxAndOutboxMain.h"
#include "spamDetection.h"

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
    const string userEmail = "mj@gmail.com";

    emailSystem.loadEmailsFromFile(emailFile, inbox, outbox);

    // Load spam words from a file
    int spamWordCount = spamDetection.loadSpamWords(spamFile, spamWords, MAX_SPAM_WORDS);

    // Sort spam words for binary search
    spamDetection.quickSort(spamWords, 0, spamWordCount - 1);

    // emailSystem.displayLatestEmail();

    int mainChoice;
    int choice, id;
    do
    {
        cout << "\nChoose an option:\n"
             << "1. Inbox Management\n"
             << "2. Outbox Management\n"
             << "3. Spam Management\n"
             << "4. Exit\n"
             << "Enter your choice (1-4): ";
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
        case 4:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid option. Please choose a number between 1 and 4.\n";
            break;
        }
    } while (mainChoice != 4);

    return 0;
}
