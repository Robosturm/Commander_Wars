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
    ObjectBase::__createdObjects&   ObjectBase::__getCreatedObjects()
    {
        static __createdObjects __objects;
        return __objects;
    }

    int ObjectBase::_lastID = 0;
    int ObjectBase::_assertCtorID = -1;
    int ObjectBase::_assertDtorID = -1;

    QString ObjectBase::getName() const
    {
        return __name;
    }

    bool ObjectBase::isName(QString name) const
    {
        return name == __name;
    }

    void ObjectBase::setName(QString name)
    {
        __name = name;
    }

    ObjectBase::ObjectBase(const ObjectBase& src)
        : __name(""),
          __id(0)
    {
        __userData64 = src.__userData64;
        setName(src.__name);
        __generateID();
    }

    void ObjectBase::__generateID()
    {
        __id = ++_lastID;
    }

    ObjectBase::ObjectBase(bool assignID): __id(0)
    {
        __userData64 = 0;

        if (assignID)
            __generateID();
    }

    void ObjectBase::showAssertInCtor(int id)
    {
        _assertCtorID = id;
    }

    void ObjectBase::showAssertInDtor(int id)
    {
        _assertDtorID = id;
    }

    ObjectBase::~ObjectBase()
    {

    }

    Object::Object(const Object& src): ObjectBase(src)
    {
    }

    Object::Object(bool assignUniqueID): ObjectBase(assignUniqueID)
    {
    }
}
