#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <QString>

#include "oxygine-framework.h"


class ObjectManager : public oxygine::Resources
{
public:
    static ObjectManager* getInstance();

    static oxygine::spButton createButton(QString text, qint32 width = -1);
private:
    explicit ObjectManager();
    virtual ~ObjectManager() = default;
    static ObjectManager* m_pInstance;
};

#endif // OBJECTMANAGER_H
