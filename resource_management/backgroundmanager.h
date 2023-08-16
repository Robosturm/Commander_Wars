#ifndef BACKGROUNDMANAGER_H
#define BACKGROUNDMANAGER_H

#include "resource_management/ressourcemanagement.h"

class BackgroundManager final : public QObject, public RessourceManagement<BackgroundManager>
{
    Q_OBJECT
protected:
    friend RessourceManagement<BackgroundManager>;
    BackgroundManager();
private:
    ~BackgroundManager() = default;
};

#endif // BACKGROUNDMANAGER_H
