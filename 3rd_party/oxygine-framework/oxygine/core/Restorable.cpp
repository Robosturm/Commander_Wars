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

    Restorable::Restorable(): _registered(false)
    {

    }

    Restorable::~Restorable()
    {
        unreg();
    }

    void Restorable::reg(RestoreCallback cb, void* user)
    {
        if (_registered)
            return;

        QMutexLocker al(&m_mutex);

        Q_ASSERT(m_restoring == false);
        _cb = cb;
        _userData = user;

        _registered = true;

        restorable::iterator i = findRestorable(this);
        Q_ASSERT(i == m_restorable.end());
        m_restorable.push_back(this);
    }

    void Restorable::unreg()
    {
        if (!_registered)
        {
            return;
        }
        QMutexLocker al(&m_mutex);
        Q_ASSERT(m_restoring == false);
        restorable::iterator i = findRestorable(this);
        //Q_ASSERT(i != _restorable.end());
        if (i != m_restorable.end())
        {
            m_restorable.erase(i);
        }
        _registered = false;
    }

    void Restorable::restore()
    {
        if (_cb.isSet())
        {
            _cb(this, _userData);
        }
    }
}
