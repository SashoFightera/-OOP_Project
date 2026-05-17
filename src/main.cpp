#include <iostream>
#include <string>
#include <ctime>
#include <stdexcept>
#include "../include/TaskManager.h"

std::time_t parseDate(const std::string& s) {
    std::tm tm = {};
    int y, m, d;
    if (sscanf(s.c_str(), "%d-%d-%d", &y, &m, &d) != 3)
        throw std::invalid_argument("Use format YYYY-MM-DD");
    tm.tm_year = y - 1900;
    tm.tm_mon  = m - 1;
    tm.tm_mday = d;
    tm.tm_hour = 23;
    tm.tm_min  = 59;
    return std::mktime(&tm);
}

void printTasks(const std::vector<std::shared_ptr<Task>>& tasks) {
    if (tasks.empty()) { std::cout << "  (no tasks found)\n"; return; }
    for (const auto& t : tasks) std::cout << t->toString() << "\n\n";
}

void printMenu() {
    std::cout << "\n"
        << "╔══════════════════════════════════╗\n"
        << "║         TASK MANAGER             ║\n"
        << "╠══════════════════════════════════╣\n"
        << "║  PROJECTS                        ║\n"
        << "║   1. Create project              ║\n"
        << "║   2. List all projects           ║\n"
        << "║   3. Delete project              ║\n"
        << "╠══════════════════════════════════╣\n"
        << "║  TASKS                           ║\n"
        << "║   4. Create task                 ║\n"
        << "║   5. List tasks in project       ║\n"
        << "║   6. Delete task                 ║\n"
        << "║   7. Change task status          ║\n"
        << "║   8. Assign task to user         ║\n"
        << "║   9. Add tag to task             ║\n"
        << "╠══════════════════════════════════╣\n"
        << "║  FILTER & SORT                   ║\n"
        << "║  10. Filter by status            ║\n"
        << "║  11. Filter by priority          ║\n"
        << "║  12. Filter by assignee          ║\n"
        << "║  13. Filter by tag               ║\n"
        << "║  14. Sort by due date            ║\n"
        << "║  15. Sort by priority            ║\n"
        << "║  16. Show overdue tasks          ║\n"
        << "╠══════════════════════════════════╣\n"
        << "║  OTHER                           ║\n"
        << "║  17. Show statistics             ║\n"
        << "║  18. Show change log             ║\n"
        << "║  19. Create user                 ║\n"
        << "║  20. List users                  ║\n"
        << "║   0. Exit                        ║\n"
        << "╚══════════════════════════════════╝\n"
        << "Choice: ";
}

int main() {
    TaskManager tm;

    // Seed data
    auto u1 = tm.createUser("alice", "alice@example.com");
    auto u2 = tm.createUser("bob",   "bob@example.com");
    auto p1 = tm.createProject("Website Redesign",
                               "Redesign the company website",
                               parseDate("2025-08-01"));
    tm.createTask(p1->getId(), "Design mockups",
                  "Create Figma mockups for all pages",
                  Priority::HIGH, parseDate("2025-06-15"));
    tm.createTask(p1->getId(), "Setup CI/CD",
                  "Configure GitHub Actions pipeline",
                  Priority::MEDIUM, parseDate("2024-12-01")); // overdue

    std::cout << "Welcome to Task Manager!\n";
    std::cout << "Seed data loaded: 2 users, 1 project, 2 tasks.\n";

    int choice;
    while (true) {
        printMenu();
        if (!(std::cin >> choice)) { std::cin.clear(); std::cin.ignore(1000,'\n'); continue; }
        std::cin.ignore();

        try {
            if (choice == 0) break;

            // ── Projects ──────────────────────────────────────
            else if (choice == 1) {
                std::string name, desc, date;
                std::cout << "Name: ";     std::getline(std::cin, name);
                std::cout << "Desc: ";     std::getline(std::cin, desc);
                std::cout << "Deadline (YYYY-MM-DD): "; std::getline(std::cin, date);
                auto p = tm.createProject(name, desc, parseDate(date));
                std::cout << "Created:\n" << p->toString() << "\n";
            }
            else if (choice == 2) {
                tm.listProjects();
            }
            else if (choice == 3) {
                int pid;
                std::cout << "Project ID to delete: "; std::cin >> pid; std::cin.ignore();
                tm.deleteProject(pid);
                std::cout << "Project deleted.\n";
            }

            // ── Tasks ─────────────────────────────────────────
            else if (choice == 4) {
                int pid, pri;
                std::string title, desc, date;
                std::cout << "Project ID: ";  std::cin >> pid; std::cin.ignore();
                std::cout << "Title: ";       std::getline(std::cin, title);
                std::cout << "Desc: ";        std::getline(std::cin, desc);
                std::cout << "Priority (0=LOW 1=MEDIUM 2=HIGH 3=CRITICAL): ";
                std::cin >> pri; std::cin.ignore();
                std::cout << "Due (YYYY-MM-DD): "; std::getline(std::cin, date);
                auto t = tm.createTask(pid, title, desc,
                                       strToPriority(pri), parseDate(date));
                std::cout << "Created:\n" << t->toString() << "\n";
            }
            else if (choice == 5) {
                int pid;
                std::cout << "Project ID: "; std::cin >> pid; std::cin.ignore();
                auto p = tm.findProject(pid);
                if (!p) { std::cout << "Project not found.\n"; continue; }
                printTasks(p->getTasks());
            }
            else if (choice == 6) {
                int pid, tid;
                std::cout << "Project ID: "; std::cin >> pid;
                std::cout << "Task ID: ";    std::cin >> tid; std::cin.ignore();
                tm.deleteTask(pid, tid);
                std::cout << "Task deleted.\n";
            }
            else if (choice == 7) {
                int pid, tid, uid, st;
                std::cout << "Project ID: "; std::cin >> pid;
                std::cout << "Task ID: ";    std::cin >> tid;
                std::cout << "Your User ID: "; std::cin >> uid;
                std::cout << "New status (0=TODO 1=IN_PROGRESS 2=DONE 3=CANCELLED): ";
                std::cin >> st; std::cin.ignore();
                tm.changeTaskStatus(pid, tid, strToStatus(st), uid);
                std::cout << "Status updated.\n";
            }
            else if (choice == 8) {
                int pid, tid, uid;
                std::cout << "Project ID: "; std::cin >> pid;
                std::cout << "Task ID: ";    std::cin >> tid;
                std::cout << "User ID: ";    std::cin >> uid; std::cin.ignore();
                tm.assignTask(pid, tid, uid);
                std::cout << "Task assigned.\n";
            }
            else if (choice == 9) {
                int pid, tid;
                std::string tag;
                std::cout << "Project ID: "; std::cin >> pid;
                std::cout << "Task ID: ";    std::cin >> tid; std::cin.ignore();
                std::cout << "Tag name: ";   std::getline(std::cin, tag);
                tm.addTagToTask(pid, tid, tag);
                std::cout << "Tag added.\n";
            }

            // ── Filter & Sort ─────────────────────────────────
            else if (choice == 10) {
                int pid, st;
                std::cout << "Project ID: "; std::cin >> pid;
                std::cout << "Status (0=TODO 1=IN_PROGRESS 2=DONE 3=CANCELLED): ";
                std::cin >> st; std::cin.ignore();
                printTasks(tm.filterByStatus(pid, strToStatus(st)));
            }
            else if (choice == 11) {
                int pid, pri;
                std::cout << "Project ID: "; std::cin >> pid;
                std::cout << "Priority (0=LOW 1=MEDIUM 2=HIGH 3=CRITICAL): ";
                std::cin >> pri; std::cin.ignore();
                printTasks(tm.filterByPriority(pid, strToPriority(pri)));
            }
            else if (choice == 12) {
                int pid, uid;
                std::cout << "Project ID: "; std::cin >> pid;
                std::cout << "User ID: ";    std::cin >> uid; std::cin.ignore();
                printTasks(tm.filterByAssignee(pid, uid));
            }
            else if (choice == 13) {
                int pid;
                std::string tag;
                std::cout << "Project ID: "; std::cin >> pid; std::cin.ignore();
                std::cout << "Tag name: ";   std::getline(std::cin, tag);
                printTasks(tm.filterByTag(pid, tag));
            }
            else if (choice == 14) {
                int pid;
                std::cout << "Project ID: "; std::cin >> pid; std::cin.ignore();
                printTasks(tm.sortByDueDate(pid));
            }
            else if (choice == 15) {
                int pid;
                std::cout << "Project ID: "; std::cin >> pid; std::cin.ignore();
                printTasks(tm.sortByPriority(pid));
            }
            else if (choice == 16) {
                int pid;
                std::cout << "Project ID: "; std::cin >> pid; std::cin.ignore();
                printTasks(tm.getOverdueTasks(pid));
            }

            // ── Other ─────────────────────────────────────────
            else if (choice == 17) {
                int pid;
                std::cout << "Project ID: "; std::cin >> pid; std::cin.ignore();
                tm.printStatistics(pid);
            }
            else if (choice == 18) {
                tm.printChangeLogs();
            }
            else if (choice == 19) {
                std::string username, email;
                std::cout << "Username: "; std::getline(std::cin, username);
                std::cout << "Email: ";    std::getline(std::cin, email);
                auto u = tm.createUser(username, email);
                std::cout << "Created: " << u->toString() << "\n";
            }
            else if (choice == 20) {
                tm.listUsers();
            }
            else {
                std::cout << "Unknown option.\n";
            }

        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    std::cout << "\nGoodbye!\n";
    return 0;
}