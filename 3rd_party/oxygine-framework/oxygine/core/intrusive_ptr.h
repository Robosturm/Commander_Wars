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
        T* _ptr{nullptr};
    public:
        typedef T element_type;

        intrusive_ptr()
            : _ptr(nullptr)
        {
        }
        intrusive_ptr(const intrusive_ptr& s)
            : _ptr(s._ptr)
        {
            if (_ptr != nullptr)
            {
                s._ptr->addRef();
            }
        }

        template<class U>
        intrusive_ptr(intrusive_ptr<U> const& rhs)
            : _ptr(rhs.get())
        {
            if (_ptr != nullptr)
            {
                _ptr->addRef();
            }
        }

        T* get() const
        {
            return _ptr;
        }

        T& operator*() const
        {
            Q_ASSERT(_ptr && _ptr->_ref_counter > 0);
            return *_ptr;
        }

        T* operator->() const
        {
            Q_ASSERT(_ptr && _ptr->_ref_counter > 0);
            return _ptr;
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
            : _ptr(p)
        {
            if (p)
            {
                p->addRef();
            }
        }

        bool operator!() const
        {
            return _ptr == nullptr;
        }

        void swap(intrusive_ptr& s)
        {
            T* p = s._ptr;
            s._ptr = _ptr;
            _ptr = p;
        }
        operator bool ()const
        {
            return _ptr != nullptr;
        }

        ~intrusive_ptr()
        {
            if (_ptr != nullptr)
            {
                _ptr->releaseRef();
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
