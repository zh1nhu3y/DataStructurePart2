#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

// Node structure for each email in the queue
struct EmailNode
{
    int id;
    string senderEmail;
    string recipientEmail;
    string title;
    string content;
    string dateTime;
    string status;
    int priority;
    string spamStatus;
    EmailNode *next;
};

// Class for managing a queue of emails
class EmailQueue
{
    EmailNode *front; // Pointer to the front of the queue
    EmailNode *rear;  // Pointer to the rear of the queue

public:
    EmailQueue()
    {
        front = nullptr;
        rear = nullptr;
    }

    // Enqueue function to add a new email to the rear of the queue
    void enqueue(int id, const string &sender, const string &recipient, const string &title, const string &content,
                 const string &dateTime, const string &status, int priority, const string &spamStatus)
    {
        EmailNode *newNode = new EmailNode{id, sender, recipient, title, content, dateTime, status, priority, spamStatus, nullptr};
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

    // Dequeue function to remove the front email from the queue
    void dequeue()
    {
        if (front == nullptr)
        {
            cout << "Queue is underflow!" << endl;
            return;
        }
        EmailNode *temp = front;
        front = (front == rear) ? nullptr : front->next;
        if (!front)
            rear = nullptr;
        delete temp;
    }

    // Function to check if the queue is empty
    bool isEmpty()
    {
        return front == nullptr;
    }

    // Getter for the front of the queue
    EmailNode *getFront()
    {
        return front;
    }

    // Function to save all emails in the queue to a file, overwriting the file's content
    void saveToFile(const string &filename)
    {
        ofstream outFile(filename);
        if (!outFile.is_open())
        {
            cout << "Error opening file for saving." << endl;
            return;
        }

        // Traverse the queue and write each email's details to the file
        EmailNode *current = front;
        while (current != nullptr)
        {
            outFile << current->id << "|"
                    << current->senderEmail << "|"
                    << current->recipientEmail << "|"
                    << current->title << "|"
                    << current->content << "|"
                    << current->dateTime << "|"
                    << current->status << "|"
                    << current->priority << "|"
                    << current->spamStatus << endl;

            current = current->next;
        }
        outFile.close();
        cout << "Changes saved to file successfully." << endl;
    }

    ~EmailQueue()
    {
        while (!isEmpty())
        {
            dequeue();
        }
    }
};