#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"
#include "3rd_party/oxygine-framework/oxygine/core/ref_counter.h"
#include <QMultiMap>
#include <qmutex.h>

namespace oxygine
{
    inline void hash_combine(std::size_t&) { }

    template <typename T, typename... Rest>
    inline void hash_combine(std::size_t& seed, const T& v, Rest... rest)
    {
        seed ^= qHash(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        hash_combine(seed, rest...);
    }

    class Material;
    typedef intrusive_ptr<Material> spMaterialX;

    class Material;
    typedef intrusive_ptr<Material> spMaterial;

    class MaterialCache
    {
    public:
        MaterialCache();

        template<class T>
        intrusive_ptr<T> cache(const T& other)
        {
            return dynamic_pointer_cast<T>(clone_(other));
        }

        void clear();
        void removeUnused();
        size_t getTotalMaterials() const { return m_materials.size(); }

        static MaterialCache& mc();
    protected:
        spMaterial clone_(const Material& other);
        void removeUnusedNoLock();

    protected:
        typedef QMultiMap<size_t, spMaterialX> materials;
        materials m_materials;

        QMutex m_lock;
        qint32 m_addCounter;

    private:
        static MaterialCache mcache;
    };
}
