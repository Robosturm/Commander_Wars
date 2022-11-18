#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"
#include "3rd_party/oxygine-framework/oxygine/core/ref_counter.h"
#include <QMultiMap>
#include <QMutex>

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
    using spMaterial = intrusive_ptr<Material>;

    class MaterialCache final
    {
    public:
        explicit MaterialCache() = default;
        ~MaterialCache() = default;
        spMaterial cache(const Material& other)
        {
            return clone_(other);
        }
        void clear();
        void removeUnused();
        qint32 getSize();
        static MaterialCache& mc();
    protected:
        spMaterial clone_(const Material& other);
        void removeUnusedNoLock();

    protected:
        using materials = QMultiMap<size_t, spMaterial>;
        materials m_materials;

        QMutex m_lock;
        qint32 m_addCounter{0};

    private:
        static MaterialCache mcache;
    };
}
