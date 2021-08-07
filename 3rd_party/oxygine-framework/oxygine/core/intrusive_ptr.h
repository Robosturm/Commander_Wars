#pragma once
#ifndef INTRUSIVE_PTR_HEADER
#define INTRUSIVE_PTR_HEADER

#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/ref_counter.h"
#include "QtGlobal"

namespace oxygine
{
    template <class T>
    class intrusive_ptr
    {
        T* m_pPointer{nullptr};
    public:
        template<typename ...TArgs>
        static intrusive_ptr<T> create(TArgs... args)
        {
            ++oxygine::ref_counter::instanceCounter;
            intrusive_ptr<T> pRet;
            pRet.m_pPointer =  new T(args...);
#ifdef GAMEDEBUG
            oxygine::ref_counter::lock.lock();
            oxygine::ref_counter::objects.append(pRet.m_pPointer);
            oxygine::ref_counter::lock.unlock();
#endif
            pRet.m_pPointer->addRef();
            return pRet;
        }

        explicit intrusive_ptr()
            : m_pPointer(nullptr)
        {
        }

        intrusive_ptr(const intrusive_ptr& s)
            : m_pPointer(s.m_pPointer)
        {
            if (m_pPointer != nullptr)
            {
                m_pPointer->addRef();
            }
        }

        template<class U>
        intrusive_ptr(intrusive_ptr<U> const& rhs)
            : m_pPointer(rhs.get())
        {
            if (m_pPointer != nullptr)
            {
                m_pPointer->addRef();
            }
        }

        T* get() const
        {
            return m_pPointer;
        }

        T& operator*() const
        {
            return *m_pPointer;
        }

        T* operator->() const
        {
            return m_pPointer;
        }

        intrusive_ptr& operator = (const intrusive_ptr& s)
        {
            intrusive_ptr(s).swap(*this);
            return *this;
        }

        intrusive_ptr& operator = (T* ptr)
        {
            intrusive_ptr(ptr).swap(*this);
            return *this;
        }

        intrusive_ptr(T* p)
        {
            // free old pointer
            if (m_pPointer != nullptr)
            {
                m_pPointer->releaseRef();
            }
            // swap pointer
            m_pPointer = p;
            if (m_pPointer != nullptr)
            {
                // allocate new pointer
                if (m_pPointer->getRefCounter() == 0)
                {
                    ++oxygine::ref_counter::instanceCounter;
                }
                m_pPointer->addRef();
            }
        }

        bool operator!() const
        {
            return m_pPointer == nullptr;
        }

        void swap(intrusive_ptr& s)
        {
            T* p = s.m_pPointer;
            s.m_pPointer = m_pPointer;
            m_pPointer = p;
        }

        operator bool ()const
        {
            return m_pPointer != nullptr;
        }

        virtual ~intrusive_ptr()
        {
            if (m_pPointer != nullptr)
            {
                m_pPointer->releaseRef();
                m_pPointer = nullptr;
            }
        }
    };

    template<class T, class U> inline bool operator==(intrusive_ptr<T> const& a, intrusive_ptr<U> const& b)
    {
        return a.get() == b.get();
    }

    template<class T, class U> inline bool operator!=(intrusive_ptr<T> const& a, intrusive_ptr<U> const& b)
    {
        return a.get() != b.get();
    }

    template<class T> inline bool operator==(intrusive_ptr<T> const& a, T* b)
    {
        return a.get() == b;
    }

    template<class T> inline bool operator!=(intrusive_ptr<T> const& a, T* b)
    {
        return a.get() != b;
    }

    template<class T> inline bool operator==(T* a, intrusive_ptr<T> const& b)
    {
        return a == b.get();
    }

    template<class T> inline bool operator!=(T* a, intrusive_ptr<T> const& b)
    {
        return a != b.get();
    }

    template<class T> T* get_pointer(intrusive_ptr<T> const& p)
    {
        return p.get();
    }

    template<class T, class U> intrusive_ptr<T> static_pointer_cast(intrusive_ptr<U> const& p)
    {
        return static_cast<T*>(p.get());
    }

    template<class T, class U> intrusive_ptr<T> const_pointer_cast(intrusive_ptr<U> const& p)
    {
        return const_cast<T*>(p.get());
    }

    template<class T, class U> intrusive_ptr<T> dynamic_pointer_cast(intrusive_ptr<U> const& p)
    {
        return dynamic_cast<T*>(p.get());
    }
}

#endif
