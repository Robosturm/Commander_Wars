#pragma once
#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"
#include <atomic>
#include <qmutex.h>
#include <QVector>

namespace oxygine
{
    class ResAnim;
    /** RefCounter **/
    class ref_counter
    {
    public:
        explicit ref_counter() = default;
        virtual ~ref_counter() = default;

        inline const std::atomic<qint32> & getRefCounter() const
        {
            return m_ref_counter;
        }
        static void trackObject(ref_counter* pObj);
        static qint32 getAlloctedObjectCount()
        {
            return m_instanceCounter;
        }
        static qint32 getAlloctedJsObjectCount()
        {
            return m_jsInstanceCounter;
        }
        static void addInstanceCounter()
        {
            ++m_jsInstanceCounter;
        }
        static void releaseInstanceCounter()
        {
            --m_jsInstanceCounter;
        }
    private:
        template<typename T>
        friend class intrusive_ptr;
        inline void addRef()
        {
            ++m_ref_counter;
        }
        void releaseRef();
        void freeObject();
        ref_counter(const ref_counter&) = delete ;
        const ref_counter& operator=(const ref_counter&) = delete;
        ref_counter(const ref_counter&&) = delete ;
        const ref_counter&& operator=(const ref_counter&&) = delete;
    private:
        std::atomic<qint32> m_ref_counter{0};

        static std::atomic<qint32> m_instanceCounter;
        static std::atomic<qint32> m_jsInstanceCounter;
#ifdef MEMORYTRACING
        static QMutex m_lock;
        static QVector<ref_counter*> m_objects;
#endif
    };


}
