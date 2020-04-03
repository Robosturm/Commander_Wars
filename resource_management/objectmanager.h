#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "resource_management/RessourceManagement.h"

class ObjectManager : public QObject, public RessourceManagement<ObjectManager>
{
    Q_OBJECT
public:
    static oxygine::spButton createButton(QString text, qint32 width = -1);
protected:
    friend RessourceManagement<ObjectManager>;
    ObjectManager();
private:
    virtual ~ObjectManager() = default;
};


#endif // OBJECTMANAGER_H
