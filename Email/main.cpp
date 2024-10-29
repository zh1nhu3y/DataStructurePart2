#include "spamDetection.h"

int main()
{
    EmailQueue emailQueue;
    SpamDetection spamDetection;
    string spamWords[MAX_SPAM_WORDS];

    // Spam word input file
    const string spamFile = "../Data/spam_words.txt";
    const string emailFile = "../Data/email.txt";

    // Load spam words from a file
    int spamWordCount = spamDetection.loadSpamWords(spamFile, spamWords, MAX_SPAM_WORDS);

    // Sort spam words for binary search
    spamDetection.quickSort(spamWords, 0, spamWordCount - 1);

    // Load email from email file
    spamDetection.enqueueEmail(emailFile, emailQueue);

    // Check for spam status
    spamDetection.checkSpamStatus(emailQueue, spamWords, spamWordCount);

    // Print
    emailQueue.printEmails();

    return 0;
}