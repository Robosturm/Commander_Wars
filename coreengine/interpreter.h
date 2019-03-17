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
    explicit Interpreter(const QString& script, QObject *parent = 0);
    explicit Interpreter(QObject *parent = 0);

    virtual ~Interpreter();
    void openScript(const QString& script);

    QJSValue doFunction(const QString& func, QJSValueList& args);
    QJSValue doFunction(const QString& obj, const QString& func, const QJSValueList& args = QJSValueList());
    void cleanMemory();
    /**
     * @brief doString immediatly interprates the string with the javascript-interpreter
     * @param task string parsed to the interpreter
     */
    QJSValue doString(const QString& task);
    /**
     * @brief pushInt
     * @param value
     */
    void pushInt(const QString& name, qint32 value);
    void pushDouble(const QString& name, double value);
    void pushString(const QString& name, const QString& value);
    void pushObject(const QString& name, QObject* object);
    QJSValue newQObject(QObject* object);
    void deleteObject(const QString& name);

    qint32 getGlobalInt(const QString& var);
    bool getGlobalBool(const QString& var);
    double getGlobalDouble(const QString& var);
    QString getGlobalString(const QString& var);
    QJSValue getGlobal(const QString& var);
    void setGlobal(const QString& var, QJSValue obj);
    static void setCppOwnerShip(QObject* object);
private:
    void init();
signals:

public slots:
};

#endif // INTERPRETER_H
