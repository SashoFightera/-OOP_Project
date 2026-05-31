#include "../include/TaskManager.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>

TaskManager::TaskManager() : nextId(1) {}

int TaskManager::generateId() { return nextId++; }

// ── Projects ──────────────────────────────────────────────────────────────

std::shared_ptr<Project> TaskManager::createProject(
        const std::string& name, const std::string& desc, std::time_t deadline) {
    auto p = std::make_shared<Project>(generateId(), name, desc, deadline);
    projects.push_back(p);
    return p;
}

std::shared_ptr<Project> TaskManager::findProject(int id) const {
    for (const auto& p : projects)
        if (p->getId() == id) return p;
    return nullptr;
}

void TaskManager::listProjects() const {
    if (projects.empty()) { std::cout << "  (no projects)\n"; return; }
    for (const auto& p : projects)
        std::cout << p->toString() << "\n\n";
}

void TaskManager::deleteProject(int id) {
    auto it = std::remove_if(projects.begin(), projects.end(),
        [id](const std::shared_ptr<Project>& p) { return p->getId() == id; });
    if (it == projects.end())
        throw std::runtime_error("Project #" + std::to_string(id) + " not found.");
    projects.erase(it, projects.end());
}

// ── Users ─────────────────────────────────────────────────────────────────

std::shared_ptr<User> TaskManager::createUser(
        const std::string& username, const std::string& email) {
    auto u = std::make_shared<User>(generateId(), username, email);
    users.push_back(u);
    return u;
}

std::shared_ptr<User> TaskManager::findUser(int id) const {
    for (const auto& u : users)
        if (u->getId() == id) return u;
    return nullptr;
}
std::shared_ptr<User> TaskManager::findUserByUsername(const std::string& username) const {
    for (const auto& u : users)
        if (u->getUsername() == username) return u;
    return nullptr;
}

void TaskManager::listUsers() const {
    if (users.empty()) { std::cout << "  (no users)\n"; return; }
    for (const auto& u : users)
        std::cout << u->toString() << "\n";
}

// ── Tasks ─────────────────────────────────────────────────────────────────

std::shared_ptr<Task> TaskManager::createTask(
        int projectId, const std::string& title,
        const std::string& desc, Priority priority, std::time_t dueDate) {
    auto project = findProject(projectId);
    if (!project)
        throw std::runtime_error("Project #" + std::to_string(projectId) + " not found.");
    auto task = std::make_shared<Task>(generateId(), title, desc, priority, dueDate);
    project->addTask(task);
    return task;
}

void TaskManager::deleteTask(int projectId, int taskId) {
    auto project = findProject(projectId);
    if (!project)
        throw std::runtime_error("Project #" + std::to_string(projectId) + " not found.");
    project->removeTask(taskId);
}

void TaskManager::changeTaskStatus(int projectId, int taskId,
                                    Status newStatus, int changedByUserId) {
    auto project = findProject(projectId);
    if (!project) throw std::runtime_error("Project not found.");

    auto task = project->findTask(taskId);
    if (!task) throw std::runtime_error("Task not found.");

    auto user = findUser(changedByUserId);
    if (!user) throw std::runtime_error("User not found.");

    Status old = task->getStatus();
    task->changeStatus(newStatus);

    changeLogs.emplace_back(generateId(), task->getId(), task->getTitle(),
                            user->getUsername(), old, newStatus);
}

void TaskManager::assignTask(int projectId, int taskId, int userId) {
    auto project = findProject(projectId);
    if (!project) throw std::runtime_error("Project not found.");

    auto task = project->findTask(taskId);
    if (!task) throw std::runtime_error("Task not found.");

    auto user = findUser(userId);
    if (!user) throw std::runtime_error("User not found.");

    task->assignTo(user);

    // Log the assignment as a status-neutral entry reusing ChangeLog
    changeLogs.emplace_back(generateId(), task->getId(), task->getTitle(),
                            user->getUsername(),
                            task->getStatus(), task->getStatus());
}

void TaskManager::addTagToTask(int projectId, int taskId,
                                const std::string& tagName) {
    auto project = findProject(projectId);
    if (!project) throw std::runtime_error("Project not found.");

    auto task = project->findTask(taskId);
    if (!task) throw std::runtime_error("Task not found.");

    task->addTag(Tag(tagName));
}

// ── Filter & Sort ─────────────────────────────────────────────────────────

std::vector<std::shared_ptr<Task>> TaskManager::filterByStatus(
        int projectId, Status s) const {
    auto project = findProject(projectId);
    if (!project) throw std::runtime_error("Project not found.");
    std::vector<std::shared_ptr<Task>> result;
    for (const auto& t : project->getTasks())
        if (t->getStatus() == s) result.push_back(t);
    return result;
}

std::vector<std::shared_ptr<Task>> TaskManager::filterByPriority(
        int projectId, Priority p) const {
    auto project = findProject(projectId);
    if (!project) throw std::runtime_error("Project not found.");
    std::vector<std::shared_ptr<Task>> result;
    for (const auto& t : project->getTasks())
        if (t->getPriority() == p) result.push_back(t);
    return result;
}

std::vector<std::shared_ptr<Task>> TaskManager::filterByAssignee(
        int projectId, int userId) const {
    auto project = findProject(projectId);
    if (!project) throw std::runtime_error("Project not found.");
    auto user = findUser(userId);
    if (!user) throw std::runtime_error("User not found.");
    std::vector<std::shared_ptr<Task>> result;
    for (const auto& t : project->getTasks())
        if (t->getAssignee() && t->getAssignee()->getId() == userId)
            result.push_back(t);
    return result;
}

std::vector<std::shared_ptr<Task>> TaskManager::sortByDueDate(
        int projectId) const {
    auto project = findProject(projectId);
    if (!project) throw std::runtime_error("Project not found.");
    auto tasks = project->getTasks();
    std::sort(tasks.begin(), tasks.end(),
        [](const std::shared_ptr<Task>& a, const std::shared_ptr<Task>& b) {
            return a->getDueDate() < b->getDueDate();
        });
    return tasks;
}

std::vector<std::shared_ptr<Task>> TaskManager::sortByPriority(
        int projectId) const {
    auto project = findProject(projectId);
    if (!project) throw std::runtime_error("Project not found.");
    auto tasks = project->getTasks();
    std::sort(tasks.begin(), tasks.end(),
        [](const std::shared_ptr<Task>& a, const std::shared_ptr<Task>& b) {
            return static_cast<int>(a->getPriority())
                 > static_cast<int>(b->getPriority());
        });
    return tasks;
}

std::vector<std::shared_ptr<Task>> TaskManager::getOverdueTasks(
        int projectId) const {
    auto project = findProject(projectId);
    if (!project) throw std::runtime_error("Project not found.");
    std::vector<std::shared_ptr<Task>> result;
    for (const auto& t : project->getTasks())
        if (t->isOverdue()) result.push_back(t);
    return result;
}

std::vector<std::shared_ptr<Task>> TaskManager::filterByTag(
        int projectId, const std::string& tagName) const {
    auto project = findProject(projectId);
    if (!project) throw std::runtime_error("Project not found.");
    std::vector<std::shared_ptr<Task>> result;
    for (const auto& t : project->getTasks())
        for (const auto& tag : t->getTags())
            if (tag.getName() == tagName) { result.push_back(t); break; }
    return result;
}

// ── Statistics & Logs ─────────────────────────────────────────────────────

void TaskManager::printStatistics(int projectId) const {
    auto project = findProject(projectId);
    if (!project) throw std::runtime_error("Project not found.");

    std::cout << "\n=== Statistics: " << project->getName() << " ===\n";
    std::cout << "  TODO:        " << project->countByStatus(Status::TODO)        << "\n";
    std::cout << "  IN_PROGRESS: " << project->countByStatus(Status::IN_PROGRESS) << "\n";
    std::cout << "  DONE:        " << project->countByStatus(Status::DONE)        << "\n";
    std::cout << "  CANCELLED:   " << project->countByStatus(Status::CANCELLED)   << "\n";
    std::cout << "  Completion:  " << (int)project->completionPercent()           << "%\n";
}

void TaskManager::printChangeLogs() const {
    if (changeLogs.empty()) { std::cout << "  (no logs yet)\n"; return; }
    for (const auto& log : changeLogs)
        std::cout << log.toString() << "\n";
}