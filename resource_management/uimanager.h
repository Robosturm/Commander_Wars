#pragma once

#include "resource_management/ressourcemanagement.h"

class UiManager final : public QObject, public RessourceManagement<UiManager>
{
    Q_OBJECT
protected:
    friend MemoryManagement;
    UiManager();
private:
    virtual ~UiManager() = default;
};
