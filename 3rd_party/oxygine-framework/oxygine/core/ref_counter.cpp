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
    std::vector<ref_counter*> ref_counter::m_objects;
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
        auto cIter = m_objects.cbegin();
        while (cIter != m_objects.cend())
        {
            if (*cIter == this)
            {
                m_objects.erase(cIter);
                break;
            }
            else
            {
                ++cIter;
            }
        }
        m_lock.unlock();
#endif
    }

    void ref_counter::trackObject(ref_counter *pObj)
    {
        ++m_instanceCounter;
#ifdef MEMORYTRACING
        m_lock.lock();
        m_objects.push_back(pObj);
        m_lock.unlock();
#endif
    }
}
