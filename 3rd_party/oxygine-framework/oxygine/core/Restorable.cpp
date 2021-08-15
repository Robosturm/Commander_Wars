#include "3rd_party/oxygine-framework/oxygine/core/Restorable.h"
#include <QMutexLocker>

namespace oxygine
{
    static bool m_restoring = false;
    QMutex Restorable::m_mutex;
    Restorable::restorable Restorable::m_restorable;

    Restorable::restorable::const_iterator Restorable::findRestorable(Restorable* r)
    {
        auto i = std::find(m_restorable.cbegin(), m_restorable.cend(), r);
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

        if (m_restoring != false)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Restorable::reg error while restoring");
        }
        m_cb = cb;

        m_registered = true;

        restorable::const_iterator i = findRestorable(this);
        if (i != m_restorable.end())
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Restorable::reg iterator error");
        }
        m_restorable.push_back(this);
    }

    void Restorable::unreg()
    {
        if (!m_registered)
        {
            return;
        }
        QMutexLocker al(&m_mutex);
        if (m_restoring != false)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Restorable::unreg error while unregistering");
        }
        restorable::const_iterator i = findRestorable(this);
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
