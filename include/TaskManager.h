#pragma once
#include "../include/Project.h"
#include "../include/User.h"
#include "../include/ChangeLog.h"
#include <vector>
#include <memory>
#include <string>

class TaskManager {
private:
    std::vector<std::shared_ptr<Project>> projects;
    std::vector<std::shared_ptr<User>>    users;
    std::vector<ChangeLog>                changeLogs;
    int nextId;

    int generateId();

public:
    TaskManager();

    // Projects
    std::shared_ptr<Project> createProject(const std::string& name,
                                           const std::string& desc,
                                           std::time_t deadline);
    std::shared_ptr<Project> findProject(int id) const;
    void listProjects() const;
    void deleteProject(int id);

    // Users
    std::shared_ptr<User> createUser(const std::string& username,
                                     const std::string& email);
    std::shared_ptr<User> findUser(int id) const;
    std::shared_ptr<User> findUserByUsername(const std::string& username) const;
    void listUsers() const;

    // Tasks
    std::shared_ptr<Task> createTask(int projectId,
                                     const std::string& title,
                                     const std::string& desc,
                                     Priority priority,
                                     std::time_t dueDate);
    void deleteTask(int projectId, int taskId);
    void changeTaskStatus(int projectId, int taskId,
                          Status newStatus, int changedByUserId);
    void assignTask(int projectId, int taskId, int userId);
    void addTagToTask(int projectId, int taskId, const std::string& tagName);

    // Filter & sort
    std::vector<std::shared_ptr<Task>> filterByStatus(int projectId, Status s) const;
    std::vector<std::shared_ptr<Task>> filterByPriority(int projectId, Priority p) const;
    std::vector<std::shared_ptr<Task>> filterByAssignee(int projectId, int userId) const;
    std::vector<std::shared_ptr<Task>> sortByDueDate(int projectId) const;
    std::vector<std::shared_ptr<Task>> sortByPriority(int projectId) const;
    std::vector<std::shared_ptr<Task>> getOverdueTasks(int projectId) const;
    std::vector<std::shared_ptr<Task>> filterByTag(int projectId, const std::string& tag) const;

    // Stats & logs
    void printStatistics(int projectId) const;
    void printChangeLogs() const;
};