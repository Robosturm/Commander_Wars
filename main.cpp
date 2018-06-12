#include "ox/oxygine.hpp"
#include "ox/Stage.hpp"
#include "ox/DebugActor.hpp"
#include <QObject>

#include "mainwindow.h"



#include <QQmlApplicationEngine>
#include <QJSEngine>

#include "coreengine/mainapp.h"
#include "coreengine/settings.h"
#include "coreengine/console.h"

int main(int argc, char* argv[])
{
    // qt metatypes


    /*************************************************************************************************/

    Mainapp app(argc, argv);
    Settings* pSettings = Settings::getInstance();
    // setup stuff is done here
    oxygine::ObjectBase::__startTracingLeaks();
    // Initialize Oxygine's internal stuff
    oxygine::core::init_desc desc;
    // translate all user visible texts with qt
    desc.title = QObject::tr("Commander Wars").toStdString().c_str();

    // The initial window size can be set up here on SDL builds, ignored on Mobile devices
    desc.w = pSettings->getWidth();
    desc.h = pSettings->getHeigth();
    desc.fullscreen = pSettings->getFullscreen();
    desc.borderless = pSettings->getBorderless();

    // init oxygine engine
    oxygine::core::init(&desc);

    // Create the stage. Stage is a root node for all updateable and drawable objects
    oxygine::Stage::instance = new oxygine::Stage();


    oxygine::Point size = oxygine::core::getDisplaySize();
    oxygine::getStage()->setSize(size);

    Console* pConsole = Console::getInstance();
    oxygine::getStage()->addChild(new Mainwindow());
    oxygine::getStage()->addChild(pConsole);


#ifdef GAMEDEBUG
    // DebugActor is a helper actor node. It shows FPS, memory usage and other useful stuff
    oxygine::DebugActor::show();
#endif

    app.setup();
    /*************************************************************************************************/
    // This is the main game loop.
    app.exec();

    /*************************************************************************************************/
    // clean up section ahead
    // store current settings when closing
    pSettings->saveSettings();

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

