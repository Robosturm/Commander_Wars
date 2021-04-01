#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "resource_management/RessourceManagement.h"

class ObjectManager : public QObject, public RessourceManagement<ObjectManager>
{
    Q_OBJECT
public:
    static oxygine::spButton createButton(QString text, qint32 width = -1, QString tooltip = "", QString resAnim = "button");
    static oxygine::spButton createIconButton(QString icon);
    static oxygine::spButton createIconButton(oxygine::spSprite pSprite);
protected:
    friend RessourceManagement<ObjectManager>;
    ObjectManager();
private:
    virtual ~ObjectManager() = default;
};


#endif // OBJECTMANAGER_H
