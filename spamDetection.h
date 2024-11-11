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
        cout << "Loading Spam Words File" << endl;
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

    // Enqueue all emails from file into the OutboxQueue
    void enqueueEmail(const string &filename, OutboxQueue &outboxQueue)
    {
        cout << "Loading Emails from File." << endl;
        ifstream emailFile(filename);
        string line;

        if (!emailFile.is_open())
        {
            cout << "Error opening email file." << endl;
            return;
        }

        while (getline(emailFile, line))
        {
            stringstream ss(line);
            string idStr, sender, receiver, subject, body, timestamp, status, priorityStr, spamStatus;
            int id, priority;

            getline(ss, idStr, '|');
            getline(ss, sender, '|');
            getline(ss, receiver, '|');
            getline(ss, subject, '|');
            getline(ss, body, '|');
            getline(ss, timestamp, '|');
            getline(ss, status, '|');
            getline(ss, priorityStr, '|');
            getline(ss, spamStatus, '|');

            try
            {
                id = stoi(idStr);
                priority = stoi(priorityStr);
            }
            catch (const std::invalid_argument &e)
            {
                cout << "Invalid ID or priority. Setting to 0." << endl;
                id = 0;
                priority = 0;
            }

            // Create Email object with all parameters including spamStatus
            Email email(id, sender, receiver, subject, body, timestamp, status,
                        priority, spamStatus.empty() ? "No" : spamStatus);
            outboxQueue.enqueue(email);
        }
        emailFile.close();
    }

    void checkAllEmailsInOutbox(OutboxQueue &outboxQueue, string spamWords[], int spamWordCount, const string &emailFile)
    {
        // First, clear the existing queue
        while (!outboxQueue.isEmpty())
        {
            outboxQueue.dequeue();
        }

        // Load emails and check each for spam
        cout << "Checking emails for spam content..." << endl;

        // Load all emails into the queue
        enqueueEmail(emailFile, outboxQueue);

        // Create a temporary queue to store processed emails
        OutboxQueue tempQueue;
        int spamCount = 0;
        int nonSpamCount = 0;

        // Process each email in the queue
        while (!outboxQueue.isEmpty())
        {
            EmailNode *currentEmail = outboxQueue.getFront();
            if (currentEmail != nullptr)
            {
                // Create a copy of the current email
                EmailNode *emailCopy = new EmailNode(
                    currentEmail->id,
                    currentEmail->sender,
                    currentEmail->receiver,
                    currentEmail->subject,
                    currentEmail->body,
                    currentEmail->timestamp,
                    currentEmail->status,
                    currentEmail->priority,
                    currentEmail->spamStatus);

                // Check this email for spam and add to temp queue
                checkSingleEmailAndAppend(emailCopy, spamWords, spamWordCount, tempQueue, emailFile);

                // Count the spam and non-spam emails
                if (emailCopy->spamStatus == "Yes")
                {
                    spamCount++;
                }
                else
                {
                    nonSpamCount++;
                }

                delete emailCopy;
            }
            outboxQueue.dequeue();
        }

        // Move all emails back to the original queue
        while (!tempQueue.isEmpty())
        {
            EmailNode *front = tempQueue.getFront();
            if (front != nullptr)
            {
                Email email(front->id, front->sender, front->receiver, front->subject,
                            front->body, front->timestamp, front->status, front->priority,
                            front->spamStatus);
                outboxQueue.enqueue(email);
            }
            tempQueue.dequeue();
        }

        // Print a summary after processing all emails
        cout << "Finished checking all emails for spam content." << endl;
        cout << "Total spam emails: " << spamCount << endl;
        cout << "Total non-spam emails: " << nonSpamCount << endl;
    }

    // Check if a single email is spam and add it to OutboxQueue
    void checkSingleEmailAndAppend(EmailNode *email, string spamWords[], int spamWordCount,
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
                break;
            }
        }
        email->spamStatus = isSpam ? "Yes" : "No";

        Email emailToAdd(email->id, email->sender, email->receiver, email->subject,
                         email->body, email->timestamp, email->status, email->priority,
                         isSpam ? "Yes" : "No");

        outboxQueue.enqueue(emailToAdd);

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
    void displaySpamEmails(OutboxQueue &outboxQueue)
    {
        EmailNode *current = outboxQueue.getFront();
        cout << "Spam Emails:" << endl;
        bool hasSpam = false;
        while (current != nullptr)
        {
            if (current->spamStatus == "Yes")
            {
                hasSpam = true;
                cout << "ID: " << current->id << endl
                     << "Sender: " << current->sender << endl
                     << "Receiver: " << current->receiver << endl
                     << "Subject: " << current->subject << endl
                     << "Body: " << current->body << endl
                     << "Timestamp: " << current->timestamp << endl
                     << "Priority: " << current->priority << endl
                     << "-----------------------------------" << endl;
            }
            current = current->next;
        }
        if (!hasSpam)
            cout << "No spam emails found." << endl;
    }

    // Mark an email as spam and save the updated queue
    void markAsSpam(OutboxQueue &outboxQueue, int id, const string &filename)
    {
        EmailNode *current = outboxQueue.getFront();
        while (current != nullptr)
        {
            if (current->id == id)
            {
                current->spamStatus = "Yes";
                cout << "Email with ID " << id << " marked as spam." << endl;
                outboxQueue.saveToFile(filename);
                return;
            }
            current = current->next;
        }
        cout << "Email with ID " << id << " not found." << endl;
    }

    // Unmark an email as spam and save the updated queue
    void unmarkAsSpam(OutboxQueue &outboxQueue, int id, const string &filename)
    {
        EmailNode *current = outboxQueue.getFront();
        while (current != nullptr)
        {
            if (current->id == id)
            {
                current->spamStatus = "No";
                cout << "Email with ID " << id << " unmarked as spam." << endl;
                outboxQueue.saveToFile(filename);
                return;
            }
            current = current->next;
        }
        cout << "Email with ID " << id << " not found." << endl;
    }
};

#endif // SPAMDETECTION_H