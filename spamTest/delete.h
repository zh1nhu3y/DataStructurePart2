#include <iostream>
#include <fstream>
#include <sstream>
#include "inboxStack.h"
#include "outboxQueue.h"

// Function to delete an email by ID from both inbox and outbox
void deleteEmail(InboxStack& inbox, OutboxQueue& outbox, const string& recycleBinFile, const string& emailFile, const string& currentUser) {
    int emailID;
    string input;

    cout << "Enter the ID of the email you want to delete: ";
    while (true) {
        getline(cin, input); // Read input as a string
        stringstream ss(input); // Convert the string to a stream for validation

        if (ss >> emailID) {
            if (ss.eof()) {  // Ensures that no additional characters are present
                break; 
            } else {
                cout << "Invalid input. Please enter a valid email ID: ";
            }
        } else {
            cout << "Invalid input. Please enter a valid email ID: ";
        }
    }

    ofstream recycleBin(recycleBinFile, ios::app);
    if (!recycleBin.is_open()) {
        cerr << "Failed to open recycle bin file.\n";
        return;
    }

    bool emailFound = false;
    InboxStack tempStack;
    OutboxQueue tempQueue;

    // Delete from inbox
    while (!inbox.isEmpty()) {
        Email email = inbox.pop();
        if (email.id == emailID && (email.receiver == currentUser || email.sender == currentUser)) {
            // Write to recycle bin
            recycleBin << email.id << "|" << email.sender << "|" << email.receiver << "|"
                       << email.subject << "|" << email.body << "|" << email.timestamp << "|"
                       << email.status << "|" << email.priority << "|" << email.spamStatus << "\n";
            emailFound = true;
            cout << "Email deleted from inbox and moved to recycle bin.\n";
        } else {
            tempStack.push(email);
        }
    }

    // Restore inbox after deletion
    while (!tempStack.isEmpty()) {
        inbox.push(tempStack.pop());
    }

    // Delete from outbox
    while (!outbox.isEmpty()) {
        Email email = outbox.dequeue();
        if (email.id == emailID && (email.receiver == currentUser || email.sender == currentUser)) {
            // Write to recycle bin
            recycleBin << email.id << "|" << email.sender << "|" << email.receiver << "|"
                       << email.subject << "|" << email.body << "|" << email.timestamp << "|"
                       << email.status << "|" << email.priority << "|" << email.spamStatus << "\n";
            emailFound = true;
            cout << "Email deleted from outbox and moved to recycle bin.\n";
        } else {
            tempQueue.enqueue(email);
        }
    }

    // Restore outbox after deletion
    while (!tempQueue.isEmpty()) {
        outbox.enqueue(tempQueue.dequeue());
    }

    // Now, remove the email from dummy_emails.txt
    if (emailFound) {
        removeEmailFromFile(emailFile, emailID);
    }

    recycleBin.close();
}

void removeEmailFromFile(const string& emailFile, int emailID) {
    ifstream emailFileIn(emailFile);
    ofstream tempFile("temp.txt");

    if (!emailFileIn.is_open() || !tempFile.is_open()) {
        cerr << "Failed to open email file or temporary file for update.\n";
        return;
    }

    string line;
    while (getline(emailFileIn, line)) {
        stringstream ss(line);
        int id;
        ss >> id;

        if (id != emailID) {
            tempFile << line << "\n";
        }
    }

    emailFileIn.close();
    tempFile.close();

    // Replace original email file with updated file (without the deleted email)
    remove(emailFile.c_str());
    rename("temp.txt", emailFile.c_str());
}




// Function to restore deleted emails from recycle bin back to inbox and outbox
void restoreEmail(InboxStack& inbox, OutboxQueue& outbox, const string& recycleBinFile, const string& emailFile, const string& currentUser) {
    int emailID;
    cout << "Enter the ID of the email you want to restore: ";
    cin >> emailID;

    ifstream infile(recycleBinFile);
    ofstream tempFile("temp.txt");
    if (!infile.is_open() || !tempFile.is_open()) {
        cerr << "Failed to open recycle bin file or temp file.\n";
        return;
    }

    string line;
    bool emailFound = false;
    while (getline(infile, line)) {
        stringstream ss(line);
        int id;
        ss >> id;
        if (id == emailID) {
            emailFound = true;
            // Extract email details and push to inbox and outbox
            string sender, receiver, subject, body, timestamp, status, spamStatus;
            ss.ignore();
            getline(ss, sender, '|');
            getline(ss, receiver, '|');
            getline(ss, subject, '|');
            getline(ss, body, '|');
            getline(ss, timestamp, '|');
            getline(ss, status, '|');
            getline(ss, spamStatus, '|');

            Email restoredEmail(id, sender, receiver, subject, body, timestamp, status, 0, spamStatus);
            inbox.push(restoredEmail);
            outbox.enqueue(restoredEmail);

            // Append to the email file in correct order
            saveEmailToFile(emailFile, restoredEmail);

            cout << "Restored email from recycle bin.\n";
        } else {
            tempFile << line << "\n";  // Keep other emails
        }
    }

    infile.close();
    tempFile.close();

    // Remove the restored email from the recycle bin
    remove(recycleBinFile.c_str());
    rename("temp.txt", recycleBinFile.c_str());
}

void saveEmailToFile(const string& emailFile, const Email& email) {
    ofstream outFile(emailFile, ios::app);
    if (outFile.is_open()) {
        outFile << email.id << "|"
                << email.sender << "|"
                << email.receiver << "|"
                << email.subject << "|"
                << email.body << "|"
                << email.timestamp << "|"
                << email.status << "|"
                << email.priority << "|"
                << email.spamStatus << "\n";
        outFile.close();
    } else {
        std::cerr << "Error opening file for writing.\n";
    }
}




// Function to view deleted emails from recycle bin in FIFO order
void viewDeletedEmailsFIFO(const std::string& recycleBinFile, const std::string& currentUser) {
    std::ifstream file(recycleBinFile);
    if (!file.is_open()) {
        std::cerr << "Failed to open recycle bin file.\n";
        return;
    }

    std::string line;
    bool emailFound = false;

    // Stack to simulate FIFO (First-In, First-Out)
    InboxStack tempStack;

    // Read emails from the recycle bin file
    while (getline(file, line)) {
        std::istringstream ss(line);
        Email email;
        std::string temp;
        
        // Parse email fields
        getline(ss, temp, '|'); email.id = std::stoi(temp);
        getline(ss, email.sender, '|');
        getline(ss, email.receiver, '|');
        getline(ss, email.subject, '|');
        getline(ss, email.body, '|');
        getline(ss, email.timestamp, '|');
        getline(ss, email.status, '|');
        getline(ss, temp, '|'); email.priority = std::stoi(temp);
        getline(ss, email.spamStatus, '|');

        // Store the email if it is related to the current user
        if (email.sender == currentUser || email.receiver == currentUser) {
            tempStack.push(email);  // Push email into stack
            emailFound = true;
        }
    }

    file.close();

    // Display emails in FIFO order (earliest deleted email first)
    if (emailFound) {
        while (!tempStack.isEmpty()) {
            Email email = tempStack.pop();  // Pop to display in FIFO order
            std::cout << "ID: " << email.id << "\n"
                      << "Sender: " << email.sender << "\n"
                      << "Receiver: " << email.receiver << "\n"
                      << "Subject: " << email.subject << "\n"
                      << "Body: " << email.body << "\n"
                      << "Timestamp: " << email.timestamp << "\n"
                      << "Status: " << email.status << "\n"
                      << "Priority: " << email.priority << "\n"
                      << "Spam Status: " << email.spamStatus << "\n\n";
        }
    } else {
        std::cout << "No deleted emails found for the specified user.\n";
    }
}




