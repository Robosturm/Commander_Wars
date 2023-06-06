#include <QObject>
#include <QProcess>
#include <QDir>
#ifdef GRAPHICSUPPORT
#include <QApplication>
#else
#include <QCoreApplication>
#endif
#include <QFile>

#include "coreengine/mainapp.h"
#include "coreengine/userdata.h"
#include "coreengine/gameconsole.h"

#include "coreengine/metatyperegister.h"
#include "coreengine/globalutils.h"

#ifdef UPDATESUPPORT
#include "updater/gameupdater.h"
#endif

int main(qint32 argc, char* argv[])
{
#ifdef GRAPHICSUPPORT
    QApplication app(argc, argv);
#else
    QCoreApplication app(argc, argv);
#endif
    GlobalUtils::setup();
    app.setApplicationName("Commander Wars");
    app.setApplicationVersion(Mainapp::getGameVersion());

    Mainapp window;
    window.setTitle("Commander Wars");
    {
        Settings* pSettings = Settings::getInstance()->getInstance();
        auto & parser = window.getParser();
        parser.parseArgsPhaseOne(app);
        GameConsole::getInstance();
        pSettings->setup();
        Settings::getInstance()->loadSettings();
        parser.parseArgsPhaseTwo();
    }
    window.createBaseDirs();

    // start crash report handler
    MetaTypeRegister::registerInterfaceData();
    /*************************************************************************************************/
    // show window according to window mode
    window.setPosition(Settings::getInstance()->getX(), Settings::getInstance()->getY());
    if (Settings::getInstance()->getSmallScreenDevice())
    {
        // force a resolution reset
        window.changeScreenMode(Settings::getInstance()->ScreenModes::FullScreen);
    }
    // show as normal borderless
    window.changeScreenMode(window.getScreenMode());
    window.setBrightness(Settings::getInstance()->getBrightness());
    window.setGamma(Settings::getInstance()->getGamma());
    if (window.getScreenMode() != Settings::getInstance()->ScreenModes::Window)
    {
        window.setPosition(Settings::getInstance()->getX(), Settings::getInstance()->getY());
    }
#ifdef GRAPHICSUPPORT
    if (window.getNoUi())
    {
        window.launchGame();
    }
#else
    window.launchGame();
#endif
    qint32 returncode = app.exec();
    /*************************************************************************************************/
    // shutting down
    bool slave = window.getSlave();
    Settings::getInstance()->setX(window.x());
    Settings::getInstance()->setY(window.y());
    window.setShuttingDown(true);
    Userdata::getInstance()->release();
    CONSOLE_PRINT("Saving settings", GameConsole::eDEBUG);
    Settings::getInstance()->saveSettings();
    // give os time to save the settings
    for (qint32 i = 0; i < 150; ++i)
    {
        QCoreApplication::processEvents();
        QThread::currentThread()->msleep(1);
    }
    CONSOLE_PRINT("Shutting down main window", GameConsole::eDEBUG);
    window.shutdown();
    // give os time to save the settings
    for (qint32 i = 0; i < 150; ++i)
    {
        QCoreApplication::processEvents();
        QThread::currentThread()->msleep(1);
    }
    CONSOLE_PRINT("Checking for memory leak during runtime", GameConsole::eDEBUG);
    static constexpr qint32 finalObjects = 20;
    if (oxygine::ref_counter::getAlloctedObjectCount() > finalObjects)
    {
        CONSOLE_PRINT("c++ memory leak detected. Objects not deleted: " + QString::number(oxygine::ref_counter::getAlloctedObjectCount()), GameConsole::eDEBUG);
    }
    else if (oxygine::ref_counter::getAlloctedObjectCount() < 0)
    {
        CONSOLE_PRINT("c++ memory detector is bugged: " + QString::number(oxygine::ref_counter::getAlloctedObjectCount()), GameConsole::eWARNING);
    }
    //end
    if (!slave)
    {
        if (returncode == 1)
        {
#ifdef Q_OS_ANDROID
            CONSOLE_PRINT("No automatic restart on android", GameConsole::eDEBUG);
#else
            CONSOLE_PRINT("Restarting application", GameConsole::eDEBUG);
            QProcess::startDetached(QCoreApplication::applicationFilePath(), QStringList());
#endif
        }
#ifdef UPDATESUPPORT
        else if (returncode == 2)
        {
#ifdef Q_OS_ANDROID
            CONSOLE_PRINT("No update support on android", GameConsole::eDEBUG);
#else
            GameUpdater::launchPatcher();
#endif
        }
        else if (returncode == 3)
        {
#ifdef Q_OS_ANDROID
            CONSOLE_PRINT("No update support on android", GameConsole::eDEBUG);
#else
            GameUpdater::launchApplication();
#endif
        }
#endif
    }
    return returncode;
}
