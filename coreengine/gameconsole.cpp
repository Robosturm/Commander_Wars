#include <QMutex>
#include <QMutexLocker>
#include <QString>
#include <QDir>
#include <qlogging.h>
#include <QLoggingCategory>
#include <QDateTime>
#include <QFontMetrics>
#include <QMetaMethod>
#include <QMetaObject>

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.h"
#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"

#include "coreengine/gameconsole.h"
#include "coreengine/mainapp.h"
#include "coreengine/settings.h"
#include "coreengine/audiomanager.h"
#include "coreengine/globalutils.h"
#include "coreengine/gameversion.h"

#include "game/gamemap.h"

#include "resource_management/fontmanager.h"

#include "spritingsupport/spritecreator.h"

// values which differ from release to debug build
GameConsole::eLogLevels GameConsole::m_LogLevel = static_cast<GameConsole::eLogLevels>(DEBUG_LEVEL);
quint64 GameConsole::m_activeModules = eGeneral | eJavaScript;
bool GameConsole::m_show = false;
bool GameConsole::m_toggled = false;
bool GameConsole::m_developerMode = false;
bool GameConsole::m_outputChanged = false;
std::vector<QString> GameConsole::m_output;
spConsole GameConsole::m_pConsole{nullptr};
qint32 GameConsole::m_curlastmsgpos = 0;
std::vector<QString> GameConsole::m_lastmsgs;
qint32 GameConsole::m_outputSize = 100;
QMutex GameConsole::m_datalocker;
QMutex GameConsole::messageOutputMutex;
// Console Libary
const char* const GameConsole::functions[] =
{
    "createfunnymessage",
    "setVolume",
    "setLogLevel",
    "connectToServer",
    "getServerAdresse",
    "createSprites",
    "createColorTableSprites",
    "applyImagesTable",
    "updateMaskImages",
    "inversImagesFrames",
    "extendMaskImages",
    "help",
    "logActions",
    "version",
    "setDeveloperMode",
    "extractResources",
    "memoryUsage",
    "printObjectDetails",
    "resetMapsGameRules",
    "updateTerrainPaletteMasks",
    "loadScript",
    ""
};
const char* const GameConsole::compileTime = __TIME__;
const char* const GameConsole::compileDate = __DATE__;

GameConsole::GameConsole()
{
#ifdef GRAPHICSUPPORT
    setObjectName("Console");
#endif
    Interpreter::setCppOwnerShip(this);
    m_output.reserve(m_outputSize);
    m_lastmsgs.reserve(m_lastMsgSize);
}

void GameConsole::setActiveModules(quint64 newActiveModules)
{
    m_activeModules = newActiveModules;
}

void GameConsole::setModuleMode(quint64 newActiveModule, bool enable)
{
    if (enable)
    {
        m_activeModules |= newActiveModule;
    }
    else
    {
        m_activeModules &= ~newActiveModule;
    }
}

bool GameConsole::isActiveModule(quint64 module)
{
    return m_activeModules & module;
}

spConsole GameConsole::getSpInstance()
{
    if (m_pConsole.get() == nullptr)
    {
        m_pConsole = MemoryManagement::create<GameConsole>();
        qInstallMessageHandler(GameConsole::messageOutput);
    }
    return m_pConsole;
}

GameConsole* GameConsole::getInstance()
{
    return getSpInstance().get();
}

bool GameConsole::hasInstance()
{
    return m_pConsole.get() != nullptr;
}

void GameConsole::init()
{
    // move console to top
    oxygine::Actor::setPriority(static_cast<qint32>(Mainapp::ZOrder::Console));
    m_pBackgroundsprite = MemoryManagement::create<oxygine::ColorRectSprite>();
    m_pBackgroundsprite->setPosition(0, 0);
    m_pBackgroundsprite->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
    addChild(m_pBackgroundsprite);
    m_pBackgroundsprite->setColor(QColor(0,0,0, 180));

    m_text = MemoryManagement::create<oxygine::TextField>();
    m_text->setPosition(1, 1);
    m_text->setWidth(oxygine::Stage::getStage()->getWidth() - 2);
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getFont("console16"));
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    m_text->setStyle(style);
    addChild(m_text);

    m_editTextfield = MemoryManagement::create<oxygine::TextField>();
    m_editTextfield->setPosition(1, 1);
    m_editTextfield->setWidth(oxygine::Stage::getStage()->getWidth() - 2);
    m_editTextfield->setStyle(style);
    addChild(m_editTextfield);

    // we're hidden to begin with
    oxygine::Actor::setVisible(false);
    connect(this, &GameConsole::sigToggleView, this, &GameConsole::toggleView, Qt::QueuedConnection);
    addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event* event)
    {
        event->stopPropagation();
        emit sigFocused();
    });
    // Print some Info
    CONSOLE_PRINT("Started with: " + QCoreApplication::arguments().join(" "), GameConsole::eINFO);
    CONSOLE_PRINT("Enter \"help()\" for console info.", GameConsole::eLogLevels::eINFO);
    CONSOLE_PRINT("Starting Game...", GameConsole::eLogLevels::eINFO);
    CONSOLE_PRINT("Prepare to Fight...", GameConsole::eLogLevels::eINFO);
    CONSOLE_PRINT("Read this message while waiting...", GameConsole::eLogLevels::eINFO);
    CONSOLE_PRINT("", GameConsole::eLogLevels::eINFO);
    GameConsole::createfunnymessage();
    CONSOLE_PRINT("", GameConsole::eLogLevels::eINFO);
}

void GameConsole::release()
{
    if (m_pBackgroundsprite.get() != nullptr)
    {
        m_pBackgroundsprite->detach();
        m_pBackgroundsprite = oxygine::spSprite();
    }
    if (m_text.get() != nullptr)
    {
        m_text->detach();
        m_text = oxygine::spTextField();
    }
}

void GameConsole::createSprites(QString input, QString colorTable, QString maskTable)
{
    SpriteCreator::createSprites(input, colorTable, maskTable);
}

void GameConsole::createColorTableSprites(QString folder, QString filter, qint32 startIndex, qint32 maxColors)
{
    SpriteCreator::createColorTableSprites(folder, filter, startIndex, maxColors);
}

void GameConsole::updateMaskImages(QString folder, QString filter, qint32 min)
{
    SpriteCreator::updateMaskImages(folder, filter, min);
}

void GameConsole::inversImagesFrames(QString folder, QString filter, qint32 frames)
{
    SpriteCreator::inversImagesFrames(folder, filter, frames);
}

void GameConsole::applyImagesTable(QString input, QString inTable, QString outTable, QColor eraseColor)
{
    SpriteCreator::applyImagesTable(input, inTable, outTable, eraseColor);
}

void GameConsole::extendMaskImages(QString folder, QString filter)
{
    SpriteCreator::extendMaskImages(folder, filter);
}

void GameConsole::updateTerrainPaletteMasks(const QString& folder, const QString& filter, qint32 originalPaletteSize, qint32 newPaletteSize, qint32 xOffset, qint32 yOffset)
{
    SpriteCreator::updateTerrainPaletteMasks(folder, filter, originalPaletteSize, newPaletteSize, xOffset, yOffset);
}

void GameConsole::loadScript(QString & filepath)
{
    Interpreter::getInstance()->openScript(filepath, false);
}

void GameConsole::dotask(QString message)
{
    print(message, GameConsole::eINFO);
    if (message.startsWith("game:"))
    {
        message = message.replace("game:", "");
        emit getInstance()->sigExecuteCommand(message);
    }
    else
    {
        QString order = "GameConsole." + message;
        for (const auto& function : functions)
        {
            if (message.startsWith(QString(function) + "("))
            {
                Interpreter::getInstance()->doString(order);
                break;
            }
        }
    }
}

void GameConsole::print(const QString & message, qint8 logLevel)
{
    if (m_activeModules & eJavaScript)
    {
        printDirectly(message, static_cast<eLogLevels>(logLevel));
    }
}

void GameConsole::printDirectly(const QString & message, eLogLevels logLevel)
{
    QMutexLocker locker(&m_datalocker);

    if (logLevel >= GameConsole::m_LogLevel)
    {
        QString msg = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + ": " + message;
        QString prefix = "";
        switch (logLevel)
        {
            case eDEBUG:
            {
                qDebug("%s", msg.toStdString().c_str());
                prefix = "DEBUG: ";
                break;
            }
            case eINFO:
            {
                qInfo("%s", msg.toStdString().c_str());
                prefix = "INFO: ";
                break;
            }
            case eWARNING:
            {
                qWarning("%s", msg.toStdString().c_str());
                prefix = "WARNING: ";
                break;
            }
            case eERROR:
            {
                qCritical("%s", msg.toStdString().c_str());
                prefix = "CRITICAL: ";
                break;
            }
            case eFATAL:
            {
                prefix = "FATAL: ";
                qFatal("%s", msg.toStdString().c_str());
                break;
            }
            default:
                break;
        }
        msg.replace("&", "&amp;");
        m_output.push_back(prefix + msg);
        if (m_output.size() > m_outputSize)
        {
            m_output.erase(m_output.cbegin(), m_output.cbegin() + m_output.size() - m_outputSize);
        }
        m_outputChanged = true;
    }
}

void GameConsole::printObjectDetails(QObject* obj, GameConsole::eLogLevels logLevel)
{
    print("Object Info of :" + obj->objectName(), logLevel);
    const auto * metaObject = obj->metaObject();
    qint32 functions = metaObject->methodCount();
    for (qint32 i = 0; i < functions; ++i)
    {
        const auto metaFunction = metaObject->method(i);
        if (metaFunction.methodType() ==  QMetaMethod::Slot)
        {
            print("function: " + QString(metaFunction.typeName()) + " " + QString(metaFunction.methodSignature()), logLevel);
        }
    }
}

void GameConsole::update(const oxygine::UpdateState& us)
{
    // no need to calculate more than we need if we're invisible
    if(m_show)
    {
#ifdef GRAPHICSUPPORT
        QMutexLocker locker(&m_datalocker);
        if (m_outputChanged)
        {
            qint32 screenheight = oxygine::Stage::getStage()->getHeight();
            auto font = FontManager::getFont("console16");
            QFontMetrics metrics(font.font);
            qint32 lineHeight = metrics.height();
            // create output text
            QString drawText;
            qint32 i = m_output.size() - 1;
            qint32 currentHeight = 0;
            while (i >= 0 && currentHeight < screenheight - lineHeight * 3)
            {
                QString testDrawText = ("> " + m_output[i] + "\n") + drawText;
                qint32 nextHeight = testDrawText.split("\n").size() * lineHeight;
                if (nextHeight > screenheight - lineHeight * 3)
                {
                    break;
                }
                else
                {
                    drawText = testDrawText;
                    currentHeight = nextHeight;
                }
                --i;
            }
            m_text->setHtmlText(drawText);
            m_editTextfield->setY(currentHeight + m_text->getY());
            m_outputChanged = false;
        }
        if (m_focused)
        {
            // create blinking cursor position
            QString lineText = "> </r>" +  getDrawText(getCurrentText());
            m_editTextfield->setHtmlText("<r>" + lineText);
        }
        else
        {
            QString lineText = "> Click the console to regain focus";
            m_editTextfield->setHtmlText(lineText);
        }
#endif
    }
    oxygine::Actor::update(us);
}

void GameConsole::toggleView()
{
    m_show = !m_show;
    oxygine::Actor::setVisible(m_show);
    if (m_show)
    {
        m_pBackgroundsprite->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
        emit sigFocused();
    }
    else
    {
       emit sigLooseFocusInternal();
    }
    m_toggled = true;
}

bool GameConsole::getDeveloperMode()
{
    if (m_developerMode)
    {
        printDirectly("Developer Mode enabled! And used for changing some data.", GameConsole::eINFO);
    }
    return m_developerMode;
}

void GameConsole::setDeveloperMode(bool developerMode)
{
    m_developerMode = developerMode;
    if (m_developerMode)
    {
        printDirectly("Developer Mode enabled! Note this may lead to crashes or weird behaviour.", GameConsole::eWARNING);
        setLogLevel(eLogLevels::eDEBUG);
    }
}

void GameConsole::setVolume(qint32 volume)
{
    Mainapp::getInstance()->getAudioManager()->setVolume(volume);
}

void GameConsole::setLogLevel(eLogLevels newLogLevel)
{
    GameConsole::m_LogLevel = newLogLevel;
}

void GameConsole::help(qint32 start, qint32 end)
{
    qint32 index = 0;
    while (QString(functions[index]) != "" && ((end >= 0 && index <= end) ||  end < 0))
    {
        if (index >= start && (index <= end || end < 0))
        {
            CONSOLE_PRINT(functions[index], GameConsole::eINFO);
        }
        index++;
    }
}

void GameConsole::version()
{
    printDirectly(QCoreApplication::applicationVersion() + " Builddate: " + compileDate + " " + compileTime, GameConsole::eINFO);
}

void GameConsole::logActions(bool log)
{
    Settings::getInstance()->setLogActions(log);
}

void GameConsole::extractResources()
{
    QString targetDir = QString("extractedResources_Version_") + GameVersion().toString().replace(".", "_") + "/";
    QDir target(targetDir);
    if (target.exists())
    {
        CONSOLE_PRINT("Deleting old extracted files.", GameConsole::eINFO);
        target.removeRecursively();
    }
    QStringList filter;
    filter << "*.*";
    QDirIterator dirIter(oxygine::Resource::RCC_PREFIX_PATH, filter, QDir::Files, QDirIterator::Subdirectories);
    qint32 count = 0;
    while (dirIter.hasNext())
    {
        dirIter.next();
        count++;
        QFile file(dirIter.fileInfo().canonicalFilePath());
        QString relativePath = GlobalUtils::makePathRelative(dirIter.fileInfo().canonicalFilePath());
        QString dir = GlobalUtils::makePathRelative(dirIter.fileInfo().absoluteDir().canonicalPath());
        QDir newDir(targetDir + dir);
        newDir.mkpath(".");
        file.copy(targetDir + relativePath);
        QFile permission(targetDir + relativePath);
        permission.setPermissions(QFileDevice::ReadOther | QFileDevice::WriteOther);
        if (count % 40 == 0)
        {
            CONSOLE_PRINT("Extracted files " + QString::number(count) + "...", GameConsole::eINFO);
        }
    }
    CONSOLE_PRINT("Extracting files done. Extracted: " + QString::number(count) + " files", GameConsole::eINFO);
}

void GameConsole::memoryUsage()
{
    CONSOLE_PRINT("Textures=" + QString::number(oxygine::Texture::getHighestTextureCount()), GameConsole::eINFO);
    CONSOLE_PRINT("Materials cached=" + QString::number(oxygine::MaterialCache::mc().getSize()), GameConsole::eINFO);
}

void GameConsole::resetMapsGameRules(const QString & folder)
{
    QStringList filters;
    filters << "*.map";
    QDirIterator dirIter(folder, filters, QDir::Files, QDirIterator::IteratorFlag::Subdirectories);
    while (dirIter.hasNext())
    {
        dirIter.next();
        QString filePath = dirIter.fileInfo().canonicalFilePath();
        spGameMap pMap = MemoryManagement::create<GameMap>(filePath, true, true, false);
        pMap->getGameRules()->reset();
        QFile file(filePath);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Version::Qt_6_5);
        pMap->serializeObject(stream);
        file.close();
    }
}

static const QStringList& getFunnyMessages() {
    static QStringList list;
    bool finished = false;
    if (!finished)
    {
        QFile inputFile(":/system/startup_messages.txt");
        if (inputFile.open(QIODevice::ReadOnly))
        {
            QTextStream in(&inputFile);
            while (!in.atEnd())
            {
                QString line = in.readLine();
                if (!line.isEmpty())
                {
                    list.append(line);
                }
            }
            inputFile.close();
        }

        finished = true;
    }
    return list;
}

void GameConsole::createfunnymessage(qint32 message) {
    const QStringList& messages = getFunnyMessages();
    if (message < 0)
    {
        message = GlobalUtils::randIntBase(0, messages.length());
    }

    QString printmessage = "No more funny Messages found. Delete your Harddisk instead";
    if (message < messages.length())
    {
        printmessage = messages[message];
    }
    printDirectly(printmessage, GameConsole::eINFO);
}

bool GameConsole::doHandleEvent(QEvent *event)
{
    bool ret = false;
    // for debugging
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* inputEvent = static_cast<QKeyEvent*>(event);
        Qt::Key cur = static_cast<Qt::Key>(inputEvent->key());
        if (cur == Settings::getInstance()->getKeyConsole())
        {
            toggleView();
            ret = true;
        }
        else
        {
            //Handle Key Input for the console
            switch(cur)
            {
                case Qt::Key_Up:
                {
                    m_curlastmsgpos--;
                    if(m_curlastmsgpos < 0)
                    {
                        m_curlastmsgpos = m_lastmsgs.size() - 1;
                    }
                    if(m_curlastmsgpos < m_lastmsgs.size() && m_curlastmsgpos >= 0)
                    {
                        QString msg = m_lastmsgs[m_curlastmsgpos];
                        setCurrentText(msg);
                        setCursorPosition(msg.size());
                    }
                    ret = true;
                    break;
                }
                case Qt::Key_Down:
                {
                    m_curlastmsgpos++;
                    if(m_curlastmsgpos >= m_lastmsgs.size())
                    {
                        m_curlastmsgpos = 0;
                    }
                    if(m_curlastmsgpos < m_lastmsgs.size())
                    {
                        QString msg = m_lastmsgs[m_curlastmsgpos];
                        setCurrentText(msg);
                        setCursorPosition(msg.size());
                    }
                    ret = true;
                    break;
                }
                default:
                {
                    ret = TextInput::doHandleEvent(event);
                    break;
                }
            }
        }
    }
    else
    {
        ret = TextInput::doHandleEvent(event);
    }
    return ret;
}

bool GameConsole::onEditFinished()
{
    QString message = getCurrentText();
    if (!message.isEmpty())
    {
        dotask(message);
        m_lastmsgs.push_back(message);
        while (m_lastmsgs.size() > m_lastMsgSize)
        {
            m_lastmsgs.erase(m_lastmsgs.cbegin());
        }
        m_curlastmsgpos = m_lastmsgs.size();
        setCurrentText("");
        setCursorPosition(0);
    }
    return false;
}

void GameConsole::messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QFile file;
    static QTextStream stream(&file);
    QMutexLocker lock(&messageOutputMutex);
    if (!file.isOpen())
    {
        QString date = QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss");
        if (Settings::getInstance()->getServer())
        {
            file.setFileName((Settings::getInstance()->getUserPath() + "console-" + date + ".log"));
        }
        else
        {
            file.setFileName((Settings::getInstance()->getUserPath() + "console" + Settings::getInstance()->getUpdateStep()+ ".log"));
        }
        if (Settings::getInstance()->getAiSlave())
        {
            file.setFileName(Settings::getInstance()->getUserPath() + "consoleAiSlave.log");
            file.open(QIODevice::WriteOnly);
        }
        Mainapp* pApp = Mainapp::getInstance();
        if (pApp->getSlave() && pApp->getCreateSlaveLogs())
        {
            QString slaveName = Settings::getInstance()->getSlaveServerName();
            file.setFileName(Settings::getInstance()->getUserPath() + slaveName + "-" + date + ".log");
            file.open(QIODevice::WriteOnly);
        }
        else if (!pApp->getSlave())
        {
            file.open(QIODevice::WriteOnly);
        }
    }
    if (file.isOpen())
    {
        // QByteArray localMsg = msg.toLocal8Bit();
        switch (type)
        {
            case QtDebugMsg:
                if (GameConsole::m_LogLevel <= GameConsole::eLogLevels::eDEBUG)
                {
                    stream << "Debug: " << msg << " File: " << context.file << " Line: " << context.line << " Function: " << context.function << "\n";
                    // fprintf(stdout, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
                    stream.flush();
                    file.flush();
                }
                break;
            case QtInfoMsg:
                if (GameConsole::m_LogLevel <= GameConsole::eLogLevels::eINFO)
                {
                    stream << "Info: " << msg << " File: " << context.file << " Line: " << context.line << " Function: " << context.function << "\n";
                    // fprintf(stdout, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
                    stream.flush();
                    file.flush();
                }
                break;
            case QtWarningMsg:
                if (GameConsole::m_LogLevel <= GameConsole::eLogLevels::eWARNING)
                {
                    stream << "Warning: " << msg << " File: " << context.file << " Line: " << context.line << " Function: " << context.function << "\n";
                    // fprintf(stdout, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
                    stream.flush();
                    file.flush();
                }
                break;
            case QtCriticalMsg:
                if (GameConsole::m_LogLevel <= GameConsole::eLogLevels::eERROR)
                {
                    stream << "Critical: " << msg << " File: " << context.file << " Line: " << context.line << " Function: " << context.function << "\n";
                    // fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
                    stream.flush();
                    file.flush();
                }
                break;
            case QtFatalMsg:
                if (GameConsole::m_LogLevel <= GameConsole::eLogLevels::eFATAL)
                {
                    stream << "Fatal: " << msg << " File: " << context.file << " Line: " << context.line << " Function: " << context.function << "\n";
                    // fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
                    stream.flush();
                    file.flush();
                }
                break;
        }
    }
}
