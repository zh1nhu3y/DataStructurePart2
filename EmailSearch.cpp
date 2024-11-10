// EmailSearch.cpp

#include "EmailSearch.h"

#include "EmailSearch.h"

#include <algorithm>
#include <cctype>

// Helper function to convert a string to lowercase
std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// Check if email matches keyword across multiple fields (general keyword search, case-insensitive)
bool matchesKeyword(const EmailNode* email, const std::string& keyword) {
    std::string lowerKeyword = toLower(keyword);
    return toLower(email->senderEmail).find(lowerKeyword) != std::string::npos ||
           toLower(email->title).find(lowerKeyword) != std::string::npos ||
           toLower(email->content).find(lowerKeyword) != std::string::npos ||
           toLower(email->dateTime).find(lowerKeyword) != std::string::npos;
}

// Prompt user for keyword input
string getUserKeyword() {
    string keyword;
    cout << "Enter the keyword you want to search for: ";
    getline(cin, keyword);
    return keyword;
}

// Linear search with keyword retrieval across multiple fields
EmailNode* keywordSearch(EmailNode* head) {
    string keyword = getUserKeyword();  // Get keyword from user
    EmailNode* current = head;
    EmailNode* resultHead = nullptr;    // Start of the result linked list
    EmailNode* resultTail = nullptr;    // Tail to add new matched nodes

    while (current != nullptr) {
        // Check if the current email matches the keyword in any of the specified fields
        if (matchesKeyword(current, keyword)) {
            // Create a new node for the result list
            EmailNode* matchedEmail = new EmailNode(*current);
            matchedEmail->next = nullptr;

            // Append matchedEmail to the result list
            if (resultHead == nullptr) {
                resultHead = matchedEmail;  // First match
                resultTail = resultHead;
            } else {
                resultTail->next = matchedEmail;
                resultTail = resultTail->next;
            }
        }
        current = current->next;
    }

    return resultHead; // Return head of the list containing matched results
}

