#pragma once

#include <QJSValue>
#include <QObject>

class JsThis
{
public:
    explicit JsThis() = default;
    ~JsThis();

    static constexpr QJSValue & getJsThis(JsThis * me)
    {
        if (me == nullptr)
        {
            return m_null;
        }
        else
        {
            return me->getJsThis();
        }
    }
    static void setupJsThis(QObject* object, JsThis* me)
    {
        me->setupJsThis(object);
    }
protected:
    void setupJsThis(QObject * me);
private:
    inline QJSValue & getJsThis()
    {
        return m_jsThis;
    }
protected:
    QJSValue m_jsThis;
    static QJSValue m_null;
};

