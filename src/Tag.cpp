#include "../include/Tag.h"

Tag::Tag(const std::string& name, const std::string& color)
    : name(name), color(color) {}

std::string Tag::getName()  const { return name; }
std::string Tag::getColor() const { return color; }