#include "coreengine/memorymanagement.h"
#include "coreengine/mainapp.h"

MemoryManagement MemoryManagement::m_memoryManagement;
std::atomic<quint32> MemoryManagement::m_objectCounter{0};

MemoryManagement::MemoryManagement()
{
    Interpreter::setCppOwnerShip(this);
}

void MemoryManagement::printDeletionInfo(QObject* obj)
{
    Q_ASSERT(QJSEngine::objectOwnership(obj) == QJSEngine::ObjectOwnership::CppOwnership);
    if (obj->objectName().isEmpty())
    {
        CONSOLE_PRINT("Deleting object with no name from thread " + QThread::currentThread()->objectName() + " rendering paused " + QString::number(Mainapp::getInstance() != nullptr ? Mainapp::getInstance()->renderingPaused() : false), GameConsole::eDEBUG);    
    }
    else
    {
        CONSOLE_PRINT("Deleting object " + obj->objectName() + " from thread " + QThread::currentThread()->objectName() + " rendering paused " + QString::number(Mainapp::getInstance() != nullptr ? Mainapp::getInstance()->renderingPaused() : false), GameConsole::eDEBUG);    
    }
}

void MemoryManagement::printDeletionInfo(const QString & name)
{
    CONSOLE_PRINT("Deleting object " + name + " from thread " + QThread::currentThread()->objectName() + " rendering paused " + QString::number(Mainapp::getInstance() != nullptr ? Mainapp::getInstance()->renderingPaused() : false), GameConsole::eDEBUG);    
}

void MemoryManagement::printCreationInfo(QObject* obj)
{
    Q_ASSERT(QJSEngine::objectOwnership(obj) == QJSEngine::ObjectOwnership::CppOwnership);
    if (obj->objectName().isEmpty())
    {
        CONSOLE_PRINT("Creating object with no name from thread " + QThread::currentThread()->objectName() + " rendering paused " + QString::number(Mainapp::getInstance() != nullptr ? Mainapp::getInstance()->renderingPaused() : false), GameConsole::eDEBUG);    
    }
    else
    {
        CONSOLE_PRINT("Creating object " + obj->objectName() + " from thread " + QThread::currentThread()->objectName() + " rendering paused " + QString::number(Mainapp::getInstance() != nullptr ? Mainapp::getInstance()->renderingPaused() : false), GameConsole::eDEBUG);    
    }
}

void MemoryManagement::printCreationInfo(const QString & name)
{
    CONSOLE_PRINT("Creating object " + name + " from thread " + QThread::currentThread()->objectName() + " rendering paused " + QString::number(Mainapp::getInstance() != nullptr ? Mainapp::getInstance()->renderingPaused() : false), GameConsole::eDEBUG);    
}

quint32 MemoryManagement::getObjectCounter()
{
    return m_objectCounter;
}
