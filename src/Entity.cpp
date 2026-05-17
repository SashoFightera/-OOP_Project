#include "../include/Entity.h"
#include <ctime>

Entity::Entity(int id) : id(id), createdAt(std::time(nullptr)) {}

int Entity::getId() const { return id; }

std::string Entity::getCreatedAtStr() const {
    char buf[20];
    std::tm tm_val = *std::localtime(&createdAt);
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", &tm_val);
    return std::string(buf);
}