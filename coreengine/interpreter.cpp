#include "coreengine/interpreter.h"
#include "coreengine/console.h"
#include "coreengine/mainapp.h"

#include <QDir>
#include <QTextStream>

Interpreter::Interpreter(const QString& script, QObject *parent)
    : QObject(parent)
{
    init();
    openScript(script);
}

Interpreter::Interpreter(QObject *parent)
    : QObject(parent)
{
    init();
}

void Interpreter::init()
{
    engine = new QJSEngine();
    QJSValue globals = engine->newQObject(Mainapp::getInstance());
    engine->globalObject().setProperty("globals", globals);
    engine->installTranslatorFunctions();
}

void Interpreter::openScript(const QString& script)
{
    QFile scriptFile(script);
    if (!scriptFile.open(QIODevice::ReadOnly))
    {
        QString error = "Error: attemp to read File " + script + " which couldn't be opened.";
        Console::print(error, Console::eERROR);
    }
    else
    {
        QTextStream stream(&scriptFile);
        QString contents = stream.readAll();
        scriptFile.close();
        QJSValue value = engine->evaluate(contents, script);
        if (value.isError())
        {
            QString error = value.toString() + " in File:" + script;
            Console::print(error, Console::eERROR);
        }
    }
}

Interpreter::~Interpreter()
{
    // free memory
   engine->collectGarbage();
   delete engine;
}


QJSValue Interpreter::doFunction(const QString& func, QJSValueList& args)
{
    QJSValue ret;
    QJSValue funcPointer = engine->globalObject().property(func);
    if (funcPointer.isCallable())
    {
        ret = funcPointer.call(args);
        if (ret.isError())
        {
            QString error = ret.toString();
            Console::print(error, Console::eERROR);
        }
    }
    else
    {
        QString error = "Error: attemp to call a non function value.";
        Console::print(error, Console::eERROR);
    }
    return ret;
}

QJSValue Interpreter::doFunction(const QString& obj, const QString& func, const QJSValueList& args)
{
    QJSValue ret;

    QJSValue objPointer = engine->globalObject().property(obj);
    if (objPointer.isObject())
    {
        QJSValue funcPointer = objPointer.property(func);
        if (funcPointer.isCallable())
        {
            ret = funcPointer.call(args);
            if (ret.isError())
            {
                QString error = ret.toString();
                Console::print(error, Console::eERROR);
            }
        }
        else
        {
            QString error = "Error: attemp to call a non function value.";
            Console::print(error, Console::eERROR);
        }
    }
    else
    {
        QString error = "Error: attemp to call a non object value in order to call a function.";
        Console::print(error, Console::eERROR);
    }
    return ret;
}

QJSValue Interpreter::doString(const QString& task)
{
    QJSValue value = engine->evaluate(task, "GameCode");

    if (value.isError())
    {
        Console::print(value.toString(), Console::eERROR);
    }
    else if (!value.isNull() && !value.isUndefined())
    {
        Console::print("Result = " + value.toString(), Console::eDEBUG);
    }
    return value;
}

void Interpreter::pushInt(const QString& name, qint32 value)
{
    engine->globalObject().setProperty(name, value);
}

void Interpreter::pushDouble(const QString& name, double value)
{
    engine->globalObject().setProperty(name, value);
}

void Interpreter::pushString(const QString& name, const QString& value)
{
    engine->globalObject().setProperty(name, value);
}

void Interpreter::pushObject(const QString& name, QObject* object)
{
    QJSValue newQObj = engine->newQObject(object);
    engine->globalObject().setProperty(name, newQObj);
}

QJSValue Interpreter::newQObject(QObject* object)
{
    QJSValue newQObj = engine->newQObject(object);
    return newQObj;
}

void Interpreter::cleanMemory()
{
    engine->collectGarbage();
}

qint32 Interpreter::getGlobalInt(const QString& var)
{
    qint32 ret = 0;
    QJSValue value = engine->globalObject().property(var);
    if (!value.isNumber())
    {
        QString error = "Error: attemp to read " + var + "which is not from type number.";
        Console::print(error, Console::eERROR);
    }
    else
    {
        ret = value.toInt();
    }
    return ret;
}

bool Interpreter::getGlobalBool(const QString& var)
{
    bool ret = 0;
    QJSValue value = engine->globalObject().property(var);
    if (!value.isBool())
    {
        QString error = "Error: attemp to read " + var + "which is not from type bool.";
        Console::print(error, Console::eERROR);
    }
    else
    {
        ret = value.toBool();
    }
    return ret;
}

double Interpreter::getGlobalDouble(const QString& var)
{
    double ret = 0;
    QJSValue value = engine->globalObject().property(var);
    if (!value.isNumber())
    {
        QString error = "Error: attemp to read " + var + "which is not from type number.";
        Console::print(error, Console::eERROR);
    }
    else
    {
        ret = value.toNumber();
    }
    return ret;
}

QJSValue Interpreter::createObject()
{
    return engine->newObject();
}

QString Interpreter::getGlobalString(const QString& var)
{
    QString ret = "";
    QJSValue value = engine->globalObject().property(var);
    if (!value.isString())
    {
        QString error = "Error: attemp to read " + var + "which is not from type QString.";
        Console::print(error, Console::eERROR);
    }
    else
    {
        ret = value.toString();
    }
    return ret;
}

QJSValue Interpreter::getGlobal(const QString& var)
{
    QJSValue value = engine->globalObject().property(var);
    return value;
}

void Interpreter::setGlobal(const QString& var, QJSValue obj)
{
    engine->globalObject().setProperty(var, obj);
}

void Interpreter::deleteObject(const QString& name)
{
    QString order = "delete " + name + ";";
    doString(order);
}
