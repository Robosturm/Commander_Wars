#ifndef MemoryManagement_H
#define MemoryManagement_H

#include <QObject>
#include "coreengine/interpreter.h"

template <class Base>
class RefObject : public std::enable_shared_from_this<Base>
{
public:
    template <class Derived>
    std::shared_ptr<Derived> getSharedPtr()
    {
        return std::static_pointer_cast<Derived>(std::enable_shared_from_this<Base>::shared_from_this());
    }
};

class MemoryManagement final : public QObject
{
    Q_OBJECT
public:

    template <class T, typename ...TArgs>
    static std::shared_ptr<T> create(TArgs... args)
    {
        std::shared_ptr<T> pRet(new T(args...), &MemoryManagement::deleter<T>);
        return pRet;
    }

    template <class T, typename ...TArgs>
    static T* createAndTrackJsObject(TArgs... args)
    {
        std::shared_ptr<T> pRet(new T(args...), &MemoryManagement::deleter<T>);
        Interpreter* pInterpreter = Interpreter::getInstance();
        Q_ASSERT(pInterpreter->getInJsCall());
        pInterpreter->trackJsObject(pRet);
        return pRet.get();
    }

    template <class T>
    static void deleter(T * pObj)
    {
        if constexpr (std::is_base_of_v<T, QObject>)
        {
            pObj->deleteLater();
        }
        else
        {
            delete pObj;
        }
    }
};

#endif // TIMER_H
