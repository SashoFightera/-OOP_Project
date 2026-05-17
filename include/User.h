#pragma once
#include "../include/Entity.h"
#include <string>

class User : public Entity {
private:
    std::string username;
    std::string email;

public:
    User(int id, const std::string& username, const std::string& email);

    std::string getUsername() const;
    std::string getEmail() const;

    std::string toString() const override;
};