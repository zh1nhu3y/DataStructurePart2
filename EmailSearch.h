// EmailSearch.h

#ifndef EMAILSEARCH_H
#define EMAILSEARCH_H

#include <string>
#include "emailQueue.h"


using namespace std;

// Function declarations
EmailNode* linearSearch(EmailNode* head, const string& criteria, const string& type);
EmailNode* binarySearch(EmailNode* head, int size, const string& criteria, const string& type);
void mergeSort(EmailNode** headRef);
void splitList(EmailNode* source, EmailNode** front, EmailNode** back);
EmailNode* sortedMerge(EmailNode* a, EmailNode* b);

#endif // EMAILSEARCH_H
