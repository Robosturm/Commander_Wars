#include "coreengine/interpreter.h"
#include "coreengine/globalutils.h"
#include "coreengine/audiomanager.h"
#include "coreengine/userdata.h"
#include "resource_management/fontmanager.h"
#include "resource_management/cospritemanager.h"
#include "resource_management/terrainmanager.h"
#include "resource_management/buildingspritemanager.h"
#include "resource_management/coperkmanager.h"
#include "resource_management/unitspritemanager.h"

#include <QDir>
#include <QQmlEngine>
#include <QTextStream>
#include <QThread>

spInterpreter Interpreter::m_pInstance{nullptr};

Interpreter::Interpreter()
    : QQmlEngine()
{
#ifdef GRAPHICSUPPORT
    setObjectName("Interpreter");
#endif
    setCppOwnerShip(this);
    connect(this, &Interpreter::sigNetworkGameFinished, this, &Interpreter::networkGameFinished, Qt::QueuedConnection);
}

bool Interpreter::reloadInterpreter(const QString runtime)
{
    CONSOLE_PRINT("Reloading interpreter", GameConsole::eDEBUG);
    m_pInstance = nullptr;
    m_pInstance = spInterpreter::create();
    m_pInstance->init();
    return m_pInstance->loadScript(runtime, "Interpreter Runtime");
}

Interpreter::~Interpreter()
{
    m_jsObjects.clear();
    // free memory
    collectGarbage();
}

void Interpreter::release()
{
    m_pInstance = nullptr;
}

void Interpreter::init()
{
    Mainapp* pApp = Mainapp::getInstance();
    setOutputWarningsToStandardError(false);
    setIncubationController(nullptr);

    QJSValue globals = newQObject(GlobalUtils::getInstance());
    globalObject().setProperty("globals", globals);
    QJSValue audio = newQObject(pApp->getAudioManager());
    globalObject().setProperty("audio", audio);
    QJSValue console = newQObject(GameConsole::getInstance());
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
    QJSValue unitSpriteManager = newQObject(UnitSpriteManager::getInstance());
    globalObject().setProperty("unitSpriteManager", unitSpriteManager);
    QJSValue buildingspritemanager = newQObject(BuildingSpriteManager::getInstance());
    globalObject().setProperty("buildingSpriteManager", buildingspritemanager);
    QJSValue terrainSpriteManager = newQObject(TerrainManager::getInstance());
    globalObject().setProperty("terrainSpriteManager", terrainSpriteManager);
    QJSValue coPerkSpriteManager = newQObject(COPerkManager::getInstance());
    globalObject().setProperty("coPerkSpriteManager", coPerkSpriteManager);

    GameEnums::registerEnums();

    installExtensions(QJSEngine::Extension::AllExtensions);
}

QString Interpreter::getRuntimeData()
{
    if (m_runtimeData.isEmpty())
    {
        CONSOLE_PRINT("Trying to access not loaded runtime data in no ui mode", GameConsole::eFATAL);
    }
    return m_runtimeData;
}

bool Interpreter::openScript(const QString & script, bool setup)
{
    bool success = false;
    QFile scriptFile(script);
    if (!scriptFile.open(QIODevice::ReadOnly))
    {
        QString error = "Error: attemp to read file " + script + " which could not be opened.";
        CONSOLE_PRINT(error, GameConsole::eERROR);
    }
    else if (!scriptFile.exists())
    {
        QString error = "Error: unable to open non existing file " + script + ".";
        CONSOLE_PRINT(error, GameConsole::eERROR);
    }
    else
    {
        CONSOLE_PRINT("Loading script " + script, GameConsole::eDEBUG);
        QTextStream stream(&scriptFile);
        QString contents = stream.readAll();
        if (setup && Settings::getRecord())
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
            CONSOLE_PRINT(error, GameConsole::eERROR);
        }
        else
        {
            success = true;
        }
    }
    return success;
}

bool Interpreter::loadScript(const QString & content, const QString & script)
{
    bool success = false;
    CONSOLE_PRINT("Interpreter::loadScript: " + script, GameConsole::eDEBUG);
    QJSValue value = evaluate(content, script);
    if (value.isError())
    {
        QString error = value.toString() + " in script " + script + " in File: " +
                        value.property("fileName").toString() + " at Line: " +
                        value.property("lineNumber").toString();
        CONSOLE_PRINT(error, GameConsole::eERROR);        
    }
    else
    {
        success = true;
    }
    return success;
}

QJSValue Interpreter::doString(const QString & task)
{
#ifdef GAMEDEBUG
    OXY_ASSERT(Mainapp::getInstance()->getWorkerthread() == QThread::currentThread());
#endif
    ++m_inJsCall;
    QJSValue value = evaluate(task, "GameCode");
    exitJsCall();
    if (value.isError())
    {
        CONSOLE_PRINT(value.toString(), GameConsole::eERROR);
    }
    return value;
}

void Interpreter::pushInt(const QString & name, qint32 value)
{
    globalObject().setProperty(name, value);
}

void Interpreter::pushDouble(const QString & name, double value)
{
    globalObject().setProperty(name, value);
}

void Interpreter::pushString(const QString & name, const QString & value)
{
    globalObject().setProperty(name, value);
}

void Interpreter::pushObject(const QString & name, QObject* object)
{
    QJSValue newQObj = newQObject(object);
    globalObject().setProperty(name, newQObj);
}

void Interpreter::setCppOwnerShip(QObject* object)
{
    QQmlEngine::setObjectOwnership(object, QQmlEngine::ObjectOwnership::CppOwnership);
}

void Interpreter::cleanMemory()
{
    collectGarbage();
}

qint32 Interpreter::getGlobalInt(const QString & var)
{
    qint32 ret = 0;
    QJSValue value = globalObject().property(var);
    if (!value.isNumber())
    {
        QString error = "Error: attemp to read " + var + "which is not from type number.";
        CONSOLE_PRINT(error, GameConsole::eERROR);
    }
    else
    {
        ret = value.toInt();
    }
    return ret;
}

bool Interpreter::getGlobalBool(const QString & var)
{
    bool ret = 0;
    QJSValue value = globalObject().property(var);
    if (!value.isBool())
    {
        QString error = "Error: attemp to read " + var + "which is not from type bool.";
        CONSOLE_PRINT(error, GameConsole::eERROR);
    }
    else
    {
        ret = value.toBool();
    }
    return ret;
}

double Interpreter::getGlobalDouble(const QString & var)
{
    double ret = 0;
    QJSValue value = globalObject().property(var);
    if (!value.isNumber())
    {
        QString error = "Error: attemp to read " + var + "which is not from type number.";
        CONSOLE_PRINT(error, GameConsole::eERROR);
    }
    else
    {
        ret = value.toNumber();
    }
    return ret;
}

QString Interpreter::getGlobalString(const QString & var)
{
    QString ret = "";
    QJSValue value = globalObject().property(var);
    if (!value.isString())
    {
        QString error = "Error: attemp to read " + var + "which is not from type QString.";
        CONSOLE_PRINT(error, GameConsole::eERROR);
    }
    else
    {
        ret = value.toString();
    }
    return ret;
}

QJSValue Interpreter::getGlobal(const QString & var)
{
    QJSValue value = globalObject().property(var);
    return value;
}

void Interpreter::setGlobal(const QString & var, const QJSValue & obj)
{
    globalObject().setProperty(var, obj);
}

void Interpreter::deleteObject(const QString & name)
{
    QString order = "Global[\"" + name + "\"] = null;\nGlobal[\"" + name + "\"] = undefined;";
    doString(order);
    collectGarbage();
}

void Interpreter::networkGameFinished(qint32 value, QString id)
{
    QString obj = "Init";
    QString func = "onRemoteGameFinished";
    if (exists(obj, func))
    {
        QJSValueList args({value,
                           id,});
        doFunction(obj, func, args);
    }
}

void Interpreter::exitJsCall()
{
    --m_inJsCall;
    Q_ASSERT(m_inJsCall >= 0);
}

bool Interpreter::getInJsCall() const
{
    return m_inJsCall > 0;
}

void Interpreter::trackJsObject(oxygine::ref_counter* pObj)
{
    if (m_inJsCall == 0)
    {
        m_jsObjects.clear();
    }
    if (m_inJsCall > 0)
    {
        m_jsObjects.append(oxygine::intrusive_ptr(pObj));
    }
}
