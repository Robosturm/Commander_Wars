#pragma once

#include "resource_management/ressourcemanagement.h"

class MovementPlannerAddInManager final : public QObject, public RessourceManagement<MovementPlannerAddInManager>
{
    Q_OBJECT

protected:
    friend MemoryManagement;
    MovementPlannerAddInManager();

private:
    virtual ~MovementPlannerAddInManager() = default;
};

