/**
 *  spamDetection.h
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

const int MAX_SPAM_WORDS = 100;

// Node structure for email
struct EmailNode
{
    string senderEmail;
    string recipientEmail;
    string title;
    string content;
    string dateTime;
    int priority;
    string spamStatus;

    EmailNode *next;
};

class EmailQueue
{
    EmailNode *front;
    EmailNode *rear;

public:
    EmailQueue()
    {
        front = nullptr;
        rear = nullptr;
    }

    // Enqueue a new email (add to rear)
    void enqueue(const string &sender, const string &recipient, const string &title, const string &content,
                 const string &dateTime, int priority, const string &spamStatus)
    {
        EmailNode *newNode = new EmailNode();
        newNode->senderEmail = sender;
        newNode->recipientEmail = recipient;
        newNode->title = title;
        newNode->content = content;
        newNode->dateTime = dateTime;
        newNode->priority = priority;
        newNode->spamStatus = spamStatus;

        newNode->next = nullptr;

        if (isEmpty())
        {
            front = rear = newNode;
        }
        else
        {
            rear->next = newNode;
            rear = newNode;
        }
    }

    // Dequeue an email (remove from front)
    void dequeue()
    {
        if (front == nullptr)
        {
            cout << "Queue is underflow!" << endl;
            return;
        }

        EmailNode *temp = front;

        if (front == rear)
        {
            front = rear = nullptr;
        }
        else
        {
            front = front->next;
        }

        delete temp;
    }

    bool isEmpty()
    {
        return (front == nullptr) && (rear == nullptr);
    }

    EmailNode *getFront()
    {
        return front;
    }

    // Function to print all emails to the terminal
    void printEmails()
    {
        EmailNode *current = front;
        cout << "Processed Emails:" << endl;
        while (current != nullptr)
        {
            cout << "Sender: " << current->senderEmail << endl
                 << "Recipient: " << current->recipientEmail << endl
                 << "Title: " << current->title << endl
                 << "Content: " << current->content << endl
                 << "Date: " << current->dateTime << endl
                 << "Priority: " << current->priority << endl
                 << "Spam Status: " << current->spamStatus << endl
                 << "-----------------------------------" << endl;
            current = current->next;
        }
    }

    ~EmailQueue()
    {
        while (!isEmpty())
        {
            dequeue();
        }
    }
};

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

    void checkSpamStatus(EmailQueue &emailQueue, string spamWords[], int spamWordCount)
    {
        cout << "Updating Spam Status for Emails" << endl;

        EmailNode *current = emailQueue.getFront();

        while (current != nullptr)
        {
            // Check if the email content contains any spam words
            bool isSpam = false;

            // Split the content into words to check for spam
            stringstream contentStream(current->content);
            string word;

            while (contentStream >> word)
            {
                if (binarySearch(word, spamWords, spamWordCount))
                {
                    isSpam = true;
                    break;
                }
            }

            // Update the spam status based on the check
            if (isSpam)
            {
                current->spamStatus = "Yes";
            }
            else
            {
                current->spamStatus = "No";
            }

            current = current->next;
        }
    }
};
