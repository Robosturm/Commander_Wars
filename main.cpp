#include <QObject>
#include <QProcess>
#include <qdir.h>
#include <qapplication.h>

#ifdef GAMEDEBUG
#include <QQmlApplicationEngine>
#include <QJSEngine>
#include <QQmlContext>
#endif

#include "coreengine/audiothread.h"
#include "coreengine/mainapp.h"

#include "coreengine/userdata.h"

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
#include "game/gamerule.h"
#include "game/weather.h"
#include "game/terrainfindingsystem.h"
#include "game/campaign.h"
#include "game/gamescript.h"
#include "wiki/wikidatabase.h"

#include "objects/base/textbox.h"

#include "gameinput/cursordata.h"
#include "gameinput/basegameinputif.h"

#include "ai/heavyai.h"

#include "game/GameEnums.h"

#include "ingamescriptsupport/events/scriptevent.h"
#include "ingamescriptsupport/conditions/scriptcondition.h"
#include "qfile.h"

#include "oxygine/KeyEvent.h"

#include "coreengine/crashreporter.h"

#include "network/mainserver.h"

#include "ai/neuralnetwork/neural/neuralnetwork.h"

int main(int argc, char* argv[])
{
    qInstallMessageHandler(Console::messageOutput);
    srand(static_cast<unsigned>(time(nullptr)));
    QThread::currentThread()->setPriority(QThread::TimeCriticalPriority);
#ifdef GAMEDEBUG
    QQmlDebuggingEnabler enabler;
#endif

    QApplication app(argc, argv);
    app.setApplicationName(QObject::tr("Commander Wars"));
    app.setApplicationVersion(Mainapp::getGameVersion());
    Settings::loadSettings();
    QDir dir("temp/");
    dir.removeRecursively();
    dir.mkpath(".");
    Mainapp window;
    window.setTitle(QObject::tr("Commander Wars"));
    QStringList args = app.arguments();
    window.loadArgs(args);
    // start crash report handler
    crashReporter::setSignalHandler(&Mainapp::showCrashReport);

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
    qRegisterMetaType<GameEnums::VisionType>("GameEnums::VisionType");
    qRegisterMetaType<GameEnums::AiTypes>("GameEnums::AiTypes");
    qRegisterMetaType<GameEnums::Recoloring>("GameEnums::Recoloring");
    qRegisterMetaType<GameEnums::COInfoPosition>("GameEnums::COInfoPosition");
    qRegisterMetaType<std::shared_ptr<QTcpSocket>>("std::shared_ptr<QTcpSocket>");
    qRegisterMetaType<spScriptEvent>("spScriptEvent");
    qRegisterMetaType<spScriptCondition>("spScriptCondition");
    qRegisterMetaType<WikiDatabase::pageData>("WikiDatabase::pageData");
    qRegisterMetaType<oxygine::spActor>("oxygine::spActor");
    qRegisterMetaType<oxygine::spResAnim>("oxygine::spResAnimf");
    qRegisterMetaType<oxygine::KeyEvent>("oxygine::KeyEvent");
    qRegisterMetaType<oxygine::MouseButton>("oxygine::MouseButton");
    qRegisterMetaType<QVector<std::tuple<QString,float>>>("QVector<std::tuple<QString,float>>");
    qRegisterMetaType<QVector<float>>("QVector<float>");
    qRegisterMetaType<QList<qint32>>("QList<qint32>");
    qRegisterMetaType<QLocalSocket::LocalSocketError>("QLocalSocket::LocalSocketError");
    qRegisterMetaType<spTextbox>("spTextbox");
    qRegisterMetaType<spGameAction>("spGameAction");

    qmlRegisterInterface<QmlVectorPoint>("QmlVectorPoint", 1);
    qmlRegisterInterface<Terrain>("Terrain", 1);
    qmlRegisterInterface<Player>("Player", 1);
    qmlRegisterInterface<Building>("Building", 1);
    qmlRegisterInterface<Unit>("Unit", 1);
    qmlRegisterInterface<CO>("CO", 1);
    qmlRegisterInterface<GameAction>("GameAction", 1);
    qmlRegisterInterface<GameAnimation>("GameAnimation", 1);
    qmlRegisterInterface<GameAnimationWalk>("GameAnimationWalk", 1);
    qmlRegisterInterface<GameAnimationCapture>("GameAnimationCapture", 1);
    qmlRegisterInterface<GameAnimationDialog>("GameAnimationDialog", 1);
    qmlRegisterInterface<GameAnimationPower>("GameAnimationPower", 1);
    qmlRegisterInterface<GameAnimationNextDay>("GameAnimationNextDay", 1);
    qmlRegisterInterface<QmlVectorPoint>("QmlVectorPoint", 1);
    qmlRegisterInterface<QmlVectorUnit>("QmlVectorUnit", 1);
    qmlRegisterInterface<QmlVectorBuilding>("QmlVectorBuilding", 1);
    qmlRegisterInterface<Mainapp>("Mainapp", 1);
    qmlRegisterInterface<CursorData>("CursorData", 1);
    qmlRegisterInterface<VictoryRule>("VictoryRule", 1);
    qmlRegisterInterface<GameRules>("GameRules", 1);
    qmlRegisterInterface<GameRule>("GameRule", 1);
    qmlRegisterInterface<ScriptVariable>("ScriptVariable", 1);
    qmlRegisterInterface<ScriptVariables>("ScriptVariables", 1);
    qmlRegisterInterface<Weather>("Weather", 1);
    qmlRegisterInterface<TerrainFindingSystem>("TerrainFindingSystem", 1);
    qmlRegisterInterface<GameRecorder>("GameRecorder", 1);
    qmlRegisterInterface<GameScript>("GameScript", 1);
    qmlRegisterInterface<Campaign>("Campaign", 1);
    qmlRegisterInterface<BaseGameInputIF>("BaseGameInputIF", 1);
    qmlRegisterInterface<Settings>("Settings", 1);
    qmlRegisterInterface<Wikipage>("Wikipage", 1);
    qmlRegisterInterface<HeavyAi>("HeavyAi", 1);
    qmlRegisterInterface<NetworkGame>("NetworkGame", 1);
    /*************************************************************************************************/

    NeuralNetwork test;
    test.addLayer({{Layer::LAYER_PARAMETER_TYPE, static_cast<double>(Layer::LayerType::INPUT)},
                   {Layer::LAYER_PARAMETER_SIZE, static_cast<double>(2)},});
    for (qint32 i = 0; i < 5; ++i)
    {
        test.addLayer({{Layer::LAYER_PARAMETER_TYPE, static_cast<double>(Layer::LayerType::STANDARD)},
                       {Layer::LAYER_PARAMETER_SIZE, static_cast<double>(6)},
                       {Layer::LAYER_PARAMETER_ACTIVATION, static_cast<double>(Neuron::ActivationFunction::SIGMOID)},});
    }
    test.addLayer({{Layer::LAYER_PARAMETER_TYPE, static_cast<double>(Layer::LayerType::OUTPUT)},
                   {Layer::LAYER_PARAMETER_SIZE, static_cast<double>(1)},
                   {Layer::LAYER_PARAMETER_ACTIVATION, static_cast<double>(Neuron::ActivationFunction::SIGMOID)},});
    test.autogenerate();
    QFile file("dummy.nn");
    QDataStream stream(&file);
    file.open(QIODevice::WriteOnly);
    test.serializeObject(stream);
    file.close();
    NeuralNetwork test2;
    file.open(QIODevice::ReadOnly);
    test2.deserializeObject(stream);
    QString testText = test.toString();
    QString testText2 = test2.toString();
    bool equal = testText2 == testText;

    // show window according to window mode
    window.changeScreenMode(window.getScreenMode());
    window.setBrightness(Settings::getBrightness());
    window.setGamma(Settings::getGamma());
    window.setPosition(Settings::getX(), Settings::getY());
    qint32 returncode = app.exec();
    Settings::setX(window.x());
    Settings::setY(window.y());
    crashReporter::setSignalHandler(nullptr);
    window.getWorkerthread()->exit(0);
    /*************************************************************************************************/
    Settings::saveSettings();

    if (GameMap::getInstance() != nullptr)
    {
        GameMap::getInstance()->deleteMap();
    }
    // store current settings when closing
    Userdata::getInstance()->storeUser();
    if (MainServer::getInstance() != nullptr)
    {
        MainServer::getInstance()->deleteLater();
        window.getGameServerThread()->quit();
        window.getGameServerThread()->wait();
    }
    dir.removeRecursively();
    //end
    if (returncode == 1)
    {
        QProcess::startDetached(QCoreApplication::applicationFilePath(), QStringList());
    }
    return returncode;
}
