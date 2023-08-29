#pragma once

#include <QJSValue>
#include <QObject>

class JsThis
{
public:
    explicit JsThis() = default;

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

