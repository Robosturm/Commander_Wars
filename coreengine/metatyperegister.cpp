#include <QEasingCurve>

#include "coreengine/metatyperegister.h"

#include "3rd_party/oxygine-framework/oxygine/KeyEvent.h"

#include "coreengine/audiothread.h"
#include "coreengine/scriptvariable.h"
#include "coreengine/scriptvariables.h"
#include "coreengine/scriptvariablefile.h"

#include "ai/heavyai.h"
#include "ai/normalai.h"
#include "ai/influencefrontmap.h"

#include "menue/basegamemenu.h"
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
#include "game/ui/movementplanneraddin.h"

#include "game/jsData/terrainflowdata.h"
#include "game/jsData/campaignmapdata.h"

#include "wiki/wikidatabase.h"
#include "wiki/wikiview.h"

#include "objects/base/textbox.h"
#include "objects/playerselection.h"
#include "objects/dialogs/mapSelection/mapselectionfilterdialog.h"
#include "objects/dialogs/editor/mapeditdialog.h"
#include "objects/dialogs/gamepadinfo.h"
#include "objects/dialogs/customdialog.h"

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
    qRegisterMetaType<QEasingCurve::Type>("QEasingCurve::Type");
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
}
