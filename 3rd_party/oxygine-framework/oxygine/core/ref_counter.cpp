#include "3rd_party/oxygine-framework/oxygine/core/ref_counter.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

#include "gameinput/mapmover.h"
#include "network/NetworkInterface.h"

#include <qobject.h>
#include <qthread.h>

namespace oxygine
{
    std::atomic<qint32> ref_counter::instanceCounter = 0;
#ifdef GAMEDEBUG
    QMutex ref_counter::lock;
    QVector<ref_counter*> ref_counter::objects;
#endif
    void ref_counter::releaseRef()
    {
        if (0 == --m_ref_counter)
        {

            --oxygine::ref_counter::instanceCounter;
#ifdef GAMEDEBUG
            lock.lock();
            objects.removeOne(this);
            lock.unlock();
#endif
            QObject* pObj = dynamic_cast<QObject*>(this);
            if (pObj == nullptr)
            {
                delete this;
            }
            else if (pObj->thread() == QThread::currentThread())
            {
                delete this;
            }
            else
            {
                oxygine::ResAnim* pObj1 = dynamic_cast<oxygine::ResAnim*>(this);
                MapMover* pObj2 = dynamic_cast<MapMover*>(this);
                NetworkInterface* pObj3 = dynamic_cast<NetworkInterface*>(this);
                if (GameWindow::getWindow() != nullptr &&
                    !GameWindow::getWindow()->getShuttingDown())
                {
                    if (pObj1 == nullptr &&
                        pObj2 == nullptr &&
                        pObj3 == nullptr)
                    {
                        handleErrorPolicy(oxygine::ep_show_error, "deleting object from different thread");
                    }
                }
                pObj->deleteLater();
            }
        }
    }
}
