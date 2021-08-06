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

        static std::atomic<qint32> instanceCounter;
        static QMutex lock;
        static QVector<ref_counter*> objects;
    private:
        template<typename T>
        friend class intrusive_ptr;
        inline void addRef()
        {
            ++m_ref_counter;
        }
        void releaseRef();
    private:
        std::atomic<qint32> m_ref_counter{0};
    private:
        ref_counter(const ref_counter&) = delete ;
        const ref_counter& operator=(const ref_counter&) = delete ;
        ref_counter(const ref_counter&&) = delete ;
        const ref_counter&& operator=(const ref_counter&&) = delete ;
    };


}
