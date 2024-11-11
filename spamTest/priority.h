#include <iostream>
#include <fstream>
#include <sstream> // Needed for std::istringstream
#include "inboxStack.h"
#include "outboxQueue.h"

// Display email by priority
void displayEmailsByPriority(InboxStack& inbox, OutboxQueue& outbox, const std::string& currentUser) {
    InboxStack tempStack;
    OutboxQueue tempQueue;

    // Linked list or array to store all emails from inbox and outbox, ensuring uniqueness
    Email emailList[100];  
    int emailCount = 0;

    // Function to check if the email is already in the list (using email ID)
    auto isDuplicate = [&](int emailID) {
        for (int i = 0; i < emailCount; ++i) {
            if (emailList[i].id == emailID) return true;
        }
        return false;
    };

    // Collect emails from inbox
    while (!inbox.isEmpty()) {
        Email email = inbox.pop();
        if ((email.receiver == currentUser || email.sender == currentUser) && !isDuplicate(email.id)) {
            emailList[emailCount++] = email;  // Store unique email in the list
        }
        tempStack.push(email);  // Store email back to tempStack
    }

    // Collect emails from outbox
    while (!outbox.isEmpty()) {
        Email email = outbox.dequeue();
        if ((email.receiver == currentUser || email.sender == currentUser) && !isDuplicate(email.id)) {
            emailList[emailCount++] = email;  // Store unique email in the list
        }
        tempQueue.enqueue(email);  // Store email back to tempQueue
    }

    // Sort emails by priority (1 is highest, 5 is lowest)
    for (int i = 0; i < emailCount - 1; i++) {
        for (int j = i + 1; j < emailCount; j++) {
            if (emailList[i].priority > emailList[j].priority) {  // Swap if higher priority comes after lower
                std::swap(emailList[i], emailList[j]);
            }
        }
    }

    // Display emails by priority (now sorted from highest to lowest priority)
    for (int i = 0; i < emailCount; i++) {
        std::cout << "ID: " << emailList[i].id << "\nSender: " << emailList[i].sender
                  << "\nReceiver: " << emailList[i].receiver << "\nSubject: " << emailList[i].subject
                  << "\nBody: " << emailList[i].body << "\nTimestamp: " << emailList[i].timestamp
                  << "\nStatus: " << emailList[i].status << "\nPriority: " << emailList[i].priority
                  << "\nSpam Status: " << emailList[i].spamStatus << "\n\n";
    }

    // Restore inbox and outbox
    while (!tempStack.isEmpty()) {
        inbox.push(tempStack.pop());
    }

    while (!tempQueue.isEmpty()) {
        outbox.enqueue(tempQueue.dequeue());
    }
}

void editEmailPriority(InboxStack& inbox, OutboxQueue& outbox, const string& emailFile, int emailID, int newPriority) {
    InboxStack tempStack;
    OutboxQueue tempQueue;
    bool found = false;

    // Edit inbox emails by priority
    while (!inbox.isEmpty()) {
        Email email = inbox.pop();
        if (email.id == emailID) {
            email.priority = newPriority;
            found = true;
        }
        tempStack.push(email);
    }

    // Push remaining inbox emails back to the stack
    while (!tempStack.isEmpty()) {
        inbox.push(tempStack.pop());
    }

    // Edit outbox emails by priority
    while (!outbox.isEmpty()) {
        Email email = outbox.dequeue();
        if (email.id == emailID) {
            email.priority = newPriority;
            found = true;
        }
        tempQueue.enqueue(email);
    }

    // Push remaining outbox emails back to the queue
    while (!tempQueue.isEmpty()) {
        outbox.enqueue(tempQueue.dequeue());
    }

    if (found) {
        std::cout << "Email priority updated successfully.\n";
        // Now update the file to reflect the new priority
        updateEmailFile(emailFile, emailID, newPriority);
    } else {
        std::cout << "Email not found.\n";
    }
}

void updateEmailFile(const string& emailFile, int emailID, int newPriority) {
    std::ifstream emailFileIn(emailFile);
    std::ofstream tempFile("temp.txt");

    if (!emailFileIn.is_open() || !tempFile.is_open()) {
        std::cerr << "Failed to open email file or temporary file for update.\n";
        return;
    }

    std::string line;
    bool emailUpdated = false;

    while (getline(emailFileIn, line)) {
        std::stringstream ss(line);
        int id;
        ss >> id;

        if (id == emailID) {
            // If email is found, update its priority
            std::string sender, receiver, subject, body, timestamp, status, spamStatus;
            ss.ignore();
            getline(ss, sender, '|');
            getline(ss, receiver, '|');
            getline(ss, subject, '|');
            getline(ss, body, '|');
            getline(ss, timestamp, '|');
            getline(ss, status, '|');
            getline(ss, spamStatus, '|');

            // Write the updated email with new priority
            tempFile << id << "|" << sender << "|" << receiver << "|"
                     << subject << "|" << body << "|" << timestamp << "|"
                     << status << "|" << newPriority << "|" << spamStatus << "\n";
            emailUpdated = true;
        } else {
            tempFile << line << "\n";  // Keep the email as it is
        }
    }

    emailFileIn.close();
    tempFile.close();

    // Replace the original email file with the updated file
    if (emailUpdated) {
        remove(emailFile.c_str());  // Remove the original file
        rename("temp.txt", emailFile.c_str());  // Rename temp file to the original file
        std::cout << "Email priority saved to file.\n";
    } else {
        std::cout << "Email ID not found in the file.\n";
    }
}

void searchEmailsByPriority(InboxStack& inbox, OutboxQueue& outbox, int searchPriority, const std::string& currentUser) {
    InboxStack tempStack;
    OutboxQueue tempQueue;
    bool found = false;
    std::set<int> seenEmails;  // Set to keep track of email IDs we've already printed

    std::cout << "Searching Emails with Priority " << searchPriority << ":\n";

    // Search inbox
    while (!inbox.isEmpty()) {
        Email email = inbox.pop();
        if (email.priority == searchPriority && (email.receiver == currentUser || email.sender == currentUser)) {
            // Check if this email has already been printed (based on email ID)
            if (seenEmails.find(email.id) == seenEmails.end()) {
                std::cout << "ID: " << email.id << " | From: " << email.sender
                          << " | To: " << email.receiver << " | Subject: " << email.subject
                          << " | Date: " << email.timestamp << "\n";
                found = true;
                seenEmails.insert(email.id);  // Mark this email as seen
            }
        }
        tempStack.push(email);
    }

    // Search outbox
    while (!outbox.isEmpty()) {
        Email email = outbox.dequeue();
        if (email.priority == searchPriority && (email.receiver == currentUser || email.sender == currentUser)) {
            // Check if this email has already been printed (based on email ID)
            if (seenEmails.find(email.id) == seenEmails.end()) {
                std::cout << "ID: " << email.id << " | From: " << email.sender
                          << " | To: " << email.receiver << " | Subject: " << email.subject
                          << " | Date: " << email.timestamp << "\n";
                found = true;
                seenEmails.insert(email.id);  // Mark this email as seen
            }
        }
        tempQueue.enqueue(email);
    }

    if (!found) {
        std::cout << "No emails found with priority " << searchPriority << ".\n";
    }

    // Restore inbox and outbox
    while (!tempStack.isEmpty()) {
        inbox.push(tempStack.pop());
    }

    while (!tempQueue.isEmpty()) {
        outbox.enqueue(tempQueue.dequeue());
    }
}



