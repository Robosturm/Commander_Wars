#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QObject>
#include <QQmlEngine>
#include <QVector>
#include <QThread>
#include <list>
#include <QCoreApplication>
#include "coreengine/jsthis.h"

using spQObject = std::shared_ptr<QObject>;

class Interpreter;
using spInterpreter = std::shared_ptr<Interpreter>;
/**
 * @brief The Interpreter class java-script interpreter with easy access functions
 */
class Interpreter final : public QJSEngine
{
    Q_OBJECT
    struct JsThisData
    {
        QObject* pObject{nullptr};
        JsThis* pThis{nullptr};
    };
public:
    static Interpreter* getInstance()
    {
        Q_ASSERT(m_pOwner == QThread::currentThread());
        return m_pInstance.get();
    }
    static Interpreter* createInstance();
    virtual ~Interpreter();
    static void release();

    static void setCppOwnerShip(QObject* object);
    /**
     * @brief getRuntimeData
     * @return
     */
    QString getRuntimeData();
    /**
     * @brief reloadInterpreter
     * @param runtime
     */
    static bool reloadInterpreter(QString runtime);

    bool getInJsCall() const;
    void trackJsObject(spQObject pObj);

    template<typename _TType, template<typename T> class _TVectorList>
    QJSValue arraytoJSValue(const _TVectorList<_TType> & array)
    {
        QJSValue jsArray = newArray(array.size());
        for (qint32 i = 0; i < array.size(); i++)
        {
            if constexpr (std::is_same<_TType, QPoint>::value)
            {
                jsArray.setProperty(i, toScriptValue(array[i]));
            }
            else
            {
                jsArray.setProperty(i, array[i]);
            }
        }
        return jsArray;
    }

    void threadProcessEvents()
    {
        Q_ASSERT(m_inJsCall == 0);
        QCoreApplication::processEvents(QEventLoop::ProcessEventsFlag::AllEvents, 5);
    }

signals:
    void sigNetworkGameFinished(qint32 value, QString id);
public slots:
    bool openScript(const QString & script, bool setup);
    bool loadScript(const QString & content, const QString & script);
    inline QJSValue doFunction(const QString & func, const QJSValueList& args = QJSValueList())
    {
        clearJsStack();
        QJSValue funcPointer = globalObject().property(func);
        ++m_inJsCall;
        QJSValue ret = funcPointer.call(args);
        exitJsCall();
        if (ret.isError())
        {
            QString error = ret.toString() + " in File: " +
                            ret.property("fileName").toString() + " at Line: " +
                            ret.property("lineNumber").toString();
            printError(error);
        }
        return ret;
    }
    inline QJSValue doFunction(const QString & obj, const QString & func, const QJSValueList& args = QJSValueList())
    {
        QJSValue ret;
        clearJsStack();
        if (exists(obj, func))
        {
            QJSValue objPointer = globalObject().property(obj);
            QJSValue funcPointer = objPointer.property(func);
            ++m_inJsCall;
            ret = funcPointer.call(args);
            exitJsCall();
            if (ret.isError())
            {
                QString error = ret.toString() + " in File: " +
                                ret.property("fileName").toString() + " at Line: " +
                                ret.property("lineNumber").toString();
                printError(error);
            }
        }
        return ret;
    }
    /**
     * @brief doString immediately interprates the string with the javascript-interpreter
     * @param task string parsed to the interpreter
     */
    QJSValue doString(const QString & task);
    /**
     * @brief pushInt
     * @param value
     */
    void pushInt(const QString & name, qint32 value);
    void pushDouble(const QString & name, double value);
    void pushString(const QString & name, const QString & value);
    void pushObject(const QString & name, QObject* object);
    inline QJSValue newQObject(QObject* object)
    {
        return QJSEngine::newQObject(object);
    }
    void deleteObject(const QString & name);

    qint32 getGlobalInt(const QString & var);
    bool getGlobalBool(const QString & var);
    double getGlobalDouble(const QString & var);
    QString getGlobalString(const QString & var);
    QJSValue getGlobal(const QString & var);
    void setGlobal(const QString & var, const QJSValue & obj);
    /**
     * @brief exists checks if the js object exists
     * @param object
     * @return
     */
    bool exists(const QString & object)
    {
        QJSValue objPointer = globalObject().property(object);
        if (objPointer.isObject())
        {
            return true;
        }
        return false;
    }
    /**
     * @brief exists checks if the js object and function exists
     * @param object
     * @return
     */
    bool exists(const QString & object, const QString & function)
    {
        QJSValue objPointer = globalObject().property(object);
        if (objPointer.isObject())
        {
            QJSValue funcPointer = objPointer.property(function);
            if (funcPointer.isCallable())
            {
                return true;
            }
        }
        return false;
    }
    QJSValue newQJsObject(QObject* object, JsThis* me)
    {
        m_ownedObjects.push_back({object, me});
        return newQObject(object);
    }
    void deleteQJsObject(JsThis* me)
    {
        for (auto iter = m_ownedObjects.begin(); iter != m_ownedObjects.end(); ++iter)
        {
            if (iter->pThis == me)
            {
                m_ownedObjects.erase(iter);
                break;
            }
        }
    }

private slots:
    void networkGameFinished(qint32 value, QString id);
private:
    friend class MemoryManagement;
    explicit Interpreter();    
    /**
     * @brief init
     */
    void init();
    void exitJsCall()
    {
        --m_inJsCall;
        Q_ASSERT(m_inJsCall >= 0);
    }
    void clearJsStack()
    {
        if (m_inJsCall == 0)
        {
            m_jsObjects.clear();
        }
    }
    void printError(const QString & msg);
private:
    static spInterpreter m_pInstance;
    static QString m_runtimeData;
    qint32 m_inJsCall{0};
    std::vector<spQObject> m_jsObjects;
    std::list<JsThisData> m_ownedObjects;
    qint32 m_jsCallCount{0};
    static QThread* m_pOwner;
};

#endif // INTERPRETER_H
