#pragma once
#include "../include/Entity.h"
#include "../include/Task.h"
#include <string>
#include <vector>
#include <memory>
#include <ctime>

class Project : public Entity {
private:
    std::string name;
    std::string description;
    std::time_t deadline;
    std::vector<std::shared_ptr<Task>> tasks;

public:
    Project(int id, const std::string& name,
            const std::string& description, std::time_t deadline);

    std::string getName()        const;
    std::string getDescription() const;
    std::string getDeadlineStr() const;
    const std::vector<std::shared_ptr<Task>>& getTasks() const;

    void addTask(std::shared_ptr<Task> task);
    void removeTask(int taskId);
    std::shared_ptr<Task> findTask(int taskId) const;

    int    countByStatus(Status s) const;
    double completionPercent()     const;

    std::string toString() const override;
};