#ifndef Closure_HEADER_
#define Closure_HEADER_

#include <functional>
#include <memory>

namespace oxygine
{
    /**
     * class for closuring Class Functions and lambdas and std::function into a unique interface.
     * And providing a way of checking if the closures are equal.
     */
    template<typename TRet, typename ...TArgs>
    class Closure
    {
    public:
        explicit Closure()
        {
        }

        template<class TClass>
        explicit Closure(TClass* pOwner, TRet(TClass::*callback)(TArgs...))
            : m_pThis(pOwner)
        {
            auto lambda = [pOwner, callback](TArgs... args)
            {
                if constexpr(std::is_void<TRet>::value)
                {
                    (pOwner->*callback)(args...);
                }
                else
                {
                    return (pOwner->*callback)(args...);
                }
            };
            m_callback = std::make_shared<std::function<TRet(TArgs...)>>(lambda);
        }
        explicit Closure(void* owner, const std::function<TRet(TArgs...)> & callback)
            : m_pThis(owner),
              m_callback(std::make_shared<std::function<TRet(TArgs...)>>(callback))
        {
        }
        template<class TLambda>
        Closure(TLambda lambda)
        {
            m_callback = std::make_shared<std::function<TRet(TArgs...)>>(lambda);
            m_pThis = m_callback.get();
        }
        TRet operator()(TArgs... args)
        {
            if constexpr(std::is_void<TRet>::value)
            {
                (*m_callback)(args...);
            }
            else
            {
                return (*m_callback)(args...);
            }

        }
        bool operator == (const Closure &c) const
        {
            return (this->m_pThis == c.m_pThis) &&
                   (this->m_callback == c.m_callback);
        }
        bool isSet()
        {
            return (m_pThis != nullptr);
        }
        bool isOwner(void* pOwner) const
        {
            return (m_pThis == pOwner);
        }
    private:
        void* m_pThis{nullptr};
        std::shared_ptr<std::function<TRet(TArgs...)>> m_callback;
    };

}

#endif

