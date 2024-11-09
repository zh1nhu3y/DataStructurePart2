void displayEmail(const Email& email) {
        std::cout << "ID: " << email.id << "\nSender: " << email.sender 
                  << "\nReceiver: " << email.receiver 
                  << "\nSubject: " << email.subject 
                  << "\nBody: " << email.body 
                  << "\nTimestamp: " << email.timestamp 
                  << "\nStatus: " << email.status 
                  << "\nPriority: " << email.priority 
                  << "\nSpam Status: " << email.spamStatus << "\n";
    }