#pragma once
#include <string>
#include <ctime>

class Entity {
protected:
    int id;
    std::time_t createdAt;

public:
    explicit Entity(int id);
    virtual ~Entity() = default;

    int getId() const;
    std::string getCreatedAtStr() const;

    virtual std::string toString() const = 0;
};