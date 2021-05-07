#ifndef Closure_HEADER_
#define Closure_HEADER_

#include <functional>
#include <memory>

namespace oxygine
{
    /**
     * @brief Empty interface to avoid void pointers all over the place
     */
    class IClosureOwner
    {
    };
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
            m_callback = std::function<TRet(TArgs...)>(lambda);
        }

        template<class TLambda>
        Closure(TLambda lambda)
        {
            m_callback = std::function<TRet(TArgs...)>(lambda);
        }
        TRet operator()(TArgs... args)
        {
            if constexpr(std::is_void<TRet>::value)
            {
                (m_callback)(args...);
            }
            else
            {
                return (m_callback)(args...);
            }
        }

    private:
        std::function<TRet(TArgs...)> m_callback;
    };

    template<typename TRet, typename ...TArgs>
    class OwnedClosure : public Closure<TRet, TArgs...>
    {
    public:
        explicit OwnedClosure()
        {
        }

        template<class TClass>
        explicit OwnedClosure(TClass* pOwner, TRet(TClass::*callback)(TArgs...))
            : Closure<TRet, TArgs...>(pOwner, callback),
              m_pOwner(pOwner)
        {
            m_isSet = true;
        }

        template<class TLambda>
        OwnedClosure(TLambda lambda)
            : Closure<TRet, TArgs...>(lambda)
        {
            m_isSet = true;
        }
        bool isSet()
        {
            return m_isSet;
        }
        bool isOwner(IClosureOwner* pOwner) const
        {
            return (m_pOwner == pOwner);
        }

    private:
        IClosureOwner* m_pOwner{nullptr};
        bool m_isSet{false};
    };

}

#endif

