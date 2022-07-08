#pragma once

#include "resource_management/ressourcemanagement.h"

class UiManager : public QObject, public RessourceManagement<UiManager>
{
    Q_OBJECT
protected:
    friend RessourceManagement<UiManager>;
    UiManager();
private:
    virtual ~UiManager() = default;
};
