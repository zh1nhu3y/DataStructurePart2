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
    string sender;
    string recipient;
    string subject;
    string body;
    string datetime;
    int priority;
    Email *next;

    // Constructor
    Email(string sndr, string rcpt, string subj, string bdy, string dttme, int prio)
        : sender(sndr), recipient(rcpt), subject(subj), body(bdy), datetime(dttme), priority(prio), next(nullptr) {}
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

    // Insert email in priority order
    void insertEmailInPriority(string sender, string recipient, string subject, string body, string datetime, int priority) {
        Email *newEmail = new Email(sender, recipient, subject, body, datetime, priority);

        // If queue is empty or new email has highest priority
        if (!head || head->priority < priority)
        {
            newEmail->next = head;  // Point to the previous value pointed by Head
            head = newEmail; // Point Head to new value
        }
        else
        {
            // Find correct position
            Email *current = head;
            while (current->next && current->next->priority >= priority)
            {
                current = current->next;
            }
            newEmail->next = current->next;
            current->next = newEmail;
        }
        cout << "Email inserted with priority " << priority << endl;
    }

    // Retrieve (remove) highest priority email
    void processTopPriorityEmail()
    {
        if (!head)
        {
            cout << "No emails to process. \n";
            return;
        }

        Email *topEmail = head;
        head = head->next;

        // Display email details
        cout << "Processing Email: \n"
             << "From: " << topEmail->sender << "\n"
             << "To: " << topEmail->recipient << "\n"
             << "Subject: " << topEmail->subject << "\n"
             << "Date/Time: " << topEmail->datetime << "\n"
             << "Priority: " << topEmail->priority << "\n"
             << "Content: " << topEmail->body << "\n"; 

        delete topEmail;
    }

    // Display all emails by priority
    void displayAllEmailsByPriority() const
    {
        if (!head)
        {
            cout << "No emails to display. \n";
            return;
        }

        Email *current = head;
        while (current)
        {
            cout << "Email - Priority: " << current->priority
                 << ", From: " << current->sender
                 << ", To: " << current->recipient
                 << ", Subject: " << current->subject << "\n";
            current = current->next;
        }
    }

    // Load emails from file and add them to the priority queue
    // void loadEmailsFromFile(const string& emailFile) {
    //     ifstream file(emailFile);
    //     if (!file.is_open()) {
    //         cerr << "Error opening file: " << emailFile << endl;
    //         return;
    //     }

    //     string line;
    //     while (getline(file, line)) {
    //         stringstream ss(line);
    //         string sender, recipient, subject, body, date, priorityStr, spamStatus;
    //         int priority;

    //         // Parse each attribute by the '|' delimiter
    //         getline(ss, sender, '|');
    //         getline(ss, recipient, '|');
    //         getline(ss, subject, '|');
    //         getline(ss, body, '|');
    //         getline(ss, date, '|');
    //         getline(ss, priorityStr, '|');
    //         getline(ss, spamStatus);

    //         // Convert priority string to integer
    //         priority = stoi(priorityStr);

    //         // Insert the email into the priority queue
    //         insertEmailInPriority(sender, recipient, subject, body, priority);
    //     }

    //     file.close();
    //     cout << "Emails loaded from file and inserted into the priority queue." << std::endl;
    // }
    void loadEmailsFromFile(const string &emailFile)
    {
        ifstream file(emailFile);
        if (!file.is_open()) {
            cerr << "Error opening file: " << emailFile << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string sender, recipient, subject, body, datetime, priorityStr, spamStatus;
            int priority;

            // Parse each attribute by the '|' delimiter
            if (!getline(ss, sender, '|') ||
                !getline(ss, recipient, '|') ||
                !getline(ss, subject, '|') ||
                !getline(ss, body, '|') ||
                !getline(ss, datetime, '|') ||
                !getline(ss, priorityStr, '|') ||
                !getline(ss, spamStatus)) {
                cerr << "Error: Incorrectly formatted line: " << line << endl;
                continue;  // Skip this line and move to the next one
            }

            // Convert priority string to integer
            try {
                priority = stoi(priorityStr);
            } catch (invalid_argument &e) {
                cerr << "Error: Invalid priority value '" << priorityStr << "' in line: " << line << endl;
                continue;  // Skip this line and move to the next one
            }

            // Insert the email into the priority queue
            insertEmailInPriority(sender, recipient, subject, body, datetime, priority);
        }

        file.close();
        cout << "Emails loaded from file and inserted into the priority queue." << endl;
    }
};

