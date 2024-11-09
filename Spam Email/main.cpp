#include <iostream>
#include <string>
#include "spamDetection.h"
#include "/Users/elvin/Documents/GitHub/DataStructurePart2/Inbox and Outbox Management/OutboxQueue.h"

using namespace std;

int main()
{
    OutboxQueue outboxQueue; // Use OutboxQueue instead of EmailQueue
    SpamDetection spamDetection;
    string spamWords[MAX_SPAM_WORDS];

    // Spam word input file
    const string spamFile = "../Data/spam_words.txt";
    const string emailFile = "../Data/dummy_emails.txt";

    // Load spam words from a file
    int spamWordCount = spamDetection.loadSpamWords(spamFile, spamWords, MAX_SPAM_WORDS);

    // Sort spam words for binary search
    spamDetection.quickSort(spamWords, 0, spamWordCount - 1);

    // Load emails from email file into OutboxQueue
    spamDetection.checkAllEmailsInOutbox(outboxQueue, spamWords, spamWordCount, emailFile);

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
            spamDetection.displaySpamEmails(outboxQueue); // Pass OutboxQueue
            break;
        case 2:
            cout << "Enter Email ID to mark as spam: ";
            cin >> id;
            spamDetection.markAsSpam(outboxQueue, id, emailFile); // Pass OutboxQueue and id
            break;
        case 3:
            cout << "Enter Email ID to unmark as spam: ";
            cin >> id;
            spamDetection.unmarkAsSpam(outboxQueue, id, emailFile); // Pass OutboxQueue and id
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