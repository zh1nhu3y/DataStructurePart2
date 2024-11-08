/**
 * deleteEmail.h
 *  
*/


#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

// Placeholder definition of Email structure
struct Email {
    string id;         // Unique identifier
    string sender;
    string recipient;
    string subject;
    string body;
    string datetime;
    string status;     // New field for status
    int priority;
    string spamStatus; // New field for spam status
    Email *next;

    // Constructor
    Email(string identifier, string sndr, string rcpt, string subj, string bdy, string dttme, string stat, int prio, string spamStat)
        : id(identifier), sender(sndr), recipient(rcpt), subject(subj), body(bdy), datetime(dttme), status(stat), priority(prio), spamStatus(spamStat), next(nullptr) {}
};

// Placeholder definition of PriorityQueue class
class PriorityQueue {
public:
    Email *head;

    // Constructor
    PriorityQueue() : head(nullptr) {}

    // Destructor to free memory
    ~PriorityQueue() {
        while (head) {
            Email *temp = head;
            head = head->next;
            delete temp;
        }
    }

   // Load only emails relevant to the logged-in user
    void loadUserEmails(const string &emailFile, const string &currentUser) {
        ifstream file(emailFile);
        if (!file.is_open()) {
            cerr << "Error opening file: " << emailFile << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string id, sender, recipient, subject, body, datetime, status, priorityStr, spamStatus;
            int priority;

            // Parse each field by the '|' delimiter
            if (!getline(ss, id, '|') || !getline(ss, sender, '|') || !getline(ss, recipient, '|') ||
                !getline(ss, subject, '|') || !getline(ss, body, '|') || !getline(ss, datetime, '|') ||
                !getline(ss, status, '|') || !getline(ss, priorityStr, '|') || !getline(ss, spamStatus)) {
                cerr << "Error: Incorrectly formatted line: " << line << endl;
                continue; // Skip this line and move to the next one
            }

            // Check if this email is relevant to the logged-in user (either as sender or recipient)
            if (sender != currentUser && recipient != currentUser) {
                continue; // Skip emails not related to the current user
            }

            // Convert priority string to integer
            try {
                priority = stoi(priorityStr);
            } catch (invalid_argument &e) {
                cerr << "Error: Invalid priority value '" << priorityStr << "' in line: " << line << endl;
                continue; // Skip this line and move to the next one
            }

            // Create a new Email object for the current line
            Email *newEmail = new Email(id, sender, recipient, subject, body, datetime, status, priority, spamStatus);

            // Insert the email into the queue in priority order
            if (!head || head->priority < priority) {
                newEmail->next = head;
                head = newEmail;
            } else {
                Email *current = head;
                while (current->next && current->next->priority >= priority) {
                    current = current->next;
                }
                newEmail->next = current->next;
                current->next = newEmail;
            }
        }

        file.close();
        cout << "All emails relevant to the logged-in user have been loaded successfully.\n";
    }

    // Load all emails without filtering (for deletion or other operations)
    void loadAllEmails(const string &emailFile) {
        ifstream file(emailFile);
        if (!file.is_open()) {
            cerr << "Error opening file: " << emailFile << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string id, sender, recipient, subject, body, datetime, status, priorityStr, spamStatus;
            int priority;

            // Parse each field by the '|' delimiter
            if (!getline(ss, id, '|') || !getline(ss, sender, '|') || !getline(ss, recipient, '|') ||
                !getline(ss, subject, '|') || !getline(ss, body, '|') || !getline(ss, datetime, '|') ||
                !getline(ss, status, '|') || !getline(ss, priorityStr, '|') || !getline(ss, spamStatus)) {
                cerr << "Error: Incorrectly formatted line: " << line << endl;
                continue; // Skip this line and move to the next one
            }

            // Convert priority string to integer
            try {
                priority = stoi(priorityStr);
            } catch (invalid_argument &e) {
                cerr << "Error: Invalid priority value '" << priorityStr << "' in line: " << line << endl;
                continue; // Skip this line and move to the next one
            }

            // Create a new Email object for the current line
            Email *newEmail = new Email(id, sender, recipient, subject, body, datetime, status, priority, spamStatus);

            // Insert the email into the queue in priority order
            if (!head || head->priority < priority) {
                newEmail->next = head;
                head = newEmail;
            } else {
                Email *current = head;
                while (current->next && current->next->priority >= priority) {
                    current = current->next;
                }
                newEmail->next = current->next;
                current->next = newEmail;
            }
        }

        file.close();
        cout << "All emails have been loaded successfully.\n";
    }
};

// Delete email by ID and move it to the recycle bin
bool deleteEmail(const string &emailFile, const string &recycleBinFile, const string &currentUser, const string &emailID) {
    ifstream file(emailFile);
    if (!file.is_open()) {
        cerr << "Error opening file: " << emailFile << endl;
        return false;
    }

    ofstream recycleBin(recycleBinFile, ios::app);
    if (!recycleBin.is_open()) {
        cerr << "Error opening recycle bin file.\n";
        return false;
    }

    ofstream tempFile("temp.txt"); // Temporary file to write updated email list
    if (!tempFile.is_open()) {
        cerr << "Error creating temporary file.\n";
        return false;
    }

    string line;
    bool emailFound = false;

    // Process each line (email) in the file
    while (getline(file, line)) {
        stringstream ss(line);
        string id, sender, recipient, subject, body, datetime, status, priorityStr, spamStatus;

        // Parse each field by the '|' delimiter
        if (!getline(ss, id, '|') || !getline(ss, sender, '|') || !getline(ss, recipient, '|') ||
            !getline(ss, subject, '|') || !getline(ss, body, '|') || !getline(ss, datetime, '|') ||
            !getline(ss, status, '|') || !getline(ss, priorityStr, '|') || !getline(ss, spamStatus)) {
            cerr << "Error: Incorrectly formatted line: " << line << endl;
            continue;
        }

        // Check if this is the email to delete
        if (id == emailID && (sender == currentUser || recipient == currentUser)) {
            // Write the deleted email to the recycle bin
            recycleBin << line << "\n";
            emailFound = true;
            continue; // Skip writing this line to the temp file
        }

        // Write all other emails to the temporary file
        tempFile << line << "\n";
    }

    file.close();
    recycleBin.close();
    tempFile.close();

    // If the email was not found, notify the user
    if (!emailFound) {
        cerr << "Email not found or you do not have permission to delete it.\n";
        // Remove the temporary file since no changes are needed
        remove("temp.txt");
        return false;
    }

    // Replace original email file with the updated temp file
    if (remove(emailFile.c_str()) != 0) {
        cerr << "Error deleting original email file.\n";
        return false;
    }
    if (rename("temp.txt", emailFile.c_str()) != 0) {
        cerr << "Error renaming temporary file.\n";
        return false;
    }

    cout << "Email with ID " << emailID << " successfully deleted and moved to recycle bin.\n";
    return true;
}
