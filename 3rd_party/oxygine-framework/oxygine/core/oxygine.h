#pragma once
#include <QString>
#include <atomic>

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
    void handleErrorPolicy(error_policy ep, const QString & message, quint64 module = 1);

    template <class dest, class src>
    dest safeCast(src ptr)
    {
        if (!ptr)
        {
            return nullptr;
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
    std::shared_ptr<T> safeSpCast(std::shared_ptr<U> const& p)
    {
        if (!p)
        {
            return std::shared_ptr<T>();
        }
#ifdef OXYGINE_DEBUG_SAFECAST
        std::shared_ptr<T> t = std::dynamic_pointer_cast(p);
        if (t.get() == nullptr)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "safeSpCast can't cast pointer");
        }
        return t;
#else
        return std::static_pointer_cast(p);
#endif
    }
}
