#include "../include/Task.h"
#include "../include/User.h"
#include <stdexcept>

std::string priorityToStr(Priority p) {
    switch (p) {
        case Priority::LOW:      return "LOW";
        case Priority::MEDIUM:   return "MEDIUM";
        case Priority::HIGH:     return "HIGH";
        case Priority::CRITICAL: return "CRITICAL";
        default:                 return "UNKNOWN";
    }
}

std::string statusToStr(Status s) {
    switch (s) {
        case Status::TODO:        return "TODO";
        case Status::IN_PROGRESS: return "IN_PROGRESS";
        case Status::DONE:        return "DONE";
        case Status::CANCELLED:   return "CANCELLED";
        default:                  return "UNKNOWN";
    }
}

Priority strToPriority(int n) {
    if (n < 0 || n > 3) throw std::invalid_argument("Priority must be 0-3");
    return static_cast<Priority>(n);
}

Status strToStatus(int n) {
    if (n < 0 || n > 3) throw std::invalid_argument("Status must be 0-3");
    return static_cast<Status>(n);
}

Task::Task(int id, const std::string& title, const std::string& description,
           Priority priority, std::time_t dueDate)
    : Entity(id), title(title), description(description),
      priority(priority), status(Status::TODO),
      dueDate(dueDate), assignee(nullptr) {}

std::string Task::getTitle()       const { return title; }
std::string Task::getDescription() const { return description; }
Priority    Task::getPriority()    const { return priority; }
Status      Task::getStatus()      const { return status; }
std::time_t Task::getDueDate()     const { return dueDate; }

std::string Task::getDueDateStr() const {
    char buf[20];
    std::tm tm_val = *std::localtime(&dueDate);
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", &tm_val);
    return std::string(buf);
}

std::shared_ptr<User> Task::getAssignee()     const { return assignee; }
const std::vector<Tag>& Task::getTags()       const { return tags; }

void Task::changeStatus(Status newStatus) {
    if (status == Status::DONE)
        throw std::logic_error("Task is already DONE and cannot be changed.");
    if (status == Status::CANCELLED)
        throw std::logic_error("Task is CANCELLED and cannot be changed.");
    if (status == newStatus)
        throw std::logic_error("Task already has this status.");  
    status = newStatus;
}

void Task::assignTo(std::shared_ptr<User> user) {
    assignee = user;
}

void Task::addTag(const Tag& tag) {
    tags.push_back(tag);
}

bool Task::isOverdue() const {
    return std::time(nullptr) > dueDate
        && status != Status::DONE
        && status != Status::CANCELLED;
}

std::string Task::toString() const {
    std::string s =
        "  [Task #" + std::to_string(id) + "] " + title +
        "\n    Desc:     " + description +
        "\n    Status:   " + statusToStr(status) +
        "\n    Priority: " + priorityToStr(priority) +
        "\n    Due:      " + getDueDateStr() +
        "\n    Assignee: " + (assignee ? assignee->getUsername() : "None");

    if (!tags.empty()) {
        s += "\n    Tags:     ";
        for (const auto& t : tags) s += "[" + t.getName() + "] ";
    }

    if (isOverdue()) s += "\n    *** OVERDUE ***";

    return s;
}