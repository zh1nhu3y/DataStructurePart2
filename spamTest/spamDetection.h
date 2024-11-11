#ifndef SPAMDETECTION_H
#define SPAMDETECTION_H

#include "OutboxQueue.h"
#include "InboxStack.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

const int MAX_SPAM_WORDS = 100;

class SpamDetection
{
public:
    // Loads spam words from a file into an array
    int loadSpamWords(const string &filename, string spamWords[], int maxWords)
    {
        ifstream file(filename);
        int count = 0;

        if (file.is_open())
        {
            string word;
            while (file >> word && count < maxWords)
            {
                spamWords[count++] = word;
            }
            file.close();
            cout << "Spam Words File Loaded" << endl;
        }
        else
        {
            cout << "Error opening spam words file." << endl;
        }
        return count;
    }

    // Function to swap two elements (used in sorting)
    void swap(string &a, string &b)
    {
        string temp = a;
        a = b;
        b = temp;
    }

    // Partition function for quick sort
    int partition(string spamWords[], int low, int high)
    {
        string pivot = spamWords[high];
        int i = low - 1;
        for (int j = low; j < high; j++)
        {
            if (spamWords[j] < pivot)
            {
                i++;
                swap(spamWords[i], spamWords[j]);
            }
        }
        swap(spamWords[i + 1], spamWords[high]);
        return i + 1;
    }

    // Quick Sort function to sort spam words array
    void quickSort(string spamWords[], int low, int high)
    {
        if (low < high)
        {
            int pi = partition(spamWords, low, high);
            quickSort(spamWords, low, pi - 1);
            quickSort(spamWords, pi + 1, high);
        }
    }

    // Binary search function to check if a word is a spam word
    bool binarySearch(const string &word, string spamWords[], int n)
    {
        int left = 0;
        int right = n - 1;
        while (left <= right)
        {
            int mid = left + (right - left) / 2;
            if (spamWords[mid] == word)
            {
                return true;
            }
            else if (spamWords[mid] < word)
            {
                left = mid + 1;
            }
            else
            {
                right = mid - 1;
            }
        }
        return false;
    }

    // Check if a single email is spam and add it to OutboxQueue
    void checkSingleEmailAndAppend(Email *email, string spamWords[], int spamWordCount,
                                   OutboxQueue &outboxQueue, const string &emailFile)
    {
        stringstream contentStream(email->body);

        string word;
        bool isSpam = false;

        while (contentStream >> word)
        {
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());

            if (binarySearch(word, spamWords, spamWordCount))
            {
                isSpam = true;
                cout << "Spam Detected..." << endl;
                break;
            }
        }

        email->spamStatus = isSpam ? "Yes" : "No";

        Email emailToAdd(email->id, email->sender, email->receiver, email->subject,
                         email->body, email->timestamp, email->status, email->priority,
                         isSpam ? "Yes" : "No");

        // Save to file with updated spam status
        ofstream file(emailFile, ios::app);
        if (file.is_open())
        {
            file << email->id << "|" << email->sender << "|" << email->receiver << "|"
                 << email->subject << "|" << email->body << "|" << email->timestamp << "|"
                 << email->status << "|" << email->priority << "|"
                 << (isSpam ? "Yes" : "No") << "\n";
            file.close();
            // cout << "Email processed for spam and saved successfully!" << endl;
        }
        else
        {
            cout << "Error: Unable to open file for writing." << endl;
        }
    }

    // Function to display all spam emails
    void displaySpamEmails(InboxStack &inboxStack, const string &userEmail)
    {
        Node *current = inboxStack.getTop();
        cout << "\n-------------- Spam Emails --------------\n\n";
        bool hasSpam = false;
        while (current != nullptr)
        {
            if (current->email.spamStatus == "Yes" && current->email.receiver == userEmail)
            {
                hasSpam = true;
                cout << "ID: " << current->email.id << endl
                     << "Sender: " << current->email.sender << endl
                     << "Receiver: " << current->email.receiver << endl
                     << "Subject: " << current->email.subject << endl
                     << "Body: " << current->email.body << endl
                     << "Timestamp: " << current->email.timestamp << endl
                     << "Priority: " << current->email.priority << endl
                     << "-----------------------------------" << endl;
            }
            current = current->next;
        }
        if (!hasSpam)
            cout << "No spam emails found." << endl;
    }

    void debugDisplayEmails(InboxStack &inboxStack, const string &userEmail)
    {
        Node *current = inboxStack.getTop();
        while (current != nullptr)
        {
            if (current->email.receiver == userEmail)
            {
                cout << "ID: " << current->email.id << " | "
                     << "Sender: " << current->email.sender << " | "
                     << "Receiver: " << current->email.receiver << " | "
                     << "Spam Status: " << current->email.spamStatus << endl;
            }

            current = current->next;
        }
    }

    // Mark an email as spam and save the updated queue
    void markAsSpam(OutboxQueue &outboxQueue, int id, const string &filename, const string &userEmail, InboxStack &inboxStack)
    {
        // Update in OutboxQueue
        EmailNode *currentOutbox = outboxQueue.getFront();
        bool found = false;
        while (currentOutbox != nullptr)
        {
            if (currentOutbox->id == id && currentOutbox->receiver == userEmail)
            {
                currentOutbox->spamStatus = "Yes";
                found = true;
                cout << "Email with ID " << id << " marked as spam." << endl;
                break;
            }
            currentOutbox = currentOutbox->next;
        }

        // Update in InboxStack if found
        if (found)
        {
            Node *currentInbox = inboxStack.getTop();
            while (currentInbox != nullptr)
            {
                if (currentInbox->email.id == id && currentInbox->email.receiver == userEmail)
                {
                    currentInbox->email.spamStatus = "Yes";
                    break;
                }
                currentInbox = currentInbox->next;
            }

            // Save the updated queue to file
            outboxQueue.saveToFile(filename);
        }
        else
        {
            cout << "Email with ID " << id << " not found in outbox." << endl;
        }
    }

    // Unmark an email as spam and save the updated queue
    void unmarkAsSpam(OutboxQueue &outboxQueue, int id, const string &filename, const string &userEmail, InboxStack &inboxStack)
    {
        // Update in OutboxQueue
        EmailNode *currentOutbox = outboxQueue.getFront();
        bool found = false;
        while (currentOutbox != nullptr)
        {
            if (currentOutbox->id == id && currentOutbox->receiver == userEmail)
            {
                currentOutbox->spamStatus = "No";
                found = true;
                cout << "Email with ID " << id << " unmarked as spam." << endl;
                break;
            }
            currentOutbox = currentOutbox->next;
        }

        // Update in InboxStack if found
        if (found)
        {
            Node *currentInbox = inboxStack.getTop();
            while (currentInbox != nullptr)
            {
                if (currentInbox->email.id == id && currentInbox->email.receiver == userEmail)
                {
                    currentInbox->email.spamStatus = "No";
                    // cout << "Email with ID " << id << " unmarked as spam in inbox stack." << endl;
                    break;
                }
                currentInbox = currentInbox->next;
            }

            // Save the updated queue to file
            outboxQueue.saveToFile(filename);
        }
        else
        {
            cout << "Email with ID " << id << " not found in outbox." << endl;
        }
    }
};

#endif // SPAMDETECTION_H