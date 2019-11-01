#include <QObject>
#include <QProcess>
#include <qdir.h>
#include "qguiapplication.h"

#ifdef GAMEDEBUG
#include <QQmlApplicationEngine>
#include <QJSEngine>
#endif

#include "coreengine/audiothread.h"
#include "coreengine/mainapp.h"

#include "coreengine/userdata.h"

#include "network/tcpserver.h"

#include "coreengine/console.h"

#include "coreengine/scriptvariable.h"
#include "coreengine/scriptvariables.h"

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
#include "wiki/wikidatabase.h"

#include "gameinput/cursordata.h"
#include "gameinput/basegameinputif.h"

#include "game/GameEnums.h"

#include "ingamescriptsupport/events/scriptevent.h"
#include "ingamescriptsupport/conditions/scriptcondition.h"
#include "qfile.h"

#include "oxygine/KeyEvent.h"

#include "QDomDocument"

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
    QGuiApplication app(argc, argv);
    Mainapp window;
    window.setTitle(QObject::tr("Commander Wars"));

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
    qRegisterMetaType<GameEnums::UnitType>("GameEnums::UnitType");
    qRegisterMetaType<std::shared_ptr<QTcpSocket>>("std::shared_ptr<QTcpSocket>");
    qRegisterMetaType<spScriptEvent>("spScriptEvent");
    qRegisterMetaType<spScriptCondition>("spScriptCondition");
    qRegisterMetaType<WikiDatabase::pageData>("WikiDatabase::pageData");
    qRegisterMetaType<oxygine::spActor>("oxygine::spActor");
    qRegisterMetaType<oxygine::KeyEvent>("oxygine::KeyEvent");

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

    /*************************************************************************************************/
    // show window according to window mode
    window.changeScreenMode(window.getScreenMode());
    qint32 returncode = app.exec();
    window.getWorkerthread()->exit(0);
    QDir dir("temp/");
    dir.removeRecursively();
    /*************************************************************************************************/
    // clean up section ahead
    // store current settings when closing
    if (window.getGameServer() != nullptr)
    {
        window.stopGameServer();
    }
    networkSession.close();
    window.getSettings()->saveSettings();

    if (GameMap::getInstance() != nullptr)
    {
        GameMap::getInstance()->deleteMap();
    }
    Userdata::getInstance()->storeUser();
    //end
    if (returncode == 1)
    {
        QProcess::startDetached(QCoreApplication::applicationFilePath());
    }
    return returncode;
}
