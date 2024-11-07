#include <iostream>
#include <string>
using namespace std;

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

    void enqueue(const string &sender, const string &recipient, const string &title, const string &content,
                 const string &dateTime, int priority, const string &spamStatus)
    {
        EmailNode *newNode = new EmailNode{sender, recipient, title, content, dateTime, priority, spamStatus, nullptr};
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