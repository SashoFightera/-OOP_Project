#include "../include/Tag.h"
#include <stdexcept>

Tag::Tag(const std::string& name, const std::string& color)
    : name(name), color(color) {
        if (name.empty())
        throw std::invalid_argument("Tag name cannot be empty.");
    }

std::string Tag::getName()  const { return name; }
std::string Tag::getColor() const { return color; }