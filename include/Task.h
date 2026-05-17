#pragma once
#include "../include/Entity.h"
#include "../include/Tag.h"
#include <string>
#include <vector>
#include <memory>
#include <ctime>

enum class Priority { LOW = 0, MEDIUM = 1, HIGH = 2, CRITICAL = 3 };
enum class Status   { TODO = 0, IN_PROGRESS = 1, DONE = 2, CANCELLED = 3 };

std::string priorityToStr(Priority p);
std::string statusToStr(Status s);
Priority    strToPriority(int n);
Status      strToStatus(int n);

class User;

class Task : public Entity {
private:
    std::string title;
    std::string description;
    Priority priority;
    Status status;
    std::time_t dueDate;
    std::shared_ptr<User> assignee;
    std::vector<Tag> tags;

public:
    Task(int id, const std::string& title, const std::string& description,
         Priority priority, std::time_t dueDate);

    std::string getTitle()       const;
    std::string getDescription() const;
    Priority    getPriority()    const;
    Status      getStatus()      const;
    std::time_t getDueDate()     const;
    std::string getDueDateStr()  const;
    std::shared_ptr<User> getAssignee() const;
    const std::vector<Tag>& getTags()   const;

    void changeStatus(Status newStatus);
    void assignTo(std::shared_ptr<User> user);
    void addTag(const Tag& tag);
    bool isOverdue() const;

    std::string toString() const override;
};