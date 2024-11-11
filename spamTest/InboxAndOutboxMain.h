#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <set>
#include <algorithm>
#include "InboxStack.h"
#include "OutboxQueue.h"
#include "spamDetection.h"

class InboxandOutboxManagement
{
private:
    std::set<int> existingIds;
    Email latestEmail;

    // Private helper method to get current timestamp
    std::string getCurrentTimestamp()
    {
        time_t now = time(0);
        tm *localtm = localtime(&now);
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", localtm);
        return std::string(buffer);
    }

    // Helper method to display email details
    void displayEmail(const Email &email)
    {
        std::cout << "ID: " << email.id << "\nSender: " << email.sender
                  << "\nReceiver: " << email.receiver
                  << "\nSubject: " << email.subject
                  << "\nBody: " << email.body
                  << "\nTimestamp: " << email.timestamp
                  << "\nStatus: " << email.status
                  << "\nPriority: " << email.priority
                  << "\nSpam Status: " << email.spamStatus << "\n";
    }

    // Save email to file
    void saveEmailToFile(const Email &email)
    {
        std::ofstream outFile("../Data/dummy_emails.txt", std::ios::app);
        if (outFile.is_open())
        {
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
        }
        else
        {
            std::cerr << "Error opening file for writing." << std::endl;
        }
    }

public:
    // Constructor
    InboxandOutboxManagement() {}

    // Load emails from file
    void loadEmailsFromFile(const std::string &filename, InboxStack &inbox, OutboxQueue &outbox)
    {
        std::ifstream inFile(filename);
        if (!inFile.is_open())
        {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        std::string line;
        while (std::getline(inFile, line))
        {
            Email email;
            std::istringstream ss(line);
            std::string temp;

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

            existingIds.insert(email.id);

            if (email.id > latestEmail.id)
            {
                latestEmail = email;
            }

            // if (email.status == "Received")
            // {
            inbox.push(email);
            // }
            // else if (email.status == "Sent")
            // {
            outbox.enqueue(email);
            // }
        }
        inFile.close();
        std::cout << "Emails loaded from " << filename << "." << std::endl
                  << std::endl;
    }

    // Display the latest email
    void displayLatestEmail()
    {
        if (latestEmail.id != 0)
        {
            std::cout << "Latest Email:\n";
            displayEmail(latestEmail);
        }
        else
        {
            std::cout << "No emails available." << std::endl;
        }
        std::cout << std::endl; // Adding a blank line
    }

    // Send email method
    // void sendEmail(OutboxQueue &outbox)
    // {
    //     Email emailToSend;
    //     emailToSend.id = existingIds.empty() ? 1 : *existingIds.rbegin() + 1;
    //     std::cout << "Email ID Assigned: " << emailToSend.id << std::endl;

    //     std::cout << "Enter Receiver: ";
    //     std::getline(std::cin, emailToSend.receiver);
    //     std::cout << "Enter Subject: ";
    //     std::getline(std::cin, emailToSend.subject);
    //     std::cout << "Enter Body: ";
    //     std::getline(std::cin, emailToSend.body);

    //     emailToSend.timestamp = getCurrentTimestamp();
    //     emailToSend.status = "Sent";

    //     do
    //     {
    //         std::cout << "Enter Priority (1-5): ";
    //         std::cin >> emailToSend.priority;
    //         std::cin.ignore();
    //         if (emailToSend.priority < 1 || emailToSend.priority > 5)
    //         {
    //             std::cout << "Warning: Priority must be between 1 and 5." << std::endl;
    //         }
    //     } while (emailToSend.priority < 1 || emailToSend.priority > 5);

    //     outbox.enqueue(emailToSend);
    //     std::cout << "Email added to the outbox." << std::endl;
    //     existingIds.insert(emailToSend.id);

    //     saveEmailToFile(emailToSend);
    // }

    void sendEmail(OutboxQueue &outbox, InboxStack &inbox, const string &emailFile, string spamWords[], int spamWordCount)
    {
        Email emailToSend;
        SpamDetection spamDetection;
        emailToSend.id = existingIds.empty() ? 1 : *existingIds.rbegin() + 1;
        std::cout << "Email ID Assigned: " << emailToSend.id << std::endl;

        std::cout << "Enter Receiver: ";
        std::getline(std::cin, emailToSend.receiver);
        std::cout << "Enter Subject: ";
        std::getline(std::cin, emailToSend.subject);
        std::cout << "Enter Body: ";
        std::getline(std::cin, emailToSend.body);

        emailToSend.timestamp = getCurrentTimestamp();
        emailToSend.status = "Sent";

        do
        {
            std::cout << "Enter Priority (1-5): ";
            std::cin >> emailToSend.priority;
            std::cin.ignore();
            if (emailToSend.priority < 1 || emailToSend.priority > 5)
            {
                std::cout << "Warning: Priority must be between 1 and 5." << std::endl;
            }
        } while (emailToSend.priority < 1 || emailToSend.priority > 5);

        existingIds.insert(emailToSend.id);

        // Check if the email is spam before appending
        spamDetection.checkSingleEmailAndAppend(&emailToSend, spamWords, spamWordCount, outbox, emailFile);

        // Add the email to the outbox after spam check
        outbox.enqueue(emailToSend);
        std::cout << "Email added to the outbox." << std::endl;

        // Update the inbox by adding the email
        inbox.push(emailToSend);
        std::cout << "Email added to the inbox." << std::endl;

        // saveEmailToFile(emailToSend); // Save the email to the file
    }

    // Inbox management method
    void handleInboxManagement(InboxStack &inbox)
    {
        int inboxChoice;
        do
        {
            std::cout << "\nInbox Management:\n"
                      << "1. Pop top email\n"
                      << "2. View current top email\n"
                      << "3. Back to main menu\n"
                      << "Enter your choice (1-3): ";
            std::cin >> inboxChoice;
            std::cin.ignore();
            std::cout << std::endl;

            switch (inboxChoice)
            {
            case 1:
                if (!inbox.isEmpty())
                {
                    Email poppedEmail = inbox.pop();
                    std::cout << "Popped Email:\n";
                    displayEmail(poppedEmail);
                }
                else
                {
                    std::cout << "Inbox is empty. No email to pop.\n\n";
                }
                break;
            case 2:
                if (!inbox.isEmpty())
                {
                    inbox.displayTop();
                }
                else
                {
                    std::cout << "Inbox is empty.\n\n";
                }
                break;
            case 3:
                std::cout << "Returning to main menu...\n\n";
                break;
            default:
                std::cout << "Invalid option. Please choose a number between 1 and 3.\n\n";
                break;
            }
        } while (inboxChoice != 3);
    }

    // Outbox management method
    void handleOutboxManagement(OutboxQueue &outbox, InboxStack &inbox, const string &emailFile, string spamWords[], int spamWordCount)
    {
        int outboxChoice;
        do
        {
            std::cout << "\nOutbox Management:\n"
                      << "1. Enqueue new email\n"
                      << "2. Dequeue email to send\n"
                      << "3. View front email\n"
                      << "4. Back to main menu\n"
                      << "Enter your choice (1-4): ";
            std::cin >> outboxChoice;
            std::cin.ignore();
            std::cout << std::endl;

            switch (outboxChoice)
            {
            case 1:
                sendEmail(outbox, inbox, emailFile, spamWords, spamWordCount);
                break;
            case 2:
                if (!outbox.isEmpty())
                {
                    Email sentEmail = outbox.dequeue();
                    std::cout << "Sent Email:\n";
                    displayEmail(sentEmail);
                }
                else
                {
                    std::cout << "Outbox is empty. No email to send.\n\n";
                }
                break;
            case 3:
                if (!outbox.isEmpty())
                {
                    outbox.displayFront();
                }
                else
                {
                    std::cout << "Outbox is empty.\n\n";
                }
                break;
            case 4:
                std::cout << "Returning to main menu...\n\n";
                break;
            default:
                std::cout << "Invalid option. Please choose a number between 1 and 4.\n\n";
                break;
            }
        } while (outboxChoice != 4);
    }
};
