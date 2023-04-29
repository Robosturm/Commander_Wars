#pragma once
#include "3rd_party/oxygine-framework/oxygine/core/ref_counter.h"

namespace oxygine
{
    template <class T>
    class intrusive_ptr final
    {
        T* m_pPointer{nullptr};
    public:
        template<typename ...TArgs>
        static intrusive_ptr<T> create(TArgs... args)
        {
            intrusive_ptr<T> pRet;
            pRet.m_pPointer = new T(args...);
            oxygine::ref_counter::trackObject(pRet.m_pPointer);
            pRet.m_pPointer->addRef();
            return pRet;
        }

        explicit intrusive_ptr()
            : m_pPointer(nullptr)
        {
        }

        ~intrusive_ptr()
        {
            if (m_pPointer != nullptr)
            {
                m_pPointer->releaseRef();
                m_pPointer = nullptr;
            }
        }

        intrusive_ptr(const intrusive_ptr& s)
        {
            // free old pointer
            if (m_pPointer != nullptr)
            {
                m_pPointer->releaseRef();
            }
            m_pPointer = s.get();
            if (m_pPointer != nullptr)
            {
                m_pPointer->addRef();
            }
        }

        template<class U>
        intrusive_ptr(intrusive_ptr<U> const& rhs)
        {
            // free old pointer
            if (m_pPointer != nullptr)
            {
                m_pPointer->releaseRef();
            }
            m_pPointer = rhs.get();
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

        explicit intrusive_ptr(T* p, bool inConstructor = false)
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
                if (!inConstructor)
                {
                    oxygine::ref_counter::trackObject(m_pPointer);
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

    template<class T, class U> intrusive_ptr<T> static_pointer_cast(intrusive_ptr<U> const& p)
    {
        return intrusive_ptr<T>(static_cast<T*>(p.get()));
    }

    template<class T, class U> intrusive_ptr<T> const_pointer_cast(intrusive_ptr<U> const& p)
    {
        return intrusive_ptr<T>(const_cast<T*>(p.get()));
    }

    template<class T, class U> intrusive_ptr<T> dynamic_pointer_cast(intrusive_ptr<U> const& p)
    {
        return intrusive_ptr<T>(dynamic_cast<T*>(p.get()));
    }
}
