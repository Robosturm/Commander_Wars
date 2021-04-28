#include "3rd_party/oxygine-framework/oxygine/core/Object.h"
#include "3rd_party/oxygine-framework/oxygine/EventDispatcher.h"
#include <qmutex.h>
#include <QMutexLocker>

namespace oxygine
{    
    Object::Object(const Object& src)
        : m_name("")
    {
        setName(src.m_name);
    }

    Object::Object()
    {
    }

    QString Object::getName() const
    {
        return m_name;
    }

    bool Object::isName(QString name) const
    {
        return name == m_name;
    }

    void Object::setName(QString name)
    {
        m_name = name;
    }
}
