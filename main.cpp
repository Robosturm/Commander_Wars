#include <QObject>
#include <QProcess>

#ifdef GAMEDEBUG
#include <QQmlApplicationEngine>
#include <QJSEngine>
#endif

#include "coreengine/audiothread.h"
#include "coreengine/mainapp.h"
#include "network/tcpserver.h"

#include "coreengine/console.h"

#include "coreengine/scriptvariable.h"
#include "coreengine/scriptvariables.h"

#include "ox/oxygine.hpp"
#include "ox/Stage.hpp"
#include "ox/DebugActor.hpp"

#include "menue/ingamemenue.h"
#include "menue/victorymenue.h"

#include "game/terrain.h"
#include "game/player.h"
#include "game/building.h"
#include "game/unit.h"
#include "game/co.h"
#include "game/gameaction.h"
#include "game/gameanimation.h"
#include "game/gameanimationwalk.h"
#include "game/gameanimationcapture.h"
#include "game/gameanimationdialog.h"
#include "game/gameanimationpower.h"
#include "game/gameanimationnextday.h"
#include "game/victoryrule.h"
#include "game/gamerules.h"
#include "game/weather.h"
#include "game/terrainfindingsystem.h"
#include "game/campaign.h"
#include "game/gamescript.h"


#include "gameinput/cursordata.h"
#include "gameinput/basegameinputif.h"

#include "game/GameEnums.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/buildingspritemanager.h"
#include "resource_management/cospritemanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/gameanimationmanager.h"
#include "resource_management/gamemanager.h"
#include "resource_management/gamerulemanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/terrainmanager.h"
#include "resource_management/unitspritemanager.h"
#include "resource_management/battleanimationmanager.h"

#include "ingamescriptsupport/scriptevent.h"
#include "ingamescriptsupport/scriptcondition.h"
#include "qfile.h"

#include "wiki/wikidatabase.h"

int main(int argc, char* argv[])
{
    QThread::currentThread()->setPriority(QThread::TimeCriticalPriority);
    /*************************************************************************************************/
    // setup network session support
    QNetworkConfigurationManager manager;
    // If the saved network configuration is not currently discovered use the system default
    QNetworkConfiguration config = manager.defaultConfiguration();
    QNetworkSession networkSession(config);
    networkSession.open();

    srand(static_cast<unsigned>(time(nullptr)));
#ifdef GAMEDEBUG
    qQmlEnableDebuggingHelper.startTcpDebugServer(3768);
#endif

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
    oxygine::Stage::instance->setVisible(false);

    oxygine::Point size = oxygine::core::getDisplaySize();
    oxygine::getStage()->setSize(size);

    app.setup();

    // qt metatypes we need this for js and signal slot stuff
    qRegisterMetaType<NetworkInterface::NetworkSerives>("NetworkInterface::NetworkSerives");
    qRegisterMetaType<VictoryMenue::GraphModes>("VictoryMenue::GraphModes");
    qRegisterMetaType<GameEnums::Alliance>("GameEnums::Alliance");
    qRegisterMetaType<GameEnums::UnitRanks>("GameEnums::UnitRanks");
    qRegisterMetaType<GameEnums::PowerMode>("GameEnums::PowerMode");
    qRegisterMetaType<GameEnums::RocketTarget>("GameEnums::RocketTarget");
    qRegisterMetaType<GameEnums::DefeatType>("GameEnums::DefeatType");
    qRegisterMetaType<GameEnums::Directions>("GameEnums::Directions");
    qRegisterMetaType<GameEnums::AIQuestionType>("GameEnums::AIQuestionType");
    qRegisterMetaType<GameEnums::Fog>("GameEnums::Fog");
    qRegisterMetaType<GameEnums::COMood>("GameEnums::COMood");
    qRegisterMetaType<GameEnums::LuckDamageMode>("GameEnums::LuckDamageMode");
    qRegisterMetaType<GameEnums::GameRecord_SpecialEvents>("GameEnums::GameRecord_SpecialEvents");
    qRegisterMetaType<GameEnums::GameAi>("GameEnums::GameAi");
    qRegisterMetaType<GameEnums::AnimationMode>("GameEnums::AnimationMode");
    qRegisterMetaType<GameEnums::BuildingTarget>("GameEnums::BuildingTarget");
    qRegisterMetaType<GameEnums::BattleAnimationMode>("GameEnums::BattleAnimationMode");
    qRegisterMetaType<SDL_Event>("SDL_Event");
    qRegisterMetaType<std::shared_ptr<QTcpSocket>>("std::shared_ptr<QTcpSocket>");
    qRegisterMetaType<spScriptEvent>("spScriptEvent");
    qRegisterMetaType<spScriptCondition>("spScriptCondition");
    qRegisterMetaType<WikiDatabase::pageData>("WikiDatabase::pageData");
    qRegisterMetaType<oxygine::spActor>("oxygine::spActor");

    qmlRegisterInterface<QmlVectorPoint>("QmlVectorPoint");
    qmlRegisterInterface<Terrain>("Terrain");
    qmlRegisterInterface<Player>("Player");
    qmlRegisterInterface<Building>("Building");
    qmlRegisterInterface<Unit>("Unit");
    qmlRegisterInterface<CO>("CO");
    qmlRegisterInterface<GameAction>("GameAction");
    qmlRegisterInterface<GameAnimation>("GameAnimation");
    qmlRegisterInterface<GameAnimationWalk>("GameAnimationWalk");
    qmlRegisterInterface<GameAnimationCapture>("GameAnimationCapture");
    qmlRegisterInterface<GameAnimationDialog>("GameAnimationDialog");
    qmlRegisterInterface<GameAnimationPower>("GameAnimationPower");
    qmlRegisterInterface<GameAnimationNextDay>("GameAnimationNextDay");
    qmlRegisterInterface<QmlVectorPoint>("QmlVectorPoint");
    qmlRegisterInterface<QmlVectorUnit>("QmlVectorUnit");
    qmlRegisterInterface<QmlVectorBuilding>("QmlVectorBuilding");
    qmlRegisterInterface<Mainapp>("Mainapp");
    qmlRegisterInterface<CursorData>("CursorData");    
    qmlRegisterInterface<VictoryRule>("VictoryRule");
    qmlRegisterInterface<GameRules>("GameRules");
    qmlRegisterInterface<ScriptVariable>("ScriptVariable");
    qmlRegisterInterface<ScriptVariables>("ScriptVariables");
    qmlRegisterInterface<Weather>("Weather");
    qmlRegisterInterface<TerrainFindingSystem>("TerrainFindingSystem");
    qmlRegisterInterface<GameRecorder>("GameRecorder");
    qmlRegisterInterface<GameScript>("GameScript");
    qmlRegisterInterface<Campaign>("Campaign");
    qmlRegisterInterface<BaseGameInputIF>("BaseGameInputIF");
    qmlRegisterInterface<Settings>("Settings");
    qmlRegisterInterface<Wikipage>("Wikipage");
    qmlRegisterInterface<oxygine::spActor>("oxygine::spActor");

    // load ressources
    BackgroundManager::getInstance();
    BuildingSpriteManager::getInstance();
    COSpriteManager::getInstance();
    FontManager::getInstance();
    GameAnimationManager::getInstance();
    GameManager::getInstance();
    GameRuleManager::getInstance();
    ObjectManager::getInstance();
    TerrainManager::getInstance();
    UnitSpriteManager::getInstance();
    BattleAnimationManager::getInstance();
    WikiDatabase::getInstance();

    /*************************************************************************************************/
    // This is the main game loop.
    app.start();
    oxygine::Stage::instance->setVisible(true);
    qint32 returncode = app.exec();
    app.getWorkerthread()->exit(0);
    /*************************************************************************************************/
    // clean up section ahead
    // store current settings when closing
    if (app.getGameServer() != nullptr)
    {
        app.stopGameServer();
    }
    networkSession.close();
    app.getSettings()->saveSettings();

    if (GameMap::getInstance() != nullptr)
    {
        GameMap::getInstance()->deleteMap();
    }

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
    if (returncode == 1)
    {
        QProcess::startDetached(QCoreApplication::applicationFilePath());
    }
    return returncode;
}
