/**
 *  spamDetection.h
 */
#include "emailQueue.h"
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
            // Read each word and store it in the array
            while (file >> word && count < maxWords)
            {
                spamWords[count++] = word;
            }
            file.close();
        }
        else
        {
            cout << "Error to open file." << endl;
        }
        return count;
    }

    // Function to swap two elements
    void swap(string &a, string &b)
    {
        string temp = a;
        a = b;
        b = temp;
    }

    // Partition function for quick sort
    int partition(string spamWords[], int low, int high)
    {
        string pivot = spamWords[high]; // Select the last element as pivot
        int i = low - 1;
        for (int j = low; j < high; j++)
        {
            // Place smaller elements to the left of the pivot
            if (spamWords[j] < pivot)
            {
                i++;
                swap(spamWords[i], spamWords[j]);
            }
        }
        swap(spamWords[i + 1], spamWords[high]); // Place pivot in correct position
        return i + 1;
    }

    // Quick Sort function to sort spam words file
    void quickSort(string spamWords[], int low, int high)
    {
        if (low < high)
        {
            int pi = partition(spamWords, low, high);
            quickSort(spamWords, low, pi - 1);  // Sort elements before pivot
            quickSort(spamWords, pi + 1, high); // Sort elements after pivot
        }
    }

    // Binary search function to check if a word exists in the spam words list
    bool binarySearch(const string &word, string spamWords[], int n)
    {
        int left = 0;
        int right = n - 1;

        while (left <= right)
        {
            int mid = left + (right - left) / 2;
            if (spamWords[mid] == word)
            {
                return true; // Spam word found
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
        return false; // Spam word not found
    }

    // Enqueue all email from file
    void enqueueEmail(const string &filename, EmailQueue &emailQueue)
    {
        cout << "Loading Emails from File." << endl;
        ifstream emailFile(filename);
        string line;

        if (!emailFile.is_open())
        {
            cout << "Error opening file." << endl;
            return;
        }

        while (getline(emailFile, line))
        {
            stringstream ss(line);
            string idStr, senderEmail, recipientEmail, title, content, dateTime, status, priorityStr, spamStatus;
            int id, priority;

            // Parse each field based on the new file format
            getline(ss, idStr, '|');
            getline(ss, senderEmail, '|');
            getline(ss, recipientEmail, '|');
            getline(ss, title, '|');
            getline(ss, content, '|');
            getline(ss, dateTime, '|');
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
                cout << "Invalid argument for id or priority. Setting id and priority to 0." << endl;
                id = 0;
                priority = 0;
            }

            emailQueue.enqueue(id, senderEmail, recipientEmail, title, content, dateTime, status, priority, spamStatus);
        }
        emailFile.close();
    }

    // WILL REMOVE LATER: Update spam status for all emails (ONLY USED ONCE FOR DUMMY DATA)
    // void checkSpamStatus(EmailQueue &emailQueue, string spamWords[], int spamWordCount, const string &filename)
    // {
    //     EmailNode *current = emailQueue.getFront();
    //     while (current != nullptr)
    //     {
    //         checkSingleEmail(current, spamWords, spamWordCount);
    //         current = current->next;
    //     }
    //     emailQueue.saveToFile(filename);
    // }

    // void checkSingleEmail(EmailNode *email, string spamWords[], int spamWordCount)
    // {
    //     stringstream contentStream(email->content);
    //     string word;
    //     bool isSpam = false;
    //     while (contentStream >> word)
    //     {
    //         if (binarySearch(word, spamWords, spamWordCount))
    //         {
    //             isSpam = true;
    //             break;
    //         }
    //     }
    //     email->spamStatus = isSpam ? "Yes" : "No";
    // }

    // Check spam for a single email
    // THIS PART TO INTEGRATE
    // 1. add all outbox data in one node
    // 2. pass the node and queue to this function
    void checkSingleEmailAndAppend(EmailNode *email, string spamWords[], int spamWordCount, EmailQueue &emailQueue, const string &emailFile)
    {
        stringstream contentStream(email->content);
        string word;
        bool isSpam = false;
        while (contentStream >> word)
        {
            // Convert word to lowercase
            transform(word.begin(), word.end(), word.begin(), ::tolower);

            // Remove punctuation from the word
            word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());

            if (binarySearch(word, spamWords, spamWordCount))
            {
                isSpam = true;
                break;
            }
        }
        email->spamStatus = isSpam ? "Yes" : "No"; // Set spam status

        // Enqueue the email
        emailQueue.enqueue(email->id, email->senderEmail, email->recipientEmail, email->title,
                           email->content, email->dateTime, email->status, email->priority, email->spamStatus);

        // Append the email to the file
        ofstream file(emailFile, ios::app); // Open the file in append mode
        if (file.is_open())
        {
            file << email->id << "|" << email->senderEmail << "|" << email->recipientEmail << "|" << email->title << "|"
                 << email->content << "|" << email->dateTime << "|" << email->status << "|"
                 << email->priority << "|" << email->spamStatus << "\n";
            file.close();
            cout << "Email has been sent, checked for spam, and saved to file successfully!" << endl;
        }
        else
        {
            cout << "Error: Unable to open file for writing." << endl;
        }
    }

    // Function to display all spam emails
    void displaySpamEmails(EmailQueue &emailQueue)
    {
        EmailNode *current = emailQueue.getFront();
        cout << "Spam Emails:" << endl;
        bool hasSpam = false;
        while (current != nullptr)
        {
            if (current->spamStatus == "Yes")
            {
                hasSpam = true;
                cout << "ID: " << current->id << endl
                     << "Sender: " << current->senderEmail << endl
                     << "Recipient: " << current->recipientEmail << endl
                     << "Title: " << current->title << endl
                     << "Content: " << current->content << endl
                     << "Date: " << current->dateTime << endl
                     << "Priority: " << current->priority << endl
                     << "-----------------------------------" << endl;
            }
            current = current->next;
        }
        if (!hasSpam)
            cout << "No spam emails found." << endl;
    }

    // Function to mark an email as spam and saves the updated queue to file
    void markAsSpam(EmailQueue &emailQueue, int id, const string &filename)
    {
        EmailNode *current = emailQueue.getFront();
        while (current != nullptr)
        {
            if (current->id == id)
            {
                current->spamStatus = "Yes"; // Mark as spam
                cout << "Email with ID " << id << " has been marked as spam." << endl;
                emailQueue.saveToFile(filename); // Save after marking as spam
                return;
            }
            current = current->next;
        }
        cout << "Email with ID " << id << " not found." << endl;
    }

    // Function to unmark an email as spam and saves the updated queue to file
    void unmarkAsSpam(EmailQueue &emailQueue, int id, const string &filename)
    {
        EmailNode *current = emailQueue.getFront();
        while (current != nullptr)
        {
            if (current->id == id)
            {
                current->spamStatus = "No"; // Unmark as spam
                cout << "Email with ID " << id << " has been unmarked as spam." << endl;
                emailQueue.saveToFile(filename); // Save after unmarking as spam
                return;
            }
            current = current->next;
        }
        cout << "Email with ID " << id << " not found." << endl;
    }
};
