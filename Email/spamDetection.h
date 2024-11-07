/**
 *  spamDetection.h
 */
#include "emailQueue.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

const int MAX_SPAM_WORDS = 100;

class SpamDetection
{
public:
    // Function to load spam word from text file
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

    // Quick Sort function
    void quickSort(string spamWords[], int low, int high)
    {
        if (low < high)
        {
            int pi = partition(spamWords, low, high);
            quickSort(spamWords, low, pi - 1);
            quickSort(spamWords, pi + 1, high);
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

    // Enqueue email from file
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
            string senderEmail, recipientEmail, title, content, dateTime, priorityStr, spamStatus;
            int priority;

            getline(ss, senderEmail, '|');
            getline(ss, recipientEmail, '|');
            getline(ss, title, '|');
            getline(ss, content, '|');
            getline(ss, dateTime, '|');
            getline(ss, priorityStr, '|');
            getline(ss, spamStatus, '|');

            try
            {
                priority = stoi(priorityStr);
            }
            catch (const std::invalid_argument &e)
            {
                cout << "Invalid argument for priority: " << priorityStr << ". Setting priority to 0." << endl;
                priority = 0; // Default value or handle it as per your needs
            }
            catch (const std::out_of_range &e)
            {
                cout << "Priority value out of range: " << priorityStr << ". Setting priority to 0." << endl;
                priority = 0; // Default value or handle it as per your needs
            }

            emailQueue.enqueue(senderEmail, recipientEmail, title, content, dateTime, priority, spamStatus);
        }
        emailFile.close();
    }

    // Update spam status for all emails
    void SpamDetection::checkSpamStatus(EmailQueue &emailQueue, string spamWords[], int spamWordCount)
    {
        EmailNode *current = emailQueue.getFront();
        while (current != nullptr)
        {
            checkSingleEmail(current, spamWords, spamWordCount);
            current = current->next;
        }
    }

    // Check spam for a single email
    void SpamDetection::checkSingleEmail(EmailNode *email, string spamWords[], int spamWordCount)
    {
        stringstream contentStream(email->content);
        string word;
        bool isSpam = false;
        while (contentStream >> word)
        {
            if (binarySearch(word, spamWords, spamWordCount))
            {
                isSpam = true;
                break;
            }
        }
        email->spamStatus = isSpam ? "Yes" : "No";
        email->priority = isSpam ? 0 : email->priority;
    }

    // Function to check a new email message for spam and append it to the queue
    void SpamDetection::checkAndAppendMessage(const string &sender, const string &recipient, const string &title,
                                              const string &content, const string &dateTime, EmailQueue &emailQueue,
                                              string spamWords[], int spamWordCount)
    {
        int priority = 1; // Default priority for new message
        string spamStatus = "No";

        EmailNode tempEmail{sender, recipient, title, content, dateTime, priority, spamStatus, nullptr};

        checkSingleEmail(&tempEmail, spamWords, spamWordCount);

        // Enqueue with updated priority and spam status
        emailQueue.enqueue(sender, recipient, title, content, dateTime, tempEmail.priority, tempEmail.spamStatus);
    }
};
