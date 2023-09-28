#pragma once
#include <QMultiMap>
#include <QMutex>

class MemoryManagement;

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
using spMaterial = std::shared_ptr<Material>;

class MaterialCache;
using spMaterialCache = std::shared_ptr<MaterialCache>;

class MaterialCache final
{
public:
    static constexpr const char* const getTypeName()
    {
        return "ClipRectActor";
    }
    ~MaterialCache() = default;
    spMaterial cache(const Material& other)
    {
        return clone_(other);
    }
    void clear();
    void removeUnused();
    qint32 getSize();
    static MaterialCache& mc();
    static void release()
    {
        mcache.reset();
    }
protected:
    spMaterial clone_(const Material& other);
    void removeUnusedNoLock();

protected:
    using materials = QMultiMap<size_t, spMaterial>;
    materials m_materials;

    QMutex m_lock;
    qint32 m_addCounter{0};
private:
    friend MemoryManagement;
    explicit MaterialCache() = default;
    static spMaterialCache mcache;
};
}
