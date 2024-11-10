/**
 * deleteEmail.h
 *  
*/


#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

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

            if (!getline(ss, id, '|') || !getline(ss, sender, '|') || !getline(ss, recipient, '|') ||
                !getline(ss, subject, '|') || !getline(ss, body, '|') || !getline(ss, datetime, '|') ||
                !getline(ss, status, '|') || !getline(ss, priorityStr, '|') || !getline(ss, spamStatus)) {
                cerr << "Error: Incorrectly formatted line: " << line << endl;
                continue; // Skip this line and move to the next one
            }

            if (sender != currentUser && recipient != currentUser) {
                continue; // Skip emails not related to the current user
            }

            try {
                priority = stoi(priorityStr);
            } catch (invalid_argument &e) {
                cerr << "Error: Invalid priority value '" << priorityStr << "' in line: " << line << endl;
                continue; // Skip this line and move to the next one
            }

            Email *newEmail = new Email(id, sender, recipient, subject, body, datetime, status, priority, spamStatus);

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

    void clear() {
        while (head) {
            Email *temp = head;
            head = head->next;
            delete temp;
        }
    }

    void displayEmails(const string &currentUser) {
        Email *current = head;
        bool found = false;
        cout << "\n--- Emails ---\n";
        while (current) {
            if (current->sender == currentUser || current->recipient == currentUser) {
                found = true;
                cout << "ID: " << current->id 
                    << " | From: " << current->sender 
                    << " | To: " << current->recipient 
                    << " | Subject: " << current->subject 
                    << " | Date: " << current->datetime 
                    << " | Status: " << current->status 
                    << " | Priority: " << current->priority 
                    << " | Spam Status: " << current->spamStatus << "\n";
            }
            current = current->next;
        }

        if (!found) {
            cout << "No emails found for this user.\n";
        }
    }
};

// // Delete email by ID and move it to the recycle bin
// bool deleteEmail(const string &emailFile, const string &recycleBinFile, const string &currentUser, const string &emailID) {
//     ifstream file(emailFile);
//     if (!file.is_open()) {
//         cerr << "Error opening file: " << emailFile << endl;
//         return false;
//     }

//     ofstream recycleBin(recycleBinFile, ios::app);
//     if (!recycleBin.is_open()) {
//         cerr << "Error opening recycle bin file.\n";
//         return false;
//     }

//     ofstream tempFile("temp.txt"); // Temporary file to write updated email list
//     if (!tempFile.is_open()) {
//         cerr << "Error creating temporary file.\n";
//         return false;
//     }

//     string line;
//     bool emailFound = false;

//     // Process each line (email) in the file
//     while (getline(file, line)) {
//         stringstream ss(line);
//         string id, sender, recipient, subject, body, datetime, status, priorityStr, spamStatus;

//         // Parse each field by the '|' delimiter
//         if (!getline(ss, id, '|') || !getline(ss, sender, '|') || !getline(ss, recipient, '|') ||
//             !getline(ss, subject, '|') || !getline(ss, body, '|') || !getline(ss, datetime, '|') ||
//             !getline(ss, status, '|') || !getline(ss, priorityStr, '|') || !getline(ss, spamStatus)) {
//             cerr << "Error: Incorrectly formatted line: " << line << endl;
//             continue;
//         }

//         // Check if this is the email to delete
//         if (id == emailID && (sender == currentUser || recipient == currentUser)) {
//             // Write the deleted email to the recycle bin
//             recycleBin << line << "\n";
//             emailFound = true;
//             continue; // Skip writing this line to the temp file
//         }

//         // Write all other emails to the temporary file
//         tempFile << line << "\n";
//     }

//     file.close();
//     recycleBin.close();
//     tempFile.close();

//     // If the email was not found, notify the user
//     if (!emailFound) {
//         cerr << "Email not found or you do not have permission to delete it.\n";
//         // Remove the temporary file since no changes are needed
//         remove("temp.txt");
//         return false;
//     }

//     // Replace original email file with the updated temp file
//     if (remove(emailFile.c_str()) != 0) {
//         cerr << "Error deleting original email file.\n";
//         return false;
//     }
//     if (rename("temp.txt", emailFile.c_str()) != 0) {
//         cerr << "Error renaming temporary file.\n";
//         return false;
//     }

//     cout << "Email with ID " << emailID << " successfully deleted and moved to recycle bin.\n";
//     return true;
// }


bool deleteEmail(const string &emailFile, const string &recycleBinFile, const string &currentUser, const string &emailID);
void viewDeletedEmailsLIFO(const string &recycleBinFile);
bool restoreEmailById(const string &recycleBinFile, const string &emailFile, const string &currentUser);

bool deleteEmail(const string &emailFile, const string &recycleBinFile, const string &currentUser, const string &emailID) {
    ifstream file(emailFile);
    if (!file.is_open()) {
        cerr << "Error opening file: " << emailFile << endl;
        return false;
    }

    ofstream recycleBin(recycleBinFile, ios::app);
    ofstream tempFile("temp.txt");
    if (!recycleBin.is_open() || !tempFile.is_open()) {
        cerr << "Error opening recycle bin or temp file.\n";
        return false;
    }

    string line;
    bool emailFound = false;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, sender, recipient, subject, body, datetime, status, priorityStr, spamStatus;

        if (!getline(ss, id, '|') || !getline(ss, sender, '|') || !getline(ss, recipient, '|') ||
            !getline(ss, subject, '|') || !getline(ss, body, '|') || !getline(ss, datetime, '|') ||
            !getline(ss, status, '|') || !getline(ss, priorityStr, '|') || !getline(ss, spamStatus)) {
            cerr << "Error: Incorrectly formatted line: " << line << endl;
            continue;
        }

        if (id == emailID && (sender == currentUser || recipient == currentUser)) {
            recycleBin << line << "\n";
            emailFound = true;
            continue;
        }
        tempFile << line << "\n";
    }

    file.close();
    recycleBin.close();
    tempFile.close();

    if (!emailFound) {
        cerr << "Email not found or you do not have permission to delete it.\n";
        remove("temp.txt");
        return false;
    }

    remove(emailFile.c_str());
    rename("temp.txt", emailFile.c_str());

    cout << "Email with ID " << emailID << " successfully deleted and moved to recycle bin.\n";
    return true;
}

void viewDeletedEmailsLIFO(const string &recycleBinFile) {
    struct StackNode {
        string emailData;
        StackNode *next;
    };

    StackNode *stackTop = nullptr;

    ifstream file(recycleBinFile);
    if (!file.is_open()) {
        cerr << "Error opening recycle bin file.\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        StackNode *newNode = new StackNode{line, stackTop};
        stackTop = newNode; // Push email onto the stack
    }
    file.close();

    if (!stackTop) {
        cout << "Recycle bin is empty.\n";
        return;
    }

    cout << "Deleted Emails (LIFO order):\n";
    while (stackTop) {
        cout << stackTop->emailData << "\n";
        StackNode *temp = stackTop;
        stackTop = stackTop->next; // Pop the stack
        delete temp;
    }
}

bool restoreEmailById(const string &recycleBinFile, const string &emailFile, const string &currentUser) {
    string emailID;
    cout << "Enter the ID of the email you want to restore: ";
    cin >> emailID;

    // Validate that the email ID is numeric (this assumes the ID is numeric)
    if (!all_of(emailID.begin(), emailID.end(), ::isdigit)) {
        cout << "Invalid ID! Please enter a valid numeric email ID.\n";
        return false;
    }

    // Step 1: Open the recycle bin and search for the email with the given ID
    ifstream recycleBin(recycleBinFile);
    if (!recycleBin.is_open()) {
        cerr << "Error opening recycle bin file.\n";
        return false;
    }

    string line;
    string restoredEmail;
    bool emailFound = false;

    // Read each line from the recycle bin and find the email to restore
    while (getline(recycleBin, line)) {
        stringstream ss(line);
        string id;
        // Extract the ID from the line
        getline(ss, id, '|'); // Assuming ID is the first field
        if (id == emailID) {
            restoredEmail = line; // Found the email, store it
            emailFound = true;
            break;
        }
    }

    recycleBin.close();

    if (!emailFound) {
        cout << "Email with ID " << emailID << " not found in the recycle bin.\n";
        return false;
    }

    // Step 2: Check if the restored email belongs to the current user
    stringstream ss(restoredEmail);
    string id, sender, recipient, subject, body, datetime, status, priorityStr, spamStatus;

    // Assuming email fields are separated by '|'
    getline(ss, id, '|');
    getline(ss, sender, '|');
    getline(ss, recipient, '|');
    // Skip the rest of the fields
    getline(ss, subject, '|');
    getline(ss, body, '|');
    getline(ss, datetime, '|');
    getline(ss, status, '|');
    getline(ss, priorityStr, '|');
    getline(ss, spamStatus);

    // Validate if the email belongs to the current user
    if (sender != currentUser && recipient != currentUser) {
        cout << "This email doesn't belong to the logged-in user.\n";
        return false;
    }

    // Step 3: Open the main email file and find the correct position for the restored email
    ifstream emailFileIn(emailFile);
    ofstream emailFileOut("temp_email_file.txt");  // Temporary file to store the updated list
    if (!emailFileIn.is_open() || !emailFileOut.is_open()) {
        cerr << "Error opening email file for updating.\n";
        return false;
    }

    string currentLine;
    bool emailInserted = false;

    // Read through the email file and write it to the temporary file, inserting the restored email at the correct position
    while (getline(emailFileIn, currentLine)) {
        stringstream lineStream(currentLine);
        string currentID;
        getline(lineStream, currentID, '|'); // Extract email ID

        if (!emailInserted && stoi(currentID) > stoi(emailID)) {
            emailFileOut << restoredEmail << "\n";  // Insert the restored email before the first greater ID
            emailInserted = true;
        }

        emailFileOut << currentLine << "\n";  // Write the rest of the emails
    }

    // If the restored email wasn't inserted (it should go at the end)
    if (!emailInserted) {
        emailFileOut << restoredEmail << "\n";
    }

    emailFileIn.close();
    emailFileOut.close();

    // Step 4: Now update the recycle bin, excluding the restored email
    // Open the recycle bin again and copy everything except the restored email to the updated recycle bin file
    ifstream recycleBinIn(recycleBinFile);
    ofstream tempRecycleBin("temp_recycle_bin.txt");  // Temporary file to store the updated recycle bin

    if (!recycleBinIn.is_open() || !tempRecycleBin.is_open()) {
        cerr << "Error opening recycle bin file for updating.\n";
        return false;
    }

    // Copy the remaining emails back to the recycle bin (excluding the restored email)
    while (getline(recycleBinIn, line)) {
        if (line != restoredEmail) {
            tempRecycleBin << line << "\n";
        }
    }

    recycleBinIn.close();
    tempRecycleBin.close();

    // Step 5: Replace the original email file and recycle bin file with the new versions
    remove(emailFile.c_str());  // Delete the original email file
    rename("temp_email_file.txt", emailFile.c_str());  // Rename the temporary email file to the original file name

    remove(recycleBinFile.c_str());  // Delete the original recycle bin file
    rename("temp_recycle_bin.txt", recycleBinFile.c_str());  // Rename the temporary recycle bin file to the original recycle bin file name

    cout << "Email restored successfully!\n";
    return true;
}