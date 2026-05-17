#include "../include/ChangeLog.h"

ChangeLog::ChangeLog(int id, int taskId, const std::string& taskTitle,
                     const std::string& changedByUsername,
                     Status oldStatus, Status newStatus)
    : Entity(id), taskId(taskId), taskTitle(taskTitle),
      changedByUsername(changedByUsername),
      oldStatus(oldStatus), newStatus(newStatus) {}

std::string ChangeLog::toString() const {
    return "[Log #" + std::to_string(id) + "] " + getCreatedAtStr()
         + "  |  Task: \"" + taskTitle + "\""
         + "  |  " + statusToStr(oldStatus) + " -> " + statusToStr(newStatus)
         + "  |  By: " + changedByUsername;
}