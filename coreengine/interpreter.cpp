#include "coreengine/interpreter.h"
#include "coreengine/console.h"
#include "coreengine/mainapp.h"
#include "coreengine/globalutils.h"
#include "coreengine/audiothread.h"
#include "coreengine/userdata.h"
#include "resource_management/fontmanager.h"
#include "resource_management/cospritemanager.h"

#include <QDir>
#include <QQmlEngine>
#include <QTextStream>
#include <QThread>

spInterpreter Interpreter::m_pInstance = nullptr;
QString Interpreter::m_runtimeData;

Interpreter::Interpreter()
    : QQmlEngine()
{
    setObjectName("Interpreter");
    setCppOwnerShip(this);
    m_pInstance = this;
    init();
}

void Interpreter::reloadInterpreter(QString runtime)
{
    m_pInstance = nullptr;
    m_pInstance = spInterpreter::create();
    m_pInstance->loadScript(runtime, "Interpreter Runtime");
}

void Interpreter::init()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    setOutputWarningsToStandardError(false);
    setIncubationController(nullptr);

    QJSValue globals = newQObject(GlobalUtils::getInstance());
    globalObject().setProperty("globals", globals);
    QJSValue audio = newQObject(pApp->getAudioThread());
    globalObject().setProperty("audio", audio);
    QJSValue console = newQObject(Console::getInstance());
    globalObject().setProperty("GameConsole", console);
    globalObject().setProperty("console", console);
    QJSValue fontManager = newQObject(FontManager::getInstance());
    globalObject().setProperty("FontManager", fontManager);
    QJSValue settings = newQObject(Settings::getInstance());
    globalObject().setProperty("settings", settings);
    QJSValue userdata = newQObject(Userdata::getInstance());
    globalObject().setProperty("userdata", userdata);
    QJSValue coSpriteManager = newQObject(COSpriteManager::getInstance());
    globalObject().setProperty("coSpriteManager", coSpriteManager);

    GameEnums::registerEnums();

    installExtensions(QJSEngine::Extension::AllExtensions);
}

QString Interpreter::getRuntimeData()
{
    return m_runtimeData;
}

void Interpreter::openScript(QString script, bool setup)
{
    QFile scriptFile(script);
    if (!scriptFile.open(QIODevice::ReadOnly))
    {
        QString error = "Error: attemp to read file " + script + " which could not be opened.";
        Console::print(error, Console::eERROR);
    }
    else if (!scriptFile.exists())
    {
        QString error = "Error: unable to open non existing file " + script + ".";
        Console::print(error, Console::eERROR);
    }
    else
    {
        QTextStream stream(&scriptFile);
        QString contents = stream.readAll();
        if (setup)
        {
            stream.seek(0);
            while (!stream.atEnd())
            {
                QString line = stream.readLine().simplified();
                m_runtimeData += line + "\n";
            }
        }
        scriptFile.close();

        QJSValue value = evaluate(contents, script);
        if (value.isError())
        {
            QString error = value.toString() + " in File:" + script + " in File: " +
                            value.property("fileName").toString() + " at Line: " +
                            value.property("lineNumber").toString();
            Console::print(error, Console::eERROR);
        }
    }
}

void Interpreter::loadScript(QString content, QString script)
{
    QJSValue value = evaluate(content, script);
    if (value.isError())
    {
        QString error = value.toString() + " in script " + script + " in File: " +
                        value.property("fileName").toString() + " at Line: " +
                        value.property("lineNumber").toString();
        Console::print(error, Console::eERROR);
    }
}

Interpreter::~Interpreter()
{
    // free memory
    collectGarbage();
}


QJSValue Interpreter::doFunction(QString func, const QJSValueList& args)
{
    QJSValue ret;
    QJSValue funcPointer = globalObject().property(func);
#ifdef GAMEDEBUG
    if (funcPointer.isCallable())
    {
#endif
        ret = funcPointer.call(args);
        if (ret.isError())
        {
            QString error = ret.toString() + " in File: " +
                            ret.property("fileName").toString() + " at Line: " +
                            ret.property("lineNumber").toString();
            Console::print(error, Console::eERROR);
        }
#ifdef GAMEDEBUG
    }
    else
    {
        QString error = "Error: attemp to call a non function value. Call:" + func;
        Console::print(error, Console::eERROR);
    }
#endif
    return ret;
}

QJSValue Interpreter::doFunction(QString obj, QString func, const QJSValueList& args)
{
    QJSValue ret;
    QJSValue objPointer = globalObject().property(obj);
#ifdef GAMEDEBUG
    if (objPointer.isObject())
    {
#endif
        QJSValue funcPointer = objPointer.property(func);
#ifdef GAMEDEBUG
        if (funcPointer.isCallable())
        {
#endif
            ret = funcPointer.call(args);
            if (ret.isError())
            {
                QString error = ret.toString() + " in File: " +
                                ret.property("fileName").toString() + " at Line: " +
                                ret.property("lineNumber").toString();
                Console::print(error, Console::eERROR);
            }
#ifdef GAMEDEBUG
        }
        else
        {
            QString error = "Error: attemp to call a non function value. Call:" + obj + "." + func;
            Console::print(error, Console::eERROR);
        }
    }
    else
    {
        QString error = "Error: attemp to call a non object value in order to call a function. Call:" + obj + "." + func;
        Console::print(error, Console::eERROR);
    }

#endif
    return ret;
}

QJSValue Interpreter::doString(QString task)
{
    QJSValue value = evaluate(task, "GameCode");
    if (value.isError())
    {
        Console::print(value.toString(), Console::eERROR);
    }
    return value;
}

void Interpreter::pushInt(QString name, qint32 value)
{
    globalObject().setProperty(name, value);
}

void Interpreter::pushDouble(QString name, double value)
{
    globalObject().setProperty(name, value);
}

void Interpreter::pushString(QString name, QString value)
{
    globalObject().setProperty(name, value);
}

void Interpreter::pushObject(QString name, QObject* object)
{
    QJSValue newQObj = newQObject(object);
    globalObject().setProperty(name, newQObj);
}

QJSValue Interpreter::newQObject(QObject* object)
{
    QJSValue newQObj = QQmlEngine::newQObject(object);
    // make sure js never deletes our qobject since that's kinda not what we want
    QQmlEngine::setObjectOwnership(object, QQmlEngine::ObjectOwnership::CppOwnership);
    return newQObj;
}

void Interpreter::setCppOwnerShip(QObject* object)
{
    QQmlEngine::setObjectOwnership(object, QQmlEngine::ObjectOwnership::CppOwnership);
}

void Interpreter::cleanMemory()
{
    collectGarbage();
}

qint32 Interpreter::getGlobalInt(QString var)
{
    qint32 ret = 0;
    QJSValue value = globalObject().property(var);
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

bool Interpreter::getGlobalBool(QString var)
{
    bool ret = 0;
    QJSValue value = globalObject().property(var);
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

double Interpreter::getGlobalDouble(QString var)
{
    double ret = 0;
    QJSValue value = globalObject().property(var);
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

QString Interpreter::getGlobalString(QString var)
{
    QString ret = "";
    QJSValue value = globalObject().property(var);
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

QJSValue Interpreter::getGlobal(QString var)
{
    QJSValue value = globalObject().property(var);
    return value;
}

void Interpreter::setGlobal(QString var, QJSValue obj)
{
    globalObject().setProperty(var, obj);
}

void Interpreter::deleteObject(QString name)
{
    QString order = "delete " + name + ";";
    doString(order);
}

bool Interpreter::exists(QString object, QString function)
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

bool Interpreter::exists(QString object)
{
    QJSValue objPointer = globalObject().property(object);
    if (objPointer.isObject())
    {
        return true;
    }
    return false;
}
