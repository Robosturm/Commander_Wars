#pragma once

#include <type_traits>

#include <QObject>
#include <QColor>
#include "coreengine/interpreter.h"

#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"

class MemoryManagement final : public QObject
{
    Q_OBJECT
public:

    static MemoryManagement & getInstance()
    {
        return m_memoryManagement;
    }

    template <class T, typename ...TArgs>
    static std::shared_ptr<T> createNamedQObject(const QString & name, TArgs... args)
    {
        ++m_objectCounter;
        std::shared_ptr<T> pRet(new T(args...), &MemoryManagement::deleter<T>);
        pRet->setObjectName(name);
        Q_ASSERT(QJSEngine::objectOwnership(pRet.get()) == QJSEngine::ObjectOwnership::CppOwnership);
#ifdef MEMORYTRACING
            printCreationInfo(pRet.get());
#endif
        return pRet;
    }

    template <class T, typename ...TArgs>
    static std::shared_ptr<T> create(TArgs... args)
    {
        ++m_objectCounter;
        std::shared_ptr<T> pRet(new T(args...), &MemoryManagement::deleter<T>);
        if constexpr (std::is_base_of_v<QObject, T>)
        {
            Q_ASSERT(QJSEngine::objectOwnership(pRet.get()) == QJSEngine::ObjectOwnership::CppOwnership);
#ifdef MEMORYTRACING
            printCreationInfo(pRet.get());
#endif
        }
        else
        {
#ifdef MEMORYTRACING
            printCreationInfo(T::getTypeName());
#endif
        }
        return pRet;
    }
    
    template <class T>
    static std::shared_ptr<T> createNamedFromPointer(const QString & name, T* pPtr)
    {
        ++m_objectCounter;
        std::shared_ptr<T> pRet(pPtr, &MemoryManagement::deleter<T>);
        if (pPtr != nullptr)
        {
            pRet->setObjectName(name);
        }
        return pRet;
    }

    template <class T, typename ...TArgs>
    static T* createAndTrackJsObject(TArgs... args)
    {
        ++m_objectCounter;
        std::shared_ptr<T> pRet(new T(args...), &MemoryManagement::deleter<T>);
        Q_ASSERT(QJSEngine::objectOwnership(pRet.get()) == QJSEngine::ObjectOwnership::CppOwnership);
#ifdef MEMORYTRACING
        printCreationInfo(pRet.get());
#endif
        Interpreter* pInterpreter = Interpreter::getInstance();
        Q_ASSERT(pInterpreter->getInJsCall());
        pInterpreter->trackJsObject(pRet);
        return pRet.get();
    }

    template <class T>
    static void deleter(T * pObj)
    {
        --m_objectCounter;
        if constexpr (std::is_base_of_v<QObject, T>)
        {
#ifdef MEMORYTRACING
            printDeletionInfo(pObj);
#endif
            pObj->deleteLater();
        }
        else
        {
#ifdef MEMORYTRACING
            printDeletionInfo(T::getTypeName());
#endif
            delete pObj;
        }
    }

    static quint32 getObjectCounter();

    static void printDeletionInfo(QObject* obj);
    static void printDeletionInfo(const QString & name);
    static void printCreationInfo(QObject* obj);
    static void printCreationInfo(const QString & name);
private:
    explicit MemoryManagement();

private:
    static MemoryManagement m_memoryManagement;
    static std::atomic<quint32> m_objectCounter;
};
