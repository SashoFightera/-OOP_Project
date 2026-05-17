#include "../include/Project.h"
#include <stdexcept>
#include <algorithm>

Project::Project(int id, const std::string& name,
                 const std::string& description, std::time_t deadline)
    : Entity(id), name(name), description(description), deadline(deadline) {}

std::string Project::getName()        const { return name; }
std::string Project::getDescription() const { return description; }

std::string Project::getDeadlineStr() const {
    char buf[20];
    std::tm tm_val = *std::localtime(&deadline);
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", &tm_val);
    return std::string(buf);
}

const std::vector<std::shared_ptr<Task>>& Project::getTasks() const {
    return tasks;
}

void Project::addTask(std::shared_ptr<Task> task) {
    tasks.push_back(task);
}

void Project::removeTask(int taskId) {
    auto it = std::remove_if(tasks.begin(), tasks.end(),
        [taskId](const std::shared_ptr<Task>& t) {
            return t->getId() == taskId;
        });
    if (it == tasks.end())
        throw std::runtime_error("Task #" + std::to_string(taskId) + " not found.");
    tasks.erase(it, tasks.end());
}

std::shared_ptr<Task> Project::findTask(int taskId) const {
    for (const auto& t : tasks)
        if (t->getId() == taskId) return t;
    return nullptr;
}

int Project::countByStatus(Status s) const {
    int count = 0;
    for (const auto& t : tasks)
        if (t->getStatus() == s) count++;
    return count;
}

double Project::completionPercent() const {
    if (tasks.empty()) return 0.0;
    return 100.0 * countByStatus(Status::DONE) / tasks.size();
}

std::string Project::toString() const {
    return "[Project #" + std::to_string(id) + "] " + name +
           "\n  Desc:       " + description +
           "\n  Deadline:   " + getDeadlineStr() +
           "\n  Tasks:      " + std::to_string(tasks.size()) +
           "\n  Completion: " + std::to_string((int)completionPercent()) + "%";
}