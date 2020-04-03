#ifndef BACKGROUNDMANAGER_H
#define BACKGROUNDMANAGER_H

#include "resource_management/RessourceManagement.h"

class BackgroundManager : public QObject, public RessourceManagement<BackgroundManager>
{
    Q_OBJECT
protected:
    friend RessourceManagement<BackgroundManager>;
    BackgroundManager();
private:
    virtual ~BackgroundManager() = default;
};

#endif // BACKGROUNDMANAGER_H
