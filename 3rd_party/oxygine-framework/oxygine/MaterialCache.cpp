#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"
#include "3rd_party/oxygine-framework/oxygine/Material.h"

#include <QMutexLocker>

namespace oxygine
{
    QScopedPointer<MaterialCache> MaterialCache::mcache;

    spMaterial MaterialCache::clone_(const Material& other)
    {
        QMutexLocker alock(&m_lock);
        size_t hash;
        other.update(hash);
        auto items = m_materials.values(hash);
        for (const auto & sec : items)
        {
            if (sec->compare(sec.get(), &other))
            {
                return spMaterial(sec);
            }
        }
        ++m_addCounter;
        if (m_addCounter > 30)
        {
            removeUnusedNoLock();
        }
        spMaterial copy = other.clone();
        copy->m_hash = hash;
        m_materials.insert(hash, copy);
        return copy;
    }

    void MaterialCache::removeUnusedNoLock()
    {
        m_addCounter = 0;
        materials fresh;
        for (const auto & material : qAsConst(m_materials))
        {
            if (material->getRefCounter() > 1)
            {
                fresh.insert(material->m_hash, material);
            }
        }
        std::swap(fresh, m_materials);
    }

    qint32 MaterialCache::getSize()
    {
        QMutexLocker alock(&m_lock);
        return m_materials.size();
    }

    void MaterialCache::removeUnused()
    {
        QMutexLocker alock(&m_lock);
        removeUnusedNoLock();
    }

    void MaterialCache::clear()
    {
        QMutexLocker alock(&m_lock);
        m_addCounter = 0;
        m_materials.clear();
    }

    MaterialCache& MaterialCache::mc()
    {
        if (mcache.isNull())
        {
            mcache.reset(new MaterialCache());
        }
        return *mcache.get();
    }
}
