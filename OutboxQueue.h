#ifndef OUTBOXQUEUE_H
#define OUTBOXQUEUE_H

#include <iostream>
#include <stdexcept>
#include "InboxStack.h" // Include InboxStack for Email struct

// Node structure for the queue linked list
struct QueueNode {
    Email email;
    QueueNode* next; // Pointer to the next node

    // Constructor for QueueNode
    QueueNode(const Email& emailData, QueueNode* nextNode = nullptr)
        : email(emailData), next(nextNode) {}
};

// OutboxQueue class using a linked list
class OutboxQueue {
private:
    QueueNode* front; // Pointer to the front of the queue
    QueueNode* rear;  // Pointer to the rear of the queue

public:
    OutboxQueue();
    ~OutboxQueue();

    void enqueue(const Email& email);
    Email dequeue();
    bool isEmpty() const;
    void displayFront() const;
};

// Constructor
OutboxQueue::OutboxQueue() : front(nullptr), rear(nullptr) {}

// Destructor
OutboxQueue::~OutboxQueue() {
    while (!isEmpty()) {
        dequeue();
    }
}

// Enqueue method
void OutboxQueue::enqueue(const Email& email) {
    QueueNode* newNode = new QueueNode(email); // Create new node
    if (isEmpty()) {
        front = rear = newNode; // First element in queue
    } else {
        rear->next = newNode; // Link new node at the end
        rear = newNode; // Update rear to new node
    }
}

// Dequeue method
Email OutboxQueue::dequeue() {
    if (isEmpty()) {
        throw std::runtime_error("Queue is empty. Cannot dequeue.");
    }

    QueueNode* temp = front; // Store the current front node
    Email email = front->email; // Get the email from the front node
    front = front->next; // Move front pointer to the next node

    if (front == nullptr) {
        rear = nullptr; // Queue is now empty
    }

    delete temp; // Free memory
    return email; // Return the email
}

// Check if the queue is empty
bool OutboxQueue::isEmpty() const {
    return front == nullptr;
}

// Display the front email
void OutboxQueue::displayFront() const {
    if (isEmpty()) {
        std::cout << "No emails in the outbox." << std::endl;
        return;
    }
    const Email& frontEmail = front->email; // Reference to the front email
    std::cout << "Front Email:\n"
              << "ID: " << frontEmail.id << "\n"
              << "Sender: " << frontEmail.sender << "\n"
              << "Receiver: " << frontEmail.receiver << "\n"
              << "Subject: " << frontEmail.subject << "\n"
              << "Body: " << frontEmail.body << "\n"
              << "Timestamp: " << frontEmail.timestamp << "\n"
              << "Status: " << frontEmail.status << "\n"
              << "Priority: " << frontEmail.priority << "\n"
              << "Spam Status: " << frontEmail.spamStatus << std::endl;
}

#endif // OUTBOXQUEUE_H