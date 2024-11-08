/**
 * priorityHandling.h
 * 
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

//Email struct
struct Email {
    string id;
    string sender;
    string recipient;
    string subject;
    string body;
    string datetime;
    int priority;
    Email *next;

    Email(string identifier, string sndr, string rcpt, string subj, string bdy, string dttme, int prio)
        : id(identifier), sender(sndr), recipient(rcpt), subject(subj), body(bdy), datetime(dttme), priority(prio), next(nullptr) {}
};

// Priority Queue Class
class PriorityQueue {
private:
    // Pointer to the head of the list
    Email *head;

public:
    // Constructor
    PriorityQueue() : head(nullptr) {}

    // Destructor to free memory
    ~PriorityQueue()
    {
        while (head)
        {
            Email *temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Insert email in priority order (1 is the highest priority, 5 is the lowest)
    void insertEmailInPriority(string id, string sender, string recipient, string subject, string body, string datetime, int priority) {
        Email *newEmail = new Email(id, sender, recipient, subject, body, datetime, priority);

        if (!head || head->priority > priority) {
            newEmail->next = head;
            head = newEmail;
        } else {
            Email *current = head;
            while (current->next && current->next->priority <= priority) {
                current = current->next;
            }
            newEmail->next = current->next;
            current->next = newEmail;
        }
    }

    // Display emails accroding to priotity (1 = High, 5 = Low)
    void displayEmailsByPriority(const string &currentUser) const {
        if (!head) {
            cout << "No emails to display.\n";
            return;
        }

        Email *current = head;
        bool found = false;
        while (current) {
            if (current->sender == currentUser || current->recipient == currentUser) {
                cout << "ID: " << current->id 
                    << " | Priority: " << current->priority 
                    << " | From: " << current->sender 
                    << " | To: " << current->recipient 
                    << " | Subject: " << current->subject << "\n";
                found = true;
            }
            current = current->next;
        }

        if (!found) {
            cout << "No emails found for user: " << currentUser << "\n";
        }
    }


    // Seach for emails by priority
    void searchEmailsByPriorityForUser(const string &currentUser, int searchPriority) const {
        if (!head) {
            cout << "No emails to search.\n";
            return;
        }

        Email *current = head;
        bool found = false;
        while (current) {
            if ((current->sender == currentUser || current->recipient == currentUser) && current->priority == searchPriority) {
                cout << "Email - Priority: " << current->priority
                     << ", From: " << current->sender
                     << ", To: " << current->recipient
                     << ", Subject: " << current->subject
                     << ", Date: " << current->datetime << "\n";
                found = true;
            }
            current = current->next;
        }

        if (!found) {
            cout << "No emails found with priority " << searchPriority << " for user: " << currentUser << "\n";
        }
    }
    
    // Edit email priority
    void editEmailPriority(const string &emailFile, const string &currentUser, const string &emailID, int newPriority) {
        if (newPriority < 1 || newPriority > 5) {
            cerr << "Invalid priority level. Must be between 1 and 5.\n";
            return;
        }

        Email *current = head;
        bool emailFound = false;

        // Search for the email to update the priority in memory
        while (current) {
            if (current->id == emailID && (current->sender == currentUser || current->recipient == currentUser)) {
                current->priority = newPriority;
                emailFound = true;
                cout << "Priority updated for email ID " << emailID << " to " << newPriority << "\n";
                break;
            }
            current = current->next;
        }

        if (!emailFound) {
            cout << "Email with ID " << emailID << " not found for user.\n";
            return;
        }

        // Rebuild the priority queue after updating priority
        Email *orderedHead = nullptr;
        current = head;
        while (current) {
            Email *nextEmail = current->next;
            // Reinsert emails into orderedHead instead of head to ensure a correctly ordered list
            if (!orderedHead || orderedHead->priority > current->priority) {
                current->next = orderedHead;
                orderedHead = current;
            } else {
                Email *temp = orderedHead;
                while (temp->next && temp->next->priority <= current->priority) {
                    temp = temp->next;
                }
                current->next = temp->next;
                temp->next = current;
            }
            current = nextEmail;
        }
        head = orderedHead;

        // Rewrite the entire email file with updated priorities
        ifstream inFile(emailFile);
        ofstream tempFile("temp.txt");
        string line;

        while (getline(inFile, line)) {
            stringstream ss(line);
            string id, sender, recipient, subject, body, datetime, status, priorityStr, spamStatus;
            int priority;

            // Parse each field including spamStatus
            if (!getline(ss, id, '|') || !getline(ss, sender, '|') || !getline(ss, recipient, '|') ||
                !getline(ss, subject, '|') || !getline(ss, body, '|') || !getline(ss, datetime, '|') ||
                !getline(ss, status, '|') || !getline(ss, priorityStr, '|') || !getline(ss, spamStatus)) {
                cerr << "Error: Incorrectly formatted line: " << line << endl;
                continue;
            }

            // Attempt to convert priority
            try {
                priority = stoi(priorityStr);
            } catch (invalid_argument &) {
                cerr << "Error: Invalid priority value '" << priorityStr << "' in line: " << line << endl;
                continue;
            }

            // Update priority if matching email ID and user
            if (id == emailID && (sender == currentUser || recipient == currentUser)) {
                priority = newPriority;
            }

            // Write updated or original line to the temp file, including spamStatus
            tempFile << id << "|" << sender << "|" << recipient << "|" << subject << "|" << body << "|" << datetime << "|"
                    << status << "|" << priority << "|" << spamStatus << "\n";
        }

        inFile.close();
        tempFile.close();

        // Replace original email file with the updated temp file
        if (remove(emailFile.c_str()) != 0) {
            cerr << "Error deleting original email file.\n";
            return;
        }
        if (rename("temp.txt", emailFile.c_str()) != 0) {
            cerr << "Error renaming temporary file.\n";
            return;
        }

        cout << "Email with ID " << emailID << " successfully saved with updated priority.\n";
    }


    void loadEmailsFromFile(const string &emailFile) {
        ifstream file(emailFile);
        if (!file.is_open()) {
            cerr << "Error opening file: " << emailFile << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string id, sender, recipient, subject, body, datetime, status, priorityStr;
            int priority;

            // Parse each field by the '|' delimiter, ensuring the correct order and skipping 'status'
            if (!getline(ss, id, '|') || !getline(ss, sender, '|') || !getline(ss, recipient, '|') ||
                !getline(ss, subject, '|') || !getline(ss, body, '|') || !getline(ss, datetime, '|') ||
                !getline(ss, status, '|') || !getline(ss, priorityStr, '|')) {
                cerr << "Error: Incorrectly formatted line: " << line << endl;
                continue;
            }

            // Convert priority string to integer
            try {
                priority = stoi(priorityStr);
            } catch (invalid_argument &e) {
                cerr << "Error: Invalid priority value '" << priorityStr << "' in line: " << line << endl;
                continue;  // Skip this line and move to the next one
            }

            // Insert the email into the priority queue
            insertEmailInPriority(id, sender, recipient, subject, body, datetime, priority);
        }

        file.close();
        cout << "Emails loaded from file and inserted into the priority queue.\n";
    }


    
    // void loadEmailsFromFile(const string &emailFile)
    // {
    //     ifstream file(emailFile);
    //     if (!file.is_open()) 
    //     {
    //         cerr << "Error opening file: " << emailFile << endl;
    //         return;
    //     }

    //     string line;
    //     while (getline(file, line)) 
    //     {
    //         stringstream ss(line);
    //         string id, sender, recipient, subject, body, datetime, status, priorityStr, spamStatus;
    //         int priority;

    //         // Parse each attribute by the '|' delimiter
    //         if (!getline(ss, id, '|') ||            // Skip the ID field
    //             !getline(ss, sender, '|') ||
    //             !getline(ss, recipient, '|') ||
    //             !getline(ss, subject, '|') ||
    //             !getline(ss, body, '|') ||
    //             !getline(ss, datetime, '|') ||
    //             !getline(ss, status, '|') ||        // Parse the status field but do not use it
    //             !getline(ss, priorityStr, '|') ||   // Parse the priority as a string to convert later
    //             !getline(ss, spamStatus)) {         // Parse the spam status field as a string
    //             cerr << "Error: Incorrectly formatted line: " << line << endl;
    //             continue;  // Skip this line and move to the next one
    //         }

    //         // Convert priority string to integer
    //         try 
    //         {
    //             priority = stoi(priorityStr);
    //         } 
    //         catch (invalid_argument &e) 
    //         {
    //             cerr << "Error: Invalid priority value '" << priorityStr << "' in line: " << line << endl;
    //             continue;  // Skip this line and move to the next one
    //         }

    //         // Insert the email into the priority queue
    //         insertEmailInPriority(sender, recipient, subject, body, datetime, priority);
    //     }

    //     file.close();
    //     cout << "Emails loaded from file and inserted into the priority queue." << endl;
    // }

};