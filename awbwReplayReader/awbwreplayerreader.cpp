#include "awbwReplayReader/awbwreplayerreader.h"
#include "zipSupport/qzipreader.h"

#include <QBuffer>
#include <QJsonDocument>

#include "awbwReplayReader/awbwdataparser.h"

static const char* const JSON_TURNSTART = "O:8:\"awbwGame\":";
static const char* const JSON_PLAYERSTART = "O:10:\"awbwPlayer\":";
static const char* const JSON_BUILDINGSTART = "O:12:\"awbwBuilding\":";
static const char* const JSON_UNITSSTART = "O:8:\"awbwUnit\":";
static const char* const JSON_ACTIONSTART = "a:a:3:{";
static const char* const JSON_REPLAYSTART = "p:";

void AwbwReplayerReader::parseReplay(const QString & awbwReplayZip)
{
    QZipReader reader(awbwReplayZip);
    auto files = reader.fileInfoList();
    for (auto file : files)
    {
        auto zipContent = reader.fileData(file.filePath);
        QBuffer contentBuffer(&zipContent);
        contentBuffer.open(QIODevice::ReadOnly);
        QZipReader unzipReader(&contentBuffer);
        auto unzipContent = unzipReader.unzipContent(true);
        if (unzipContent.startsWith(JSON_TURNSTART))
        {
            if (m_valid)
            {
                auto gameStates = unzipContent.split('\n');
                parseGameStates(gameStates);
            }
        }
        else if (unzipContent.startsWith(JSON_REPLAYSTART))
        {
            if (m_valid)
            {
                auto actions = unzipContent.split('\n');
                parseActions(actions);
            }
        }
    }
}

void AwbwReplayerReader::parseGameStates(QList<QByteArray> & data)
{
    for (auto & item : data)
    {
        if (!item.isEmpty())
        {
            GameState newState;
            QTextStream stream(&item);
            qint32 itemCount = 0;
            QString name = AwbwDataParser::readObjectStart(stream, m_valid, itemCount);
            if (m_valid &&
                name == "awbwGame")
            {
                for (qint32 i = 0; i < itemCount; ++i)
                {
                    QString item = AwbwDataParser::readString(stream, m_valid);
                    if (!m_valid)
                    {
                        return;
                    }
                    if (item == "players")
                    {
                        readPlayers(stream, newState);
                    }
                    else if (item == "buildings")
                    {
                        readBuildings(stream, newState);
                    }
                    else if (item == "units")
                    {
                        readUnits(stream, newState);
                    }
                    else if (item == "id")
                    {
                        AwbwDataParser::readLong(stream, m_valid);
                    }
                    else if (item == "name")
                    {
                        AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "password")
                    {
                        AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "creator")
                    {
                        AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "maps_id")
                    {
                        newState.mapId = AwbwDataParser::readLong(stream, m_valid);
                    }
                    else if (item == "funds")
                    {
                        newState.fundsPerBuilding = AwbwDataParser::readLong(stream, m_valid);
                    }
                    else if (item == "starting_funds")
                    {
                        newState.startingFunds = AwbwDataParser::readLong(stream, m_valid);
                    }
                    else if (item == "weather_type")
                    {
                        newState.weatherType = AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "fog")
                    {
                        newState.fog = AwbwDataParser::readBool(stream, m_valid);
                    }
                    else if (item == "use_powers")
                    {
                        newState.usePowers = AwbwDataParser::readBool(stream, m_valid);
                    }
                    else if (item == "official")
                    {
                        AwbwDataParser::readBool(stream, m_valid);
                    }
                    else if (item == "league")
                    {
                        AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "team")
                    {
                        AwbwDataParser::readBool(stream, m_valid);
                    }
                    else if (item == "turn")
                    {
                        newState.turn = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "day")
                    {
                        newState.day = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "start_date")
                    {
                        AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "activity_date")
                    {
                        AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "end_date")
                    {
                        AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "weather_code")
                    {
                        newState.currentWeather = AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "weather_start")
                    {
                        newState.weatherStart = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "win_condition")
                    {
                        AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "capture_win")
                    {
                        AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "active")
                    {
                        AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "comment")
                    {
                        AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "type")
                    {
                        AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "aet_date")
                    {
                        AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "aet_interval")
                    {
                        AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "boot_interval")
                    {
                        AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "max_rating")
                    {
                        AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "min_rating")
                    {
                        AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "timers_initial")
                    {
                        AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "timers_increment")
                    {
                        AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "timers_max_turn")
                    {
                        AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else
                    {
                        Q_ASSERT(false);
                    }
                    if (!m_valid)
                    {
                        return;
                    }
                }
                m_valid = AwbwDataParser::readArrayObjectEnd(stream);
            }
            else
            {
                return;
            }
            if (m_valid)
            {
                m_gameStates.append(newState);
            }
            else
            {
                return;
            }
        }
    }
}

void AwbwReplayerReader::readPlayers(QTextStream & stream, GameState & newState)
{
    qint32 items = AwbwDataParser::readArrayStart(stream, m_valid);
    for (qint32 i = 0; i < items; ++i)
    {
        PlayerInfo player;
        player.playerIdx = AwbwDataParser::readNullableInteger(stream, m_valid);
        if (m_valid)
        {
            qint32 playerItemCount = 0;
            QString name = AwbwDataParser::readObjectStart(stream, m_valid, playerItemCount);
            m_valid = m_valid && name == "awbwPlayer";
            if (m_valid)
            {
                for (qint32 i2 = 0; i2 < playerItemCount; ++i2)
                {
                    QString item = AwbwDataParser::readString(stream, m_valid);
                    if (!m_valid)
                    {
                        return;
                    }
                    if (item == "id")
                    {
                        player.playerId = AwbwDataParser::readLong(stream, m_valid);
                    }
                    else if (item == "users_id")
                    {
                        player.userId = AwbwDataParser::readLong(stream, m_valid);
                    }
                    else if (item == "team")
                    {
                        player.team = AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "countries_id")
                    {
                        player.countryId = AwbwDataParser::readLong(stream, m_valid);
                    }
                    else if (item == "co_id")
                    {
                        player.coData.coId = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "tags_co_id")
                    {
                        player.tagCoData.coId = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "co_max_power")
                    {
                        player.coData.maxPower = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "co_max_spower")
                    {
                        player.coData.maxSuperPower = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "tags_co_max_power")
                    {
                        player.tagCoData.maxPower = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "tags_co_max_spower")
                    {
                        player.tagCoData.maxSuperPower = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "funds")
                    {
                        player.funds = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "eliminated")
                    {
                        player.eliminated = AwbwDataParser::readBool(stream, m_valid);
                    }
                    else if (item == "co_power")
                    {
                        player.coData.power = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "tags_co_power")
                    {
                        player.tagCoData.power = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "order")
                    {
                        player.turnOrder = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "accept_draw")
                    {
                        player.drawAccepted = AwbwDataParser::readBool(stream, m_valid);
                    }
                    else if (item == "co_power_on")
                    {
                        player.powerOn = AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "co_image")
                    {
                        player.coImage = AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "email")
                    {
                        AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "emailpress")
                    {
                        AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "last_read")
                    {
                        AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "last_read_broadcasts")
                    {
                        AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "games_id")
                    {
                        AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "signature")
                    {
                        player.signature = AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "turn")
                    {
                        AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "turn_start")
                    {
                        AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "turn_clock")
                    {
                        AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "aet_count")
                    {
                        AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "uniq_id")
                    {
                        player.uniqueId = AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "interface")
                    {
                        AwbwDataParser::readString(stream, m_valid);
                    }
                    else
                    {
                        Q_ASSERT(false);
                    }
                    if (!m_valid)
                    {
                        return;
                    }
                }
                m_valid = m_valid && AwbwDataParser::readArrayObjectEnd(stream);
            }
            else
            {
                return;
            }
        }
        else
        {
            return;
        }
        newState.players.append(player);
    }
    m_valid = m_valid && AwbwDataParser::readArrayObjectEnd(stream);
}

void AwbwReplayerReader::readBuildings(QTextStream & stream, GameState & newState)
{
    qint32 items = AwbwDataParser::readArrayStart(stream, m_valid);
    for (qint32 i = 0; i < items; ++i)
    {
        BuildingInfo building;
        AwbwDataParser::readNullableInteger(stream, m_valid);
        if (m_valid)
        {
            qint32 buildingItemCount = 0;
            QString name = AwbwDataParser::readObjectStart(stream, m_valid, buildingItemCount);
            m_valid = m_valid && name == "awbwBuilding";
            if (m_valid)
            {
                for (qint32 i2 = 0; i2 < buildingItemCount; ++i2)
                {
                    QString item = AwbwDataParser::readString(stream, m_valid);
                    if (!m_valid)
                    {
                        return;
                    }
                    if (item == "id")
                    {
                        building.ownerId = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "terrain_id")
                    {
                        building.terrainId = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "x")
                    {
                        building.x = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "y")
                    {
                        building.y = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "capture")
                    {
                        building.capture = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "last_capture")
                    {
                        building.lastCapture = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "games_id")
                    {
                        AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "last_updated")
                    {
                        AwbwDataParser::readString(stream, m_valid);
                    }
                    else
                    {
                        Q_ASSERT(false);
                    }
                    if (!m_valid)
                    {
                        return;
                    }
                }
                m_valid = AwbwDataParser::readArrayObjectEnd(stream);
            }
            else
            {
                return;
            }
        }
        else
        {
            return;
        }
        newState.buildings.append(building);
    }
    m_valid = m_valid && AwbwDataParser::readArrayObjectEnd(stream);
}

void AwbwReplayerReader::readUnits(QTextStream & stream, GameState & newState)
{
    qint32 items = AwbwDataParser::readArrayStart(stream, m_valid);
    for (qint32 i = 0; i < items; ++i)
    {
        UnitInfo unit;
        AwbwDataParser::readNullableInteger(stream, m_valid);
        if (m_valid)
        {
            qint32 unitItemCount = 0;
            QString name = AwbwDataParser::readObjectStart(stream, m_valid, unitItemCount);
            m_valid = m_valid && name == "awbwUnit";
            if (m_valid)
            {
                for (qint32 i2 = 0; i2 < unitItemCount; ++i2)
                {
                    QString item = AwbwDataParser::readString(stream, m_valid);
                    if (!m_valid)
                    {
                        return;
                    }
                    if (item == "id")
                    {
                        unit.unitId = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "players_id")
                    {
                        unit.playerId = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "name")
                    {
                        unit.name = AwbwDataParser::readString(stream, m_valid).toLower();
                    }
                    else if (item == "movement_points")
                    {
                        unit.movementPoints = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "vision")
                    {
                        unit.vision = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "fuel")
                    {
                        unit.fuel = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "fuel_per_turn")
                    {
                        unit.fuelPerTurn = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "sub_dive")
                    {
                        unit.stealthed = AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "ammo")
                    {
                        unit.ammo = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "short_range")
                    {
                        unit.minRange = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "long_range")
                    {
                        unit.maxRange = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "second_weapon")
                    {
                        unit.hasSecondWeapon = AwbwDataParser::readBool(stream, m_valid);
                    }
                    else if (item == "cost")
                    {
                        unit.cost = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "movement_type")
                    {
                        unit.movementType = AwbwDataParser::readString(stream, m_valid);
                    }
                    else if (item == "x")
                    {
                        unit.x = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "y")
                    {
                        unit.y = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "moved")
                    {
                        unit.moved = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "capture")
                    {
                        unit.capture = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "fired")
                    {
                        unit.fired = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "hit_points")
                    {
                        unit.hp = AwbwDataParser::readFloat(stream, m_valid);
                    }
                    else if (item == "cargo1_units_id")
                    {
                        unit.loadedUnitId1 = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "cargo2_units_id")
                    {
                        unit.loadedUnitId2 = AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "carried")
                    {
                        unit.carried = AwbwDataParser::readBool(stream, m_valid);
                    }
                    else if (item == "games_id")
                    {
                        AwbwDataParser::readNullableInteger(stream, m_valid);
                    }
                    else if (item == "symbol")
                    {
                        AwbwDataParser::readString(stream, m_valid);
                    }
                    else
                    {
                        Q_ASSERT(false);
                    }
                    if (!m_valid)
                    {
                        return;
                    }
                }
                m_valid = AwbwDataParser::readArrayObjectEnd(stream);
            }
            else
            {
                return;
            }
        }
        else
        {
            return;
        }
        newState.units.append(unit);
    }
    m_valid = m_valid && AwbwDataParser::readArrayObjectEnd(stream);

}

void AwbwReplayerReader::parseActions(QList<QByteArray> & data)
{
    for (auto & item : data)
    {
        if (!item.isEmpty())
        {
            ActionInfo actionInfo;
            QTextStream stream(&item);
            actionInfo.playerId = AwbwDataParser::readNullableInteger(stream, m_valid, "p");
            actionInfo.day = AwbwDataParser::readNullableInteger(stream, m_valid, "d");
            m_valid = m_valid && stream.read(QString(JSON_ACTIONSTART).size()) == JSON_ACTIONSTART;
            AwbwDataParser::readNullableInteger(stream, m_valid);
            AwbwDataParser::readNullableInteger(stream, m_valid);
            AwbwDataParser::readNullableInteger(stream, m_valid);
            AwbwDataParser::readNullableInteger(stream, m_valid);
            AwbwDataParser::readNullableInteger(stream, m_valid);
            if (m_valid)
            {
                qint32 items = AwbwDataParser::readArrayStart(stream, m_valid);
                for (qint32 i = 0; i < items; ++i)
                {
                    AwbwDataParser::readNullableInteger(stream, m_valid);
                    actionInfo.actionData.append(QJsonDocument::fromJson(AwbwDataParser::readString(stream, m_valid).toUtf8()).object());
                    if (!m_valid)
                    {
                        return;
                    }
                }
                m_valid = AwbwDataParser::readArrayObjectEnd(stream);
            }
            else
            {
                return;
            }
            if (!m_valid)
            {
                return;
            }
            m_actions.append(actionInfo);
        }
    }
}

const QVector<AwbwReplayerReader::ActionInfo> &  AwbwReplayerReader::getActions() const
{
    return m_actions;
}

const QVector<AwbwReplayerReader::GameState> & AwbwReplayerReader::getGameStates() const
{
    return m_gameStates;
}

bool AwbwReplayerReader::getValid() const
{
    return m_valid;
}
