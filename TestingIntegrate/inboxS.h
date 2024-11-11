#ifndef INBOXSTACK_H
#define INBOXSTACK_H

#include <iostream>
#include <string>
#include <stdexcept>

struct Email
{
    int id;
    std::string sender;
    std::string receiver;
    std::string subject;
    std::string body;
    std::string timestamp;
    std::string status;
    int priority;
    std::string spamStatus;

    // Constructor
    Email(int _id, const std::string &_sender, const std::string &_receiver,
          const std::string &_subject, const std::string &_body,
          const std::string &_timestamp, const std::string &_status,
          int _priority, const std::string &_spamStatus = "No")
        : id(_id), sender(_sender), receiver(_receiver), subject(_subject),
          body(_body), timestamp(_timestamp), status(_status),
          priority(_priority), spamStatus(_spamStatus) {}

    // Default constructor
    Email() : id(0), priority(0), spamStatus("No") {}
};

// Node structure for the linked list
struct Node
{
    Email email;
    Node *next;

    // Constructor for Node
    Node(const Email &emailData, Node *nextNode = nullptr)
        : email(emailData), next(nextNode) {}
};

// InboxStack class using a linked list
class InboxStack
{
private:
    Node *top; // Pointer to the top of the stack

public:
    // Constructor
    InboxStack() : top(nullptr) {}

    // Destructor
    ~InboxStack();

    void push(const Email &email);
    Email pop();
    bool isEmpty() const;
    void displayTop() const;

    // Function to get the top email without popping it
    Node *getTop() const
    {
        return top;
    }
};

// Destructor implementation
inline InboxStack::~InboxStack()
{
    while (!isEmpty())
    {
        pop();
    }
}

// Push method implementation
inline void InboxStack::push(const Email &email)
{
    Node *newNode = new Node(email, top);
    top = newNode;
}

// Pop method implementation
inline Email InboxStack::pop()
{
    if (isEmpty())
    {
        throw std::runtime_error("Stack is empty. Cannot pop.");
    }
    Node *temp = top;
    Email email = top->email;
    top = top->next;
    delete temp;
    return email;
}

// Check if the stack is empty
inline bool InboxStack::isEmpty() const
{
    return top == nullptr;
}

// Display the top email
inline void InboxStack::displayTop() const
{
    if (isEmpty())
    {
        std::cout << "No emails in the inbox." << std::endl;
        return;
    }
    const Email &topEmail = top->email;
    std::cout << "Top Email:\n"
              << "ID: " << topEmail.id << "\n"
              << "Sender: " << topEmail.sender << "\n"
              << "Receiver: " << topEmail.receiver << "\n"
              << "Subject: " << topEmail.subject << "\n"
              << "Body: " << topEmail.body << "\n"
              << "Timestamp: " << topEmail.timestamp << "\n"
              << "Status: " << topEmail.status << "\n"
              << "Priority: " << topEmail.priority << "\n"
              << "Spam Status: " << topEmail.spamStatus << std::endl;
}

#endif // INBOXSTACK_H