#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <set>
#include "InboxStack.h"
#include "OutboxQueue.h"

// Function to get the current timestamp in the desired format
std::string getCurrentTimestamp() {
    time_t now = time(0);
    tm* localtm = localtime(&now);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", localtm);
    return std::string(buffer);
}

// Function to load emails from a file and distribute them into Inbox and Outbox
void loadEmailsFromFile(const std::string& filename, InboxStack& inbox, OutboxQueue& outbox, std::set<int>& existingIds, Email& latestEmail) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(inFile, line)) {
        Email email;
        std::istringstream ss(line);
        std::string temp;
        // Parse the line using '|' as the delimiter
        std::getline(ss, temp, '|');
        email.id = std::stoi(temp);
        std::getline(ss, email.sender, '|');
        std::getline(ss, email.receiver, '|');
        std::getline(ss, email.subject, '|');
        std::getline(ss, email.body, '|');
        std::getline(ss, email.timestamp, '|');
        std::getline(ss, email.status, '|');
        std::getline(ss, temp, '|');
        email.priority = std::stoi(temp);
        std::getline(ss, email.spamStatus, '|');
        
        // Track unique IDs
        existingIds.insert(email.id);

        // Update the latest email if this email has a higher ID
        if (email.id > latestEmail.id) {
            latestEmail = email;
        }
        
        // Add email to Inbox or Outbox based on Status
        if (email.status == "Received") {
            inbox.push(email);
        } else if (email.status == "Sent") {
            outbox.enqueue(email);
        }
    }
    inFile.close();
    std::cout << "Emails loaded from " << filename << "." << std::endl;
}

// Function to enqueue an email to the outbox
void sendEmail(OutboxQueue& outbox, std::set<int>& existingIds, InboxStack& inbox) {
    Email emailToSend;

    // Set the next available Email ID
    emailToSend.id = existingIds.empty() ? 1 : *existingIds.rbegin() + 1;

    std::cout << "Email ID Assigned: " << emailToSend.id << std::endl;

    // Gather the remaining email details from the user
    std::cout << "Enter Sender: ";
    std::getline(std::cin, emailToSend.sender);
    std::cout << "Enter Receiver: ";
    std::getline(std::cin, emailToSend.receiver);
    std::cout << "Enter Subject: ";
    std::getline(std::cin, emailToSend.subject);
    std::cout << "Enter Body: ";
    std::getline(std::cin, emailToSend.body);
    
    // Set the current timestamp
    emailToSend.timestamp = getCurrentTimestamp();

    // Gather status from the user (Received/Sent)
    do {
        std::cout << "Enter Status (Received/Sent): ";
        std::getline(std::cin, emailToSend.status);
        if (emailToSend.status != "Received" && emailToSend.status != "Sent") {
            std::cout << "Warning: Status must be 'Received' or 'Sent'." << std::endl;
        }
    } while (emailToSend.status != "Received" && emailToSend.status != "Sent");

    // Set priority and spam status
    do {
        std::cout << "Enter Priority (1-5): ";
        std::cin >> emailToSend.priority;
        std::cin.ignore(); // Ignore newline after priority input
        if (emailToSend.priority < 1 || emailToSend.priority > 5) {
            std::cout << "Warning: Priority must be between 1 and 5." << std::endl;
        }
    } while (emailToSend.priority < 1 || emailToSend.priority > 5);

    std::cout << "Enter Spam Status (Yes/No): ";
    std::getline(std::cin, emailToSend.spamStatus);

    // Enqueue the email into the outbox if the status is 'Sent'
    if (emailToSend.status == "Sent") {
        outbox.enqueue(emailToSend);
        std::cout << "Email added to the outbox." << std::endl;
    } 
    // Push to inbox if the status is 'Received'
    else if (emailToSend.status == "Received") {
        inbox.push(emailToSend); // Push to inbox stack
        std::cout << "Email added to the inbox." << std::endl;
    }

    // Save the email to dummy_emails.txt
    std::ofstream outFile("dummy_emails.txt", std::ios::app);
    if (outFile.is_open()) {
        outFile << emailToSend.id << "|"
                << emailToSend.sender << "|"
                << emailToSend.receiver << "|"
                << emailToSend.subject << "|"
                << emailToSend.body << "|"
                << emailToSend.timestamp << "|"
                << emailToSend.status << "|"
                << emailToSend.priority << "|"
                << emailToSend.spamStatus << "\n";
        outFile.close();
    } else {
        std::cerr << "Error opening file for writing." << std::endl;
    }
}

// Main function
int main() {
    InboxStack inbox;
    OutboxQueue outbox;
    std::set<int> existingIds;
    Email latestEmail = {0}; // Initialize with a dummy email

    // Load emails from dummy_emails.txt into inbox and outbox
    loadEmailsFromFile("dummy_emails.txt", inbox, outbox, existingIds, latestEmail);

    // Display the latest email loaded
    if (latestEmail.id != 0) { // Check if a valid email was found
        std::cout << "Latest Email:\n";
        std::cout << "ID: " << latestEmail.id << "\nSender: " << latestEmail.sender 
                  << "\nReceiver: " << latestEmail.receiver 
                  << "\nSubject: " << latestEmail.subject 
                  << "\nBody: " << latestEmail.body 
                  << "\nTimestamp: " << latestEmail.timestamp 
                  << "\nStatus: " << latestEmail.status 
                  << "\nPriority: " << latestEmail.priority 
                  << "\nSpam Status: " << latestEmail.spamStatus << "\n";
    } else {
        std::cout << "No emails available." << std::endl;
    }

    char mainChoice;
    do {
        std::cout << "\nChoose an option:\n"
                  << "(I)nbox Management\n"
                  << "(O)utbox Management\n"
                  << "(E)xit\n";
        std::cin >> mainChoice;
        std::cin.ignore(); // Ignore newline after choice input

        switch (mainChoice) {
            case 'I': case 'i': {
                char inboxChoice;
                do {
                    std::cout << "\nInbox Management:\n"
                              << "(P)op top email\n"
                              << "(V)iew current top email\n"
                              << "(B)ack to main menu\n";
                    std::cin >> inboxChoice;
                    std::cin.ignore(); // Ignore newline after choice input

                    switch (inboxChoice) {
                        case 'P': case 'p':
                            if (!inbox.isEmpty()) {
                                Email latestEmail = inbox.pop();
                                std::cout << "Popped Email:\n";
                                // Display popped email details
                                std::cout << "ID: " << latestEmail.id << "\nSender: " << latestEmail.sender 
                                          << "\nReceiver: " << latestEmail.receiver 
                                          << "\nSubject: " << latestEmail.subject 
                                          << "\nBody: " << latestEmail.body 
                                          << "\nTimestamp: " << latestEmail.timestamp 
                                          << "\nStatus: " << latestEmail.status 
                                          << "\nPriority: " << latestEmail.priority 
                                          << "\nSpam Status: " << latestEmail.spamStatus << "\n";
                            } else {
                                std::cout << "Inbox is empty. No email to pop." << std::endl;
                            }
                            break;
                        case 'V': case 'v':
                            if (!inbox.isEmpty()) {
                                inbox.displayTop();
                            } else {
                                std::cout << "Inbox is empty." << std::endl;
                            }
                            break;
                        case 'B': case 'b':
                            std::cout << "Returning to main menu..." << std::endl;
                            break;
                        default:
                            std::cout << "Invalid option. Please choose again." << std::endl;
                            break;
                    }
                } while (inboxChoice != 'B' && inboxChoice != 'b');
                break;
            }
            case 'O': case 'o': {
                char outboxChoice;
                do {
                    std::cout << "\nOutbox Management:\n"
                            << "(E)nqueue new email\n"
                            << "(D)equeue email to send\n"
                            << "(V)iew front email\n"
                            << "(B)ack to main menu\n";
                    std::cin >> outboxChoice;
                    std::cin.ignore(); // Ignore newline after choice input

                    switch (outboxChoice) {
                        case 'E': case 'e':
                            sendEmail(outbox, existingIds, inbox); // Pass inbox as well
                            break;
                        case 'D': case 'd':
                            if (!outbox.isEmpty()) {
                                Email sentEmail = outbox.dequeue();
                                std::cout << "Sent Email:\n";
                                // Display sent email details including status, priority, and spam status
                                std::cout << "ID: " << sentEmail.id << "\nSender: " << sentEmail.sender 
                                        << "\nReceiver: " << sentEmail.receiver 
                                        << "\nSubject: " << sentEmail.subject 
                                        << "\nBody: " << sentEmail.body 
                                        << "\nTimestamp: " << sentEmail.timestamp 
                                        << "\nStatus: " << sentEmail.status 
                                        << "\nPriority: " << sentEmail.priority 
                                        << "\nSpam Status: " << sentEmail.spamStatus << "\n";
                            } else {
                                std::cout << "Outbox is empty. No email to send." << std::endl;
                            }
                            break;
                        case 'V': case 'v':
                            if (!outbox.isEmpty()) {
                                outbox.displayFront();
                            } else {
                                std::cout << "Outbox is empty." << std::endl;
                            }
                            break;
                        case 'B': case 'b':
                            std::cout << "Returning to main menu..." << std::endl;
                            break;
                        default:
                            std::cout << "Invalid option. Please choose again." << std::endl;
                            break;
                    }
                } while (outboxChoice != 'B' && outboxChoice != 'b');
                break;
            }
            case 'E': case 'e':
                std::cout << "Exiting program..." << std::endl;
                break;
            default:
                std::cout << "Invalid option. Please choose again." << std::endl;
                break;
        }
    } while (mainChoice != 'E' && mainChoice != 'e');

    return 0;
}