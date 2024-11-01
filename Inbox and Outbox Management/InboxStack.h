#ifndef INBOXSTACK_H
#define INBOXSTACK_H

#include <iostream>
#include <string>
#include <stdexcept>

struct Email {
    int id;
    std::string sender;
    std::string receiver;
    std::string subject;
    std::string body;
    std::string timestamp;
    std::string status;
    int priority;
    std::string spamStatus;
};

// Node structure for the linked list
struct Node {
    Email email;
    Node* next; // Pointer to the next node

    // Constructor for Node
    Node(const Email& emailData, Node* nextNode = nullptr)
        : email(emailData), next(nextNode) {}
};

// InboxStack class using a linked list
class InboxStack {
private:
    Node* top; // Pointer to the top of the stack

public:
    InboxStack();
    ~InboxStack();

    void push(const Email& email);
    Email pop();
    bool isEmpty() const;
    void displayTop() const;
};

// Constructor
InboxStack::InboxStack() : top(nullptr) {}

// Destructor
InboxStack::~InboxStack() {
    while (!isEmpty()) {
        pop();
    }
}

// Push method
void InboxStack::push(const Email& email) {
    Node* newNode = new Node(email, top); // Create new node using constructor
    top = newNode; // Update top to the new node
}

// Pop method
Email InboxStack::pop() {
    if (isEmpty()) {
        throw std::runtime_error("Stack is empty. Cannot pop.");
    }

    Node* temp = top; // Store the current top node
    Email email = top->email; // Get the email from the top node
    top = top->next; // Move the top pointer to the next node
    delete temp; // Free memory
    return email; // Return the email
}

// Check if the stack is empty
bool InboxStack::isEmpty() const {
    return top == nullptr;
}

// Display the top email
void InboxStack::displayTop() const {
    if (isEmpty()) {
        std::cout << "No emails in the inbox." << std::endl;
        return;
    }
    const Email& topEmail = top->email; // Reference to the top email
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