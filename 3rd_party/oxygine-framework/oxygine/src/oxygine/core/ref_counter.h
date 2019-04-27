#pragma once
#include "../oxygine-include.h"
#include "intrusive_ptr.h"

#include <atomic>

namespace oxygine
{
    /** RefCounter **/
    class ref_counter
    {
    public:
        std::atomic<int> _ref_counter{0};

        ref_counter()
            : _ref_counter(0)
        {
        }
        virtual ~ref_counter()
        {
        }

        void addRef()
        {
            _ref_counter++;
        }

        void releaseRef()
        {

            _ref_counter--;
            OX_ASSERT(_ref_counter >= 0);
            if (_ref_counter == 0)
            {
                delete this;
            }
        }

    private:
        ref_counter(const ref_counter&) = delete ;
        const ref_counter& operator=(const ref_counter&) = delete ;
        ref_counter(const ref_counter&&) = delete ;
        const ref_counter&& operator=(const ref_counter&&) = delete ;
    };


    inline void intrusive_ptr_add_ref(ref_counter* p)
    {
        p->addRef();
    }

    inline void intrusive_ptr_release(ref_counter* p)
    {
        p->releaseRef();
    }


    class AutoRefHolder
    {
    public:
        AutoRefHolder(ref_counter* rc)
            : _rc(rc)
        {
            _rc->addRef();
        }

        ~AutoRefHolder()
        {
            _rc->releaseRef();
        }

        ref_counter* _rc;

    private:
        AutoRefHolder(const AutoRefHolder&);
        AutoRefHolder& operator=(const AutoRefHolder&);
    };
}
