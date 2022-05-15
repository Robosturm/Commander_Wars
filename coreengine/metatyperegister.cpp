#include "coreengine/metatyperegister.h"

#include "3rd_party/oxygine-framework/oxygine/KeyEvent.h"

#include "coreengine/audiothread.h"
#include "coreengine/scriptvariable.h"
#include "coreengine/scriptvariables.h"
#include "coreengine/scriptvariablefile.h"

#include "ai/heavyai.h"
#include "ai/normalai.h"

#include "menue/ingamemenue.h"
#include "menue/victorymenue.h"
#include "menue/mainwindow.h"
#include "menue/creditsmenue.h"
#include "menue/gamemenue.h"
#include "menue/mapselectionmapsmenue.h"
#include "menue/campaignmenu.h"
#include "menue/editormenue.h"
#include "menue/achievementmenu.h"
#include "menue/optionmenue.h"
#include "menue/basemenu.h"
#include "menue/replaymenu.h"
#include "menue/movementplanner.h"

#include "ui_reader/uifactory.h"
#include "ui_reader/createdgui.h"

#include "game/terrain.h"
#include "game/player.h"
#include "game/building.h"
#include "game/unit.h"
#include "game/co.h"
#include "game/gameaction.h"
#include "game/gameanimation/gameanimation.h"
#include "game/gameanimation/gameanimationwalk.h"
#include "game/gameanimation/gameanimationcapture.h"
#include "game/gameanimation/gameanimationdialog.h"
#include "game/gameanimation/gameanimationpower.h"
#include "game/gameanimation/gameanimationnextday.h"
#include "game/victoryrule.h"
#include "game/gamerules.h"
#include "game/gamerule.h"
#include "game/weather.h"
#include "game/terrainfindingsystem.h"
#include "game/campaign.h"
#include "game/gamescript.h"
#include "game/GameEnums.h"
#include "game/gamerecording/gamerecorder.h"
#include "game/gamerecording/playerrecord.h"
#include "game/gamerecording/specialevent.h"
#include "game/gamerecording/daytodayrecord.h"

#include "game/ui/customcoboostinfo.h"
#include "game/jsData/terrainflowdata.h"
#include "game/jsData/campaignmapdata.h"

#include "wiki/wikidatabase.h"
#include "wiki/wikiview.h"

#include "objects/base/textbox.h"
#include "objects/playerselection.h"
#include "objects/dialogs/mapSelection/mapselectionfilterdialog.h"
#include "objects/dialogs/editor/mapeditdialog.h"
#include "objects/dialogs/gamepadinfo.h"

#include "gameinput/cursordata.h"
#include "gameinput/basegameinputif.h"

#include "ingamescriptsupport/events/scriptevent.h"
#include "ingamescriptsupport/conditions/scriptcondition.h"

#include "network/mainserver.h"

#include "resource_management/cospritemanager.h"
#include "resource_management/terrainmanager.h"
#include "resource_management/buildingspritemanager.h"
#include "resource_management/coperkmanager.h"
#include "resource_management/unitspritemanager.h"

void MetaTypeRegister::registerInterfaceData()
{
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
    qRegisterMetaType<GameEnums::BattleAnimationType>("GameEnums::BattleAnimationType");
    qRegisterMetaType<GameEnums::BuildingTarget>("GameEnums::BuildingTarget");
    qRegisterMetaType<GameEnums::BattleAnimationMode>("GameEnums::BattleAnimationMode");
    qRegisterMetaType<GameEnums::UnitType>("GameEnums::UnitType");
    qRegisterMetaType<GameEnums::VisionType>("GameEnums::VisionType");
    qRegisterMetaType<GameEnums::AiTypes>("GameEnums::AiTypes");
    qRegisterMetaType<GameEnums::Recoloring>("GameEnums::Recoloring");
    qRegisterMetaType<GameEnums::COInfoPosition>("GameEnums::COInfoPosition");
    qRegisterMetaType<GameEnums::WeaponType>("GameEnums::WeaponType");
    qRegisterMetaType<GameEnums::ShopItemType>("GameEnums::ShopItemType");
    qRegisterMetaType<GameEnums::PowerGainZone>("GameEnums::PowerGainZone");
    qRegisterMetaType<GameEnums::PowerGainMode>("GameEnums::PowerGainMode");
    qRegisterMetaType<Console::eLogLevels>("Console::eLogLevels");
    qRegisterMetaType<Mainapp::StartupPhase>("Mainapp::StartupPhase");
    qRegisterMetaType<std::shared_ptr<QTcpSocket>>("std::shared_ptr<QTcpSocket>");
    qRegisterMetaType<spScriptEvent>("spScriptEvent");
    qRegisterMetaType<spScriptCondition>("spScriptCondition");
    qRegisterMetaType<WikiDatabase::PageData>("WikiDatabase::PageData");
    qRegisterMetaType<oxygine::spActor>("oxygine::spActor");
    qRegisterMetaType<oxygine::spResAnim>("oxygine::spResAnimf");
    qRegisterMetaType<oxygine::KeyEvent>("oxygine::KeyEvent");
    qRegisterMetaType<oxygine::MouseButton>("oxygine::MouseButton");
    qRegisterMetaType<QVector<std::tuple<QString,float>>>("QVector<std::tuple<QString,float>>");
    qRegisterMetaType<QVector<float>>("QVector<float>");
    qRegisterMetaType<QList<qint32>>("QList<qint32>");
    qRegisterMetaType<QLocalSocket::LocalSocketError>("QLocalSocket::LocalSocketError");
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
    qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");
    qRegisterMetaType<spTextbox>("spTextbox");
    qRegisterMetaType<spGameAction>("spGameAction");
    qRegisterMetaType<std::shared_ptr<QEvent>>("std::shared_ptr<QEvent>");
    qRegisterMetaType<MapEditDialog::MapEditInfo>("MapEditDialog::MapEditInfo");
    // interface

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
    qmlRegisterInterface<ScriptVariableFile>("ScriptVariableFile", 1);
    qmlRegisterInterface<Weather>("Weather", 1);
    qmlRegisterInterface<TerrainFindingSystem>("TerrainFindingSystem", 1);
    qmlRegisterInterface<GameRecorder>("GameRecorder", 1);
    qmlRegisterInterface<GameScript>("GameScript", 1);
    qmlRegisterInterface<Campaign>("Campaign", 1);
    qmlRegisterInterface<BaseGameInputIF>("BaseGameInputIF", 1);
    qmlRegisterInterface<Settings>("Settings", 1);
    qmlRegisterInterface<Wikipage>("Wikipage", 1);
    qmlRegisterInterface<HeavyAi>("HeavyAi", 1);
    qmlRegisterInterface<NormalAi>("NormalAi", 1);
    qmlRegisterInterface<NeuralNetwork>("NeuralNetwork", 1);
    qmlRegisterInterface<NetworkGame>("NetworkGame", 1);
    qmlRegisterInterface<PlayerSelection>("PlayerSelection", 1);
    qmlRegisterInterface<COSpriteManager>("COSpriteManager", 1);
    qmlRegisterInterface<UnitSpriteManager>("UnitSpriteManager", 1);
    qmlRegisterInterface<BuildingSpriteManager>("BuildingSpriteManager", 1);
    qmlRegisterInterface<TerrainManager>("TerrainManager", 1);
    qmlRegisterInterface<COPerkManager>("COPerkManager", 1);

    qmlRegisterInterface<CampaignMenu>("CampaignMenu", 1);
    qmlRegisterInterface<WikiView>("WikiView", 1);
    qmlRegisterInterface<Mainwindow>("Mainwindow", 1);
    qmlRegisterInterface<VictoryMenue>("VictoryMenue", 1);
    qmlRegisterInterface<CreditsMenue>("CreditsMenue", 1);
    qmlRegisterInterface<Achievementmenu>("Achievementmenu", 1);
    qmlRegisterInterface<OptionMenue>("OptionMenue", 1);
    qmlRegisterInterface<MapSelectionMapsMenue>("MapSelectionMapsMenue", 1);
    qmlRegisterInterface<MapSelectionFilterDialog>("MapSelectionFilterDialog", 1);
    qmlRegisterInterface<MapEditDialog>("MapEditDialog", 1);
    qmlRegisterInterface<CustomCoBoostInfo>("CustomCoBoostInfo", 1);
    qmlRegisterInterface<TerrainFlowData>("TerrainFlowData", 1);
    qmlRegisterInterface<CampaignMapData>("CampaignMapData", 1);
    qmlRegisterInterface<GamepadInfo>("GamepadInfo", 1);
    qmlRegisterInterface<CreatedGui>("CreatedGui", 1);
    qmlRegisterInterface<UiFactory>("UiFactory", 1);
    qmlRegisterInterface<GameRecorder>("GameRecorder", 1);
    qmlRegisterInterface<PlayerRecord>("PlayerRecord", 1);
    qmlRegisterInterface<SpecialEvent>("SpecialEvent", 1);
    qmlRegisterInterface<DayToDayRecord>("DayToDayRecord", 1);

    qmlRegisterInterface<GameMenue>("GameMenue", 1);
    qmlRegisterInterface<EditorMenue>("EditorMenue", 1);
    qmlRegisterInterface<InGameMenue>("InGameMenue", 1);
    qmlRegisterInterface<Basemenu>("Basemenu", 1);
}
