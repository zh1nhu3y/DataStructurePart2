#ifndef USERAUTH_H
#define USERAUTH_H

#include <string>
#include <fstream>

class UserAuth {
private:
    std::string loggedInEmail;  // Store the logged-in user's email

public:
    bool login(const std::string& username, const std::string& password);
    std::string getLoggedInEmail() const { return loggedInEmail; }  // Getter for logged-in email
};

#endif // USERAUTH_H
