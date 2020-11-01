#include "Object.h"
#include <qmutex.h>
#include <QMutexLocker>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <typeinfo>
#include "../EventDispatcher.h"

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
