#pragma once
#include <string>

class Tag {
private:
    std::string name;
    std::string color;

public:
    Tag(const std::string& name, const std::string& color = "white");
    std::string getName() const;
    std::string getColor() const;
};