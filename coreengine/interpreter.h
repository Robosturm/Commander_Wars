#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QObject>
#include <QQmlEngine>

/**
 * @brief The Interpreter class java-script interpreter with easy access functions
 */
class Interpreter : public QQmlEngine
{
    Q_OBJECT

public:
    static Interpreter* getInstance()
    {
        return m_pInstance;
    }
    static Interpreter* createInstance()
    {
        if (m_pInstance == nullptr)
        {
            m_pInstance = new Interpreter();
        }
        return m_pInstance;
    }

    virtual ~Interpreter();

    static void setCppOwnerShip(QObject* object);
    /**
     * @brief getRuntimeData
     * @return
     */
    static QString getRuntimeData();
signals:

public slots:
    void openScript(QString script, bool setup);
    void loadScript(QString content, QString script);
    QJSValue doFunction(QString func, QJSValueList& args);
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


private:
    explicit Interpreter();
    void init();
    static QString m_runtimeData;
    static Interpreter* m_pInstance;
};

#endif // INTERPRETER_H
