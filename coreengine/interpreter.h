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
            m_pInstance = new Interpreter();
        }
        return m_pInstance.get();
    }

    virtual ~Interpreter();

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
    static void reloadInterpreter(QString runtime);

signals:

public slots:
    void openScript(QString script, bool setup);
    void loadScript(QString content, QString script);
    QJSValue doFunction(QString func, const QJSValueList& args = QJSValueList());
    QJSValue doFunction(QString obj, QString func, const QJSValueList& args = QJSValueList());
    void cleanMemory();
    /**
     * @brief doString immediatly interprates the string with the javascript-interpreter
     * @param task string parsed to the interpreter
     */
    QJSValue doString(QString task);
    /**
     * @brief pushInt
     * @param value
     */
    void pushInt(QString name, qint32 value);
    void pushDouble(QString name, double value);
    void pushString(QString name, QString value);
    void pushObject(QString name, QObject* object);
    QJSValue newQObject(QObject* object);
    void deleteObject(QString name);

    qint32 getGlobalInt(QString var);
    bool getGlobalBool(QString var);
    double getGlobalDouble(QString var);
    QString getGlobalString(QString var);
    QJSValue getGlobal(QString var);
    void setGlobal(QString var, QJSValue obj);
    /**
     * @brief exists checks if the js object exists
     * @param object
     * @return
     */
    bool exists(QString object);
    /**
     * @brief exists checks if the js object and function exists
     * @param object
     * @return
     */
    bool exists(QString object, QString function);


private:
    explicit Interpreter();    
    /**
     * @brief init
     */
    void init();
    static QString m_runtimeData;
    static spInterpreter m_pInstance;
};

#endif // INTERPRETER_H
