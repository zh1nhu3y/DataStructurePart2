#ifndef OUTBOXQUEUE_H
#define OUTBOXQUEUE_H

#include <iostream>
#include <stdexcept>
#include <fstream>
#include "InboxStack.h" // Include InboxStack for Email struct

// Node structure for the linked list of emails within one QueueNode
struct EmailNode {
    int id;
    std::string sender;
    std::string receiver;
    std::string subject;
    std::string body;
    std::string timestamp;
    std::string status;
    int priority;
    std::string spamStatus;
    EmailNode* next;

    // Modified constructor to match Email constructor and include spamStatus
    EmailNode(int emailId, const std::string& senderEmail, const std::string& recipientEmail,
              const std::string& emailSubject, const std::string& emailBody, 
              const std::string& emailTimestamp, const std::string& emailStatus, 
              int emailPriority, const std::string& emailSpamStatus = "No")
        : id(emailId), sender(senderEmail), receiver(recipientEmail), 
          subject(emailSubject), body(emailBody), timestamp(emailTimestamp), 
          status(emailStatus), priority(emailPriority), 
          spamStatus(emailSpamStatus), next(nullptr) {}
};

// Node structure for the queue linked list (only one node to store all emails)
struct QueueNode {
    EmailNode* emailHead; // Head of linked list for all emails
    QueueNode* next;      // Pointer to next QueueNode

    // Constructor for QueueNode
    QueueNode() : emailHead(nullptr), next(nullptr) {}

    // Function to add an email to this QueueNode's email linked list
    void addEmail(const Email& email) {
        EmailNode* newNode = new EmailNode(email.id, email.sender, email.receiver, 
                                        email.subject, email.body, email.timestamp, 
                                        email.status, email.priority, email.spamStatus);
        if (!emailHead) {
            emailHead = newNode;
        } else {
            EmailNode* temp = emailHead;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }
};

// OutboxQueue class using a single QueueNode
class OutboxQueue {
private:
    QueueNode* front; // Front of the queue (first QueueNode)

public:
    OutboxQueue();
    ~OutboxQueue();

    void enqueue(const Email& email);
    Email dequeue();
    bool isEmpty() const;
    void displayFront() const;

    void saveToFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cout << "Error: Unable to open file for writing." << std::endl;
            return;
        }

        EmailNode* current = front->emailHead;
        while (current != nullptr) {
            file << current->id << "|"
                 << current->sender << "|"
                 << current->receiver << "|"
                 << current->subject << "|"
                 << current->body << "|"
                 << current->timestamp << "|"
                 << current->status << "|"
                 << current->priority << "|"
                 << current->spamStatus << "\n";
            current = current->next;
        }
        file.close();
        std::cout << "Queue saved to file successfully!" << std::endl;
    }

    // Method to get the first email node (to be used by SpamDetection)
    EmailNode* getFront() const {
        return front->emailHead;  // Returns the first email node
    }
};

// Constructor
OutboxQueue::OutboxQueue() : front(new QueueNode) {}

// Destructor
OutboxQueue::~OutboxQueue() {
    while (!isEmpty()) {
        dequeue();
    }
    delete front;
}

// Enqueue method
void OutboxQueue::enqueue(const Email& email) {
    front->addEmail(email);
}

// Dequeue method
Email OutboxQueue::dequeue() {
    if (isEmpty()) {
        throw std::runtime_error("Queue is empty. Cannot dequeue.");
    }

    EmailNode* temp = front->emailHead;
    // Include spamStatus in the email initialization
    Email email (temp->id, temp->sender, temp->receiver, temp->subject, 
                  temp->body, temp->timestamp, temp->status, temp->priority, 
                  temp->spamStatus);  // Add spamStatus here
    front->emailHead = temp->next;
    delete temp;
    return email;
}

// Check if the queue is empty
bool OutboxQueue::isEmpty() const {
    return front->emailHead == nullptr;
}

// Display the front email (only the first email in the list)
void OutboxQueue::displayFront() const {
    if (isEmpty()) {
        std::cout << "No emails in the outbox." << std::endl;
        return;
    }

    // Access the first email node (no need to access a non-existent 'email' member)
    EmailNode* frontEmailNode = front->emailHead; // Reference to the first email node in the queue
    std::cout << "Front Email:\n"
              << "ID: " << frontEmailNode->id << "\n"
              << "Sender: " << frontEmailNode->sender << "\n"
              << "Receiver: " << frontEmailNode->receiver << "\n"
              << "Subject: " << frontEmailNode->subject << "\n"
              << "Body: " << frontEmailNode->body << "\n"
              << "Timestamp: " << frontEmailNode->timestamp << "\n"
              << "Status: " << frontEmailNode->status << "\n"
              << "Priority: " << frontEmailNode->priority << "\n"
              << "Spam Status: " << frontEmailNode->spamStatus << std::endl;
}

#endif // OUTBOXQUEUE_H
