#include "3rd_party/oxygine-framework/oxygine/core/Restorable.h"
#include <QMutexLocker>

namespace oxygine
{
    static bool m_restoring = false;
    QMutex Restorable::m_mutex;
    Restorable::restorable Restorable::m_restorable;

    Restorable::restorable::iterator Restorable::findRestorable(Restorable* r)
    {
        Restorable::restorable::iterator i = std::find(m_restorable.begin(), m_restorable.end(), r);
        return i;
    }

    const Restorable::restorable& Restorable::getObjects()
    {
        return m_restorable;
    }

    void Restorable::restoreAll()
    {
        restorable rs;

        {
            QMutexLocker al(&m_mutex);
            rs.swap(m_restorable);
        }

        for (restorable::iterator i = rs.begin(); i != rs.end(); ++i)
        {
            Restorable* r = *i;
            r->restore();
        }
    }

    bool Restorable::isRestored()
    {
        return m_restorable.empty();
    }

    void Restorable::releaseAll()
    {
        restorable rs;
        {
            QMutexLocker al(&m_mutex);
            rs.swap(m_restorable);
        }

        for (restorable::iterator i = rs.begin(); i != rs.end(); ++i)
        {
            Restorable* r = *i;
            r->release();
        }

        {
            QMutexLocker al(&m_mutex);
            rs.swap(m_restorable);
        }
    }

    Restorable::Restorable(): m_registered(false)
    {

    }

    Restorable::~Restorable()
    {
        unreg();
    }

    void Restorable::reg(RestoreCallback cb)
    {
        if (m_registered)
        {
            return;
        }
        QMutexLocker al(&m_mutex);

        Q_ASSERT(m_restoring == false);
        m_cb = cb;

        m_registered = true;

        restorable::iterator i = findRestorable(this);
        Q_ASSERT(i == m_restorable.end());
        m_restorable.push_back(this);
    }

    void Restorable::unreg()
    {
        if (!m_registered)
        {
            return;
        }
        QMutexLocker al(&m_mutex);
        Q_ASSERT(m_restoring == false);
        restorable::iterator i = findRestorable(this);
        if (i != m_restorable.end())
        {
            m_restorable.erase(i);
        }
        m_registered = false;
    }

    void Restorable::restore()
    {
        if (m_cb.isSet())
        {
            m_cb(this);
        }
    }
}
