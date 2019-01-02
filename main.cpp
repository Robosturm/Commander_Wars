#include <QObject>
#include <QDirIterator>

#include "coreengine/audiothread.h"

#include "coreengine/mainapp.h"


#include <QQmlApplicationEngine>
#include <QJSEngine>


#include "coreengine/console.h"

#include "ox/oxygine.hpp"
#include "ox/Stage.hpp"
#include "ox/DebugActor.hpp"

#include "menue/mainwindow.h"
#include "menue/ingamemenue.h"

#include "game/terrain.h"
#include "game/player.h"
#include "game/building.h"
#include "game/unit.h"
#include "game/ko.h"
#include "game/gameaction.h"

int main(int argc, char* argv[])
{

    /*************************************************************************************************/

    Mainapp app(argc, argv);

    // setup stuff is done here
    oxygine::ObjectBase::__startTracingLeaks();
    // Initialize Oxygine's internal stuff
    oxygine::core::init_desc desc;
    // translate all user visible texts with qt
    desc.title = QObject::tr("Commander Wars").toStdString().c_str();

    // The initial window size can be set up here on SDL builds, ignored on Mobile devices
    desc.w = app.getSettings()->getWidth();
    desc.h = app.getSettings()->getHeight();
    desc.fullscreen = app.getSettings()->getFullscreen();
    desc.borderless = app.getSettings()->getBorderless();

    // init oxygine engine
    oxygine::core::init(&desc);

    // Create the stage. Stage is a root node for all updateable and drawable objects
    oxygine::Stage::instance = new oxygine::Stage();


    oxygine::Point size = oxygine::core::getDisplaySize();
    oxygine::getStage()->setSize(size);

    Console* pConsole = Console::getInstance();
    // create the initial menue no need to store the object
    // it will add itself to the current stage
    oxygine::getStage()->addChild(pConsole);


#ifdef GAMEDEBUG
    // DebugActor is a helper actor node. It shows FPS, memory usage and other useful stuff
    //oxygine::DebugActor::show();
#endif

    app.setup();

    // qt metatypes we need this for js and signal slot stuff
    qRegisterMetaType<Mainapp::NetworkSerives>("Mainapp::NetworkSerives");
    qRegisterMetaType<Player::Alliance>("Player::Alliance");
    qmlRegisterInterface<Terrain>("Terrain");
    qmlRegisterInterface<Player>("Player");
    qmlRegisterInterface<Building>("Building");
    qmlRegisterInterface<Unit>("Unit");
    qmlRegisterInterface<KO>("KO");
    qmlRegisterInterface<GameAction>("GameAction");

    // load General-Base Scripts
    QString path =  QCoreApplication::applicationDirPath() + "/resources/scripts/general";
    QStringList filter;
    filter << "*.js";
    QDirIterator* dirIter = new QDirIterator(path, filter, QDir::Files, QDirIterator::Subdirectories);
    while (dirIter->hasNext())
    {
        dirIter->next();
        QString file = dirIter->fileInfo().absoluteFilePath();
        app.getInterpreter()->openScript(file);
    }
    delete dirIter;
    oxygine::getStage()->addChild(new Mainwindow());
    /*************************************************************************************************/
    // This is the main game loop.
    app.exec();

    /*************************************************************************************************/
    // clean up section ahead
    // store current settings when closing
    app.getSettings()->saveSettings();

    // If we get here, the user has requested the Application to terminate.
    // We dump and log all our created objects that have not been freed yet
    oxygine::ObjectBase::dumpCreatedObjects();

    // Releases all internal components and the stage
    oxygine::core::release();

    // The dump list should be empty by now,
    // we want to make sure that there aren't any memory leaks, so we call it again.
    oxygine::ObjectBase::dumpCreatedObjects();

    oxygine::ObjectBase::__stopTracingLeaks();
    //end

	return 0;
}

