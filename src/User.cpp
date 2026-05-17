#include "../include/User.h"

User::User(int id, const std::string& username, const std::string& email)
    : Entity(id), username(username), email(email) {}

std::string User::getUsername() const { return username; }
std::string User::getEmail()    const { return email; }

std::string User::toString() const {
    return "[User #" + std::to_string(id) + "] "
         + username + " <" + email + ">";
}