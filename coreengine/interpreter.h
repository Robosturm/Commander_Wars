#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QObject>
#include <QQmlEngine>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

class Interpreter;
typedef oxygine::intrusive_ptr<Interpreter> spInterpreter;
/**
 * @brief The Interpreter class java-script interpreter with easy access functions
 */
class Interpreter : public QQmlEngine, public oxygine::ref_counter
{
    Q_OBJECT

public:
    static Interpreter* getInstance()
    {
        return m_pInstance.get();
    }
    static Interpreter* createInstance()
    {
        if (m_pInstance.get() == nullptr)
        {
            m_pInstance = spInterpreter::create();
            m_pInstance->init();
        }
        else
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "illegal interpreter creation");
        }
        return m_pInstance.get();
    }

    virtual ~Interpreter();
    static void release();

    static void setCppOwnerShip(QObject* object);
    /**
     * @brief getRuntimeData
     * @return
     */
    static QString getRuntimeData();
    /**
     * @brief reloadInterpreter
     * @param runtime
     */
    static void reloadInterpreter(const QString & runtime);

signals:
    void sigNetworkGameFinished(qint32 value, QString id);
public slots:
    void openScript(const QString & script, bool setup);
    void loadScript(const QString & content, const QString & script);
    QJSValue doFunction(const QString & func, const QJSValueList& args = QJSValueList());
    QJSValue doFunction(const QString & obj, const QString & func, const QJSValueList& args = QJSValueList());
    void cleanMemory();
    /**
     * @brief doString immediatly interprates the string with the javascript-interpreter
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
    QJSValue newQObject(QObject* object);
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
    bool exists(const QString & object);
    /**
     * @brief exists checks if the js object and function exists
     * @param object
     * @return
     */
    bool exists(const QString & object, const QString & function);
private slots:
    void networkGameFinished(qint32 value, QString id);
private:
    friend class oxygine::intrusive_ptr<Interpreter>;
    explicit Interpreter();    
    /**
     * @brief init
     */
    void init();

private:
    static QString m_runtimeData;
    static spInterpreter m_pInstance;
};

#endif // INTERPRETER_H
