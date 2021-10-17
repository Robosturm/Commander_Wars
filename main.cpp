#include <QObject>
#include <QProcess>
#include <QDir>
#include <QApplication>
#include <QFile>

#include "coreengine/mainapp.h"
#include "coreengine/userdata.h"
#include "coreengine/console.h"

#include "coreengine/crashreporter.h"
#include "coreengine/metatyperegister.h"

#include "network/mainserver.h"

int main(qint32 argc, char* argv[])
{
    qInstallMessageHandler(Console::messageOutput);
    srand(static_cast<unsigned>(time(nullptr)));
    QThread::currentThread()->setPriority(QThread::NormalPriority);
    QThread::currentThread()->setObjectName("RenderThread");
    QApplication app(argc, argv);
    app.setApplicationName("Commander Wars");
    app.setApplicationVersion(Mainapp::getGameVersion());
    Settings::getInstance();
    Settings::loadSettings();

    Mainapp window;
    window.setTitle("Commander Wars");
    QStringList args = app.arguments();
    window.loadArgs(args);
    // start crash report handler
    CrashReporter::setSignalHandler(&Mainapp::showCrashReport);
    MetaTypeRegister::registerInterfaceData();
    /*************************************************************************************************/
    // show window according to window mode
    window.setPosition(Settings::getX(), Settings::getY());
    window.changeScreenMode(window.getScreenMode());

    window.setBrightness(Settings::getBrightness());
    window.setGamma(Settings::getGamma());
    if (window.getScreenMode() != 0)
    {
        window.setPosition(Settings::getX(), Settings::getY());
    }
    qint32 returncode = app.exec();
    /*************************************************************************************************/
    // shutting down
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
    if (MainServer::exists())
    {
        CONSOLE_PRINT("Shutting dwon game server", Console::eDEBUG);
        MainServer::getInstance()->deleteLater();
        window.getGameServerThread()->quit();
        window.getGameServerThread()->wait();
    }
    CONSOLE_PRINT("Checking for memory leak during runtime", Console::eDEBUG);
    static constexpr qint32 finalObjects = 0;
    if (oxygine::ref_counter::getAlloctedObjectCount() != finalObjects)
    {
        oxygine::handleErrorPolicy(oxygine::ep_show_error, "c++ memory leak detected. Objects not deleted: " + QString::number(oxygine::ref_counter::getAlloctedObjectCount()));
    }
    else if (oxygine::ref_counter::getAlloctedJsObjectCount() != finalObjects)
    {
        oxygine::handleErrorPolicy(oxygine::ep_show_error, "js memory leak detected. This happens due to not deleted qml-vectors in a mod. Objects not deleted: " + QString::number(oxygine::ref_counter::getAlloctedObjectCount()));
    }
    //end
    if (returncode == 1)
    {
#ifdef Q_OS_ANDROID
        CONSOLE_PRINT("No automatic restart on android", Console::eDEBUG);
#else
        CONSOLE_PRINT("Restarting application", Console::eDEBUG);
        QProcess::startDetached(QCoreApplication::applicationFilePath(), QStringList());
#endif
    }
    return returncode;
}
