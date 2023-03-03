#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"
#include "3rd_party/oxygine-framework/oxygine/math/ScalarMath.h"

namespace oxygine
{
    template<typename TValue, typename getValueRef, typename setValueRef, typename TClass, getValueRef(TClass::*GetFunction)() const, void (TClass::*SetFunction)(setValueRef)>
    class Property0
    {
    public:
        using TActor = TClass;
        Property0(getValueRef dest)
            : m_dest(dest),
              m_src(dest)
        {
        }

        void init(TClass& t)
        {
            m_src = (t.*GetFunction)();
            m_initialized = true;
        }

        void init(getValueRef src)
        {
            m_src = src;
            m_initialized = true;
        }

        void setSrc(TClass& t)
        {
            (t.*SetFunction)(m_src);
        }

        void setDest(TClass& t)
        {
            (t.*SetFunction)(m_dest);
        }

        const TValue& getDest() const
        {
            return m_dest;
        }

        const TValue& getSrc() const
        {
            return m_src;
        }

        void update(TClass& t, float p, const oxygine::UpdateState& us)
        {
            if (!m_initialized)
            {
                oxygine::handleErrorPolicy(oxygine::ep_show_error, "Property0::update not initialized");
                return;
            }
            TValue value = lerp(m_src, m_dest, p);
            (t.*SetFunction)(value);
        }

        void done(TClass&)
        {
        }

    private:
        TValue m_dest;
        TValue m_src;
        bool m_initialized{false};
    };

    template<typename Value, typename valueRef, typename TClass, valueRef(TClass::*GetFunction)() const, void (TClass::*SetFunction)(valueRef)>
    class Property: public Property0<Value, valueRef, valueRef, TClass, GetFunction, SetFunction>
    {
    public:
        Property(valueRef v)
            : Property0<Value, valueRef, valueRef, TClass, GetFunction, SetFunction>(v)
        {
        }
    };

    template<typename value0, typename value, typename valueRef, typename TClass, valueRef(TClass::*GetFunction)() const, void (TClass::*SetFunction)(valueRef)>
    class Property2Args : public Property < value, valueRef, TClass, GetFunction, SetFunction >
    {
        using super = Property<value, valueRef, TClass, GetFunction, SetFunction>;
    public:
        Property2Args(value0 v1, value0 v2)
            : super(value(v1, v2))
        {
        }
        Property2Args(valueRef v)
            : super(v)
        {
        }
    };

    template<typename value0, typename value, typename getValueRef, typename setValueRef, typename TClass, getValueRef(TClass::*GetFunction)() const, void (TClass::*SetFunction)(setValueRef)>
    class Property2Args2 : public Property0 < value, getValueRef, setValueRef, TClass, GetFunction, SetFunction >
    {
        using super = Property0<value, getValueRef, setValueRef, TClass, GetFunction, SetFunction>;
    public:
        Property2Args2(value0 v1, value0 v2)
            : super(value(v1, v2))
        {
        }
        Property2Args2(getValueRef v)
            : super(v)
        {
        }
    };

    template<typename value0, typename value, typename valueRef, typename TClass, valueRef(TClass::*GetFunction)() const, void (TClass::*SetFunction)(valueRef)>
    class Property2Args1Arg : public Property < value, valueRef, TClass, GetFunction, SetFunction >
    {
        using super = Property<value, valueRef, TClass, GetFunction, SetFunction>;
    public:
        Property2Args1Arg(value0 v)
            : super(value(v, v))
        {
        }
        Property2Args1Arg(value0 v1, value0 v2)
            : super(value(v1, v2))
        {
        }
        Property2Args1Arg(valueRef v)
            : super(v)
        {
        }
    };
}
