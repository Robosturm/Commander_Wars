#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/ref_counter.h"
#include <qstring.h>
#include <qmutex.h>

namespace oxygine
{
    DECLARE_SMART(Object, spObject);
    class Object : public ref_counter
    {        
    public:
        Object(const Object& src);
        Object();
        virtual ~Object() = default;

        QString  getName() const;
        bool                isName(QString name) const;
        void setName(QString name);

    protected:
        QString __name;
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
        Q_ASSERT(cast && "can't cast pointer");
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
        Q_ASSERT(t && "can't cast pointer");
#endif
        return static_cast<T*>(p.get());
    }
}
