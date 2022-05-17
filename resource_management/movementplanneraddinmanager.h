#pragma once

#include "resource_management/ressourcemanagement.h"

class MovementPlannerAddInManager : public QObject, public RessourceManagement<MovementPlannerAddInManager>
{
    Q_OBJECT
public:

protected:
    friend RessourceManagement<MovementPlannerAddInManager>;
    MovementPlannerAddInManager();

private:
    virtual ~MovementPlannerAddInManager() = default;
};

