#include "UserAuth.h"

bool UserAuth::login(const std::string& username, const std::string& password) {
    std::ifstream file("../Data/login.txt");
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t commaPos = line.find(',');
        if (commaPos == std::string::npos) continue;

        std::string storedEmail = line.substr(0, commaPos);
        std::string storedPassword = line.substr(commaPos + 1);

        if (storedEmail == username && storedPassword == password) {
            loggedInEmail = storedEmail;  // Store the email of the logged-in user
            return true;
        }
    }

    return false;
}
