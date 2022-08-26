#pragma once
#include <QString>
#include <atomic>
#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"

/**main oxygine namespace*/
namespace oxygine
{
    namespace core
    {
        /**destroy and reset any GPU allocated memory and handles. Call it to free memory if app was minimized (lost focus)*/
        void reset();
    }
    enum error_policy
    {
        ep_show_error,//shows assert and prints error to log
        ep_show_warning,//prints warning to log
        ep_ignore_error//doesn't show any errors
    };
    void handleErrorPolicy(error_policy ep, const QString & message);

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
            return intrusive_ptr<T>();
        }
#ifdef OXYGINE_DEBUG_SAFECAST
        intrusive_ptr<T> t = intrusive_ptr<T>(dynamic_cast<T*>(p.get()));
        if (t.get() == nullptr)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "safeSpCast can't cast pointer");
        }
#endif
        return intrusive_ptr<T>(static_cast<T*>(p.get()));
    }
}
