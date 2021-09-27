#include "3rd_party/oxygine-framework/oxygine/core/ref_counter.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

#include "gameinput/mapmover.h"
#include "network/rxtask.h"
#include "network/txtask.h"
#include "network/NetworkInterface.h"

#include <qobject.h>
#include <qthread.h>

namespace oxygine
{
    std::atomic<qint32> ref_counter::m_instanceCounter = 0;
    std::atomic<qint32> ref_counter::m_jsInstanceCounter = 0;
#ifdef MEMORYTRACING
    QMutex ref_counter::m_lock;
    QVector<ref_counter*> ref_counter::m_objects;
#endif
    void ref_counter::releaseRef()
    {
        if (0 == --m_ref_counter)
        {
            freeObject();
            QObject* pObj = dynamic_cast<QObject*>(this);
            if (pObj == nullptr ||
                pObj->thread() == QThread::currentThread())
            {
                delete this;
            }
            else
            {
                if (GameWindow::getWindow() != nullptr &&
                    !GameWindow::getWindow()->getShuttingDown())
                {
                    oxygine::Texture* pObj1 = dynamic_cast<oxygine::Texture*>(this);
                    MapMover* pObj2 = dynamic_cast<MapMover*>(this);
                    RxTask* pObj3 = dynamic_cast<RxTask*>(this);
                    TxTask* pObj4 = dynamic_cast<TxTask*>(this);                    
                    NetworkInterface* pObj5 = dynamic_cast<NetworkInterface*>(this);
                    SingleResAnim* pObj6 = dynamic_cast<SingleResAnim*>(this);
                    if (pObj1 == nullptr &&
                        pObj2 == nullptr &&
                        pObj3 == nullptr &&
                        pObj4 == nullptr &&
                        pObj5 == nullptr &&
                        pObj6 == nullptr)
                    {
                        handleErrorPolicy(oxygine::ep_show_error, "deleting object from different thread");
                    }
                }
                pObj->deleteLater();
            }
        }
        else if (m_ref_counter < 0)
        {
            handleErrorPolicy(oxygine::ep_show_error, "ref_counter counter is negativ");
        }
    }

    void ref_counter::freeObject()
    {
        --m_instanceCounter;
#ifdef MEMORYTRACING
        m_lock.lock();
        m_objects.removeOne(this);
        m_lock.unlock();
#endif
    }

    void ref_counter::trackObject(ref_counter *pObj)
    {
        ++m_instanceCounter;
#ifdef MEMORYTRACING
        m_lock.lock();
        m_objects.append(pObj);
        m_lock.unlock();
#endif
    }
}
