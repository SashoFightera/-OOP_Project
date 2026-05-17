#pragma once
#include "../include/Entity.h"
#include "../include/Task.h"
#include <string>

class ChangeLog : public Entity {
private:
    int         taskId;
    std::string taskTitle;
    std::string changedByUsername;
    Status      oldStatus;
    Status      newStatus;

public:
    ChangeLog(int id, int taskId, const std::string& taskTitle,
              const std::string& changedByUsername,
              Status oldStatus, Status newStatus);

    std::string toString() const override;
};