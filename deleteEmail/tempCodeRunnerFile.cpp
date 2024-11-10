void restoreEmails(PriorityQueue &pq, const string &emailFile, const string &recycleBinFile, const string &currentUser) {
    if (restoreEmailById(recycleBinFile, emailFile)) {
        cout << "The selected email has been restored successfully.\n";
        pq.clear(); // Clear the current emails in memory
        pq.loadUserEmails(emailFile, currentUser); // Reload the emails after the restoration
    } else {
        cout << "No email restored.\n";
    }
}