#ifndef COPERKMANAGER_H
#define COPERKMANAGER_H

#include "resource_management/RessourceManagement.h"

class COPerkManager : public QObject, public RessourceManagement<COPerkManager>
{
    Q_OBJECT
protected:
    friend RessourceManagement<COPerkManager>;
    COPerkManager();
private:
    virtual ~COPerkManager() = default;
};

#endif // COPERKMANAGER_H
