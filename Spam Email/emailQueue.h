#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

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

    bool isEmpty()
    {
        return front == nullptr;
    }

    EmailNode *getFront()
    {
        return front;
    }

    void saveToFile(const string &filename)
    {
        ofstream outFile(filename);
        if (!outFile.is_open())
        {
            cout << "Error opening file for saving." << endl;
            return;
        }

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

    // Append to file for a new outbox email
    void appendToFile(const string &filename)
    {
        ofstream of;
        fstream f;
        }

    ~EmailQueue()
    {
        while (!isEmpty())
        {
            dequeue();
        }
    }
};