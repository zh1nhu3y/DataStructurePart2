#include "spamDetection.h"

int main()
{
    EmailQueue emailQueue;
    SpamDetection spamDetection;
    string spamWords[MAX_SPAM_WORDS];

    // Spam word input file
    const string spamFile = "../Data/spam_words.txt";
    const string emailFile = "../Data/dummy_emails.txt";

    // Load spam words from a file
    int spamWordCount = spamDetection.loadSpamWords(spamFile, spamWords, MAX_SPAM_WORDS);

    // Sort spam words for binary search
    spamDetection.quickSort(spamWords, 0, spamWordCount - 1);

    // Load email from email file
    spamDetection.enqueueEmail(emailFile, emailQueue);

    // spamDetection.checkSpamStatus(emailQueue, spamWords, spamWordCount, emailFile); // Run once and remove

    int choice, id;

    do
    {
        cout << "\nEmail Management System" << endl;
        cout << "1. Display Spam Emails" << endl;
        cout << "2. Mark Email as Spam by ID" << endl;
        cout << "3. Unmark Email as Spam by ID" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            spamDetection.displaySpamEmails(emailQueue); // Pass emailQueue
            break;
        case 2:
            cout << "Enter Email ID to mark as spam: ";
            cin >> id;
            spamDetection.markAsSpam(emailQueue, id, emailFile); // Pass emailQueue and id
            break;
        case 3:
            cout << "Enter Email ID to unmark as spam: ";
            cin >> id;
            spamDetection.unmarkAsSpam(emailQueue, id, emailFile); // Pass emailQueue and id
            break;
        case 4:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }

    } while (choice != 4);

    return 0;
}
