#include "priorityHandling.h"

using namespace std;


int main() {
    const string emailFile = "../Data/email.txt";

    PriorityQueue pq;

    // Load emails from the file and add them to the priority queue
    pq.loadEmailsFromFile(emailFile);

    // Display all emails in the priority queue
    pq.displayAllEmailsByPriority();

    // Process the highest priority email
    std::cout << "\nProcessing highest priority email:\n";
    pq.processTopPriorityEmail();

    return 0;
}
