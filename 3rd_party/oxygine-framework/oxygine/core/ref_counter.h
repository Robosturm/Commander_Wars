#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"

#include <atomic>
#include <qobject.h>
#include <qthread.h>

namespace oxygine
{
    /** RefCounter **/
    class ref_counter
    {
    public:
        ref_counter() = default;
        virtual ~ref_counter() = default;

        inline const std::atomic<qint32> & getRefCounter() const
        {
            return m_ref_counter;
        }
    private:
        template<typename T>
        friend class intrusive_ptr;
        inline void addRef()
        {
            ++m_ref_counter;
        }

        inline void releaseRef()
        {
            if (0 == --m_ref_counter)
            {
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
                    pObj->deleteLater();
                }
            }
        }
    private:
        std::atomic<qint32> m_ref_counter{0};
    private:
        ref_counter(const ref_counter&) = delete ;
        const ref_counter& operator=(const ref_counter&) = delete ;
        ref_counter(const ref_counter&&) = delete ;
        const ref_counter&& operator=(const ref_counter&&) = delete ;
    };


}
