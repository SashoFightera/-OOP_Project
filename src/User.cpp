#include "../include/User.h"
#include <stdexcept>

User::User(int id, const std::string& username, const std::string& email)
    : Entity(id), username(username), email(email) {
    if (email.find('@') == std::string::npos)
        throw std::invalid_argument("Invalid email: missing '@'");
}

std::string User::getUsername() const { return username; }
std::string User::getEmail()    const { return email; }

std::string User::toString() const {
    return "[User #" + std::to_string(id) + "] "
         + username + " <" + email + ">";
}