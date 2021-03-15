#include "3rd_party/oxygine-framework/oxygine/core/Object.h"
#include "3rd_party/oxygine-framework/oxygine/EventDispatcher.h"
#include <qmutex.h>
#include <QMutexLocker>

namespace oxygine
{    
    Object::Object(const Object& src)
        : __name("")
    {
        setName(src.__name);
    }

    Object::Object()
    {
    }

    QString Object::getName() const
    {
        return __name;
    }

    bool Object::isName(QString name) const
    {
        return name == __name;
    }

    void Object::setName(QString name)
    {
        __name = name;
    }
}
