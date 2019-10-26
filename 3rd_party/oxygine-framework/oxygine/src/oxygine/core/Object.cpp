#include "Object.h"
#include <qmutex.h>
#include <QMutexLocker>
#include "log.h"
#include "../utils/stringUtils.h"
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
    bool _tracingLeaks = false;



    //Mutex mutexDebugList;

    QMutex& getMutex()
    {
        static QMutex mutex;
        return mutex;
    }

    const std::string& ObjectBase::getName() const
    {
        return __name;
    }

    bool ObjectBase::isName(const std::string& name) const
    {
        return isName(name.c_str());
    }

    bool ObjectBase::isName(const char* name) const
    {
        if (__name == name)
        {
            return true;
        }
        return false;
    }

    void ObjectBase::setName(const std::string& name)
    {
        __name = name;
    }

    ObjectBase::ObjectBase(const ObjectBase& src)
        : __name(nullptr),
          __id(0)
    {
        __addToDebugList(this);

        __userData64 = src.__userData64;
        setName(src.__name);
        __generateID();
    }

    void ObjectBase::__removeFromDebugList()
    {
        __removeFromDebugList(this);
    }

    void ObjectBase::__generateID()
    {
        __id = ++_lastID;
    }

    ObjectBase::ObjectBase(bool assignID): __id(0)
    {
        __userData64 = 0;
        __addToDebugList(this);

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

    void ObjectBase::__startTracingLeaks()
    {
        _tracingLeaks = true;
    }

    void ObjectBase::__stopTracingLeaks()
    {
        _tracingLeaks = false;
    }

    void ObjectBase::__addToDebugList(ObjectBase* base)
    {
#ifdef OXYGINE_DEBUG_TRACE_LEAKS
        if (_tracingLeaks)
        {
            MutexAutoLock m(getMutex());
            base->__traceLeak = true;
            __getCreatedObjects().push_back(base);
        }
#endif
    }

    void ObjectBase::__removeFromDebugList(ObjectBase* base)
    {
#ifdef OXYGINE_DEBUG_TRACE_LEAKS
        if (_tracingLeaks)
        {
            MutexAutoLock m_(getMutex());
            if (base->__traceLeak)
            {
                base->__traceLeak = false;
                __createdObjects& objs = __getCreatedObjects();
                __createdObjects::iterator i = std::find(objs.begin(), objs.end(), base);
                OX_ASSERT(i != objs.end());
                objs.erase(i);
            }
        }
#endif
    }

    void ObjectBase::dumpObject() const
    {
        const Object* o = dynamic_cast<const Object*>(this);
        std::string name;

        char refs[16] = "-";
        if (o)
        {
            name = o->__name;
        }


        int cbs = 0;
        if (o && o->_ref_counter)
        {
            const EventDispatcher *ed = dynamic_cast<const EventDispatcher*>(o);
            if (ed)
                cbs = ed->getListenersCount();
        }

        logs::messageln("id = %d, name = '%s', typeid = '%s', callbacks = '%d' refs = %s", this->__id, name.c_str(), typeid(*this).name(), cbs, refs);
    }

    void ObjectBase::dumpCreatedObjects()
    {
#ifdef OXYGINE_DEBUG_TRACE_LEAKS
        MutexAutoLock m(getMutex());

        logs::messageln("\n\n\nallocated objects:");
        int n = 0;
        __createdObjects& objs = __getCreatedObjects();
        for (__createdObjects::iterator i = objs.begin(); i != objs.end(); ++i)
        {
            ObjectBase* object = *i;
            //logs::message("%d)", n);
            object->dumpObject();

            ++n;
        }
        logs::message("total: %d -----------------------------\n\n\n", (int)objs.size());
#endif
    }

    Object::Object(const Object& src): ObjectBase(src)
    {
#ifdef OX_DEBUG
        __check = 0xABCDEFAB;
#endif
    }

    Object::Object(bool assignUniqueID): ObjectBase(assignUniqueID)
    {
#ifdef OX_DEBUG
        __check = 0xABCDEFAB;
#endif
    }
#ifdef OX_DEBUG
    void Object::__doCheck()
    {
        OX_ASSERT(this);
        OX_ASSERT(__check == 0xABCDEFAB);
    }
#endif
}
