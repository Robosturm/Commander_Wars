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
#include "coreengine/console.h"

#include "coreengine/crashreporter.h"
#include "coreengine/metatyperegister.h"

#ifdef UPDATESUPPORT
#include "updater/gameupdater.h"
#endif

int main(qint32 argc, char* argv[])
{
    qInstallMessageHandler(Console::messageOutput);
    srand(static_cast<unsigned>(time(nullptr)));
    QThread::currentThread()->setPriority(QThread::NormalPriority);
#ifdef GRAPHICSUPPORT
    QThread::currentThread()->setObjectName("RenderThread");
    QApplication app(argc, argv);
#else
    QCoreApplication app(argc, argv);
#endif
    app.setApplicationName("Commander Wars");
    app.setApplicationVersion(Mainapp::getGameVersion());

    Mainapp window;
    window.setTitle("Commander Wars");
    auto & parser = window.getParser();
    parser.parseArgsPhaseOne(app);
    Settings::getInstance();
    Settings::loadSettings();
    parser.parseArgsPhaseTwo();
    window.createBaseDirs();

    // start crash report handler
    CrashReporter::setSignalHandler(&Mainapp::showCrashReport);
    MetaTypeRegister::registerInterfaceData();
    /*************************************************************************************************/
    // show window according to window mode
    window.setPosition(Settings::getX(), Settings::getY());
    if (Settings::getSmallScreenDevice())
    {
        // force a resolution reset
        window.changeScreenMode(Settings::ScreenModes::FullScreen);
    }
    // show as normal borderless
    window.changeScreenMode(window.getScreenMode());
    window.setBrightness(Settings::getBrightness());
    window.setGamma(Settings::getGamma());
    if (window.getScreenMode() != Settings::ScreenModes::Window)
    {
        window.setPosition(Settings::getX(), Settings::getY());
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
    Settings::setX(window.x());
    Settings::setY(window.y());
    CrashReporter::setSignalHandler(nullptr);
    window.setShuttingDown(true);
    Userdata::getInstance()->release();
    Settings::shutdown();
    CONSOLE_PRINT("Shutting down main window", Console::eDEBUG);
    window.shutdown();
    CONSOLE_PRINT("Saving settings", Console::eDEBUG);
    Settings::saveSettings();
    // give os time to save the settings
    QThread::currentThread()->msleep(350);
    CONSOLE_PRINT("Checking for memory leak during runtime", Console::eDEBUG);
    static constexpr qint32 finalObjects = 2;
    if (oxygine::ref_counter::getAlloctedObjectCount() > finalObjects)
    {
        CONSOLE_PRINT("c++ memory leak detected. Objects not deleted: " + QString::number(oxygine::ref_counter::getAlloctedObjectCount()), Console::eWARNING);
    }
    else if (oxygine::ref_counter::getAlloctedObjectCount() < 0)
    {
        CONSOLE_PRINT("c++ memory detector is bugged: " + QString::number(oxygine::ref_counter::getAlloctedObjectCount()), Console::eWARNING);
    }
    //end
    if (!slave)
    {
        if (returncode == 1)
        {
#ifdef Q_OS_ANDROID
            CONSOLE_PRINT("No automatic restart on android", Console::eDEBUG);
#else
            CONSOLE_PRINT("Restarting application", Console::eDEBUG);
            QProcess::startDetached(QCoreApplication::applicationFilePath(), QStringList());
#endif
        }
#ifdef UPDATESUPPORT
        else if (returncode == 2)
        {
#ifdef Q_OS_ANDROID
            CONSOLE_PRINT("No update support on android", Console::eDEBUG);
#else
            GameUpdater::launchPatcher();
#endif
        }
        else if (returncode == 3)
        {
#ifdef Q_OS_ANDROID
            CONSOLE_PRINT("No update support on android", Console::eDEBUG);
#else
            GameUpdater::launchApplication();
#endif
        }
#endif
    }
    return returncode;
}
