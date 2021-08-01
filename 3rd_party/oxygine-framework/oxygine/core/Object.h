#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/ref_counter.h"
#include <qstring.h>
#include <qmutex.h>

namespace oxygine
{
    DECLARE_SMART(Object, spObject);
    class Object : public ref_counter, public IClosureOwner
    {        
    public:
        explicit Object(const Object& src);
        explicit Object() = default;
        virtual ~Object() = default;
    protected:
        QMutex m_Locked;
    };


    template <class dest, class src>
    dest safeCast(src ptr)
    {
        if (!ptr)
        {
            return 0;
        }
#ifdef OXYGINE_DEBUG_SAFECAST
        dest cast = dynamic_cast<dest>(ptr);
        if (cast == nullptr)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "safeCast can't cast pointer");
        }
        return cast;
#else
        return static_cast<dest>(ptr);
#endif
    }

    template<class T, class U>
    intrusive_ptr<T> safeSpCast(intrusive_ptr<U> const& p)
    {
        if (!p)
        {
            return 0;
        }
#ifdef OXYGINE_DEBUG_SAFECAST
        intrusive_ptr<T> t = dynamic_cast<T*>(p.get());
        if (t.get() == nullptr)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "safeSpCast can't cast pointer");
        }
#endif
        return static_cast<T*>(p.get());
    }
}
