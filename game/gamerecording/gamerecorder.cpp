#include "game/gamerecording/gamerecorder.h"

#include "game/gamemap.h"
#include "game/player.h"
#include "game/co.h"

#include "coreengine/mainapp.h"

#include "resource_management/gamemanager.h"

#include "coreengine/filesupport.h"

GameRecorder::GameRecorder()
    : QObject()
{
    setObjectName("GameRecorder");
    moveToThread(Mainapp::getInstance()->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

void GameRecorder::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << static_cast<qint32>(m_Record.size());
    for (qint32 i = 0; i < m_Record.size(); i++)
    {
        m_Record[i]->serializeObject(pStream);
    }
    pStream << static_cast<qint32>(m_destroyedUnits.size());
    for (qint32 i = 0; i < m_destroyedUnits.size(); i++)
    {
        pStream << m_destroyedUnits[i];
        pStream << m_lostUnits[i];
        pStream << m_damageDealt[i];
        pStream << m_attackNumbers[i];
        pStream << m_deployedUnits[i];
    }
    pStream << m_mapTime;
    pStream << m_deployLimit;

    pStream << static_cast<qint32>(m_Attackreports.size());
    for (qint32 i = 0; i < m_Attackreports.size(); i++)
    {
        pStream << m_Attackreports[i]->day;
        pStream << m_Attackreports[i]->attackerKilled;
        pStream << m_Attackreports[i]->attackerDamage;
        pStream << m_Attackreports[i]->attackerX;
        pStream << m_Attackreports[i]->attackerY;
        pStream << m_Attackreports[i]->attackerID;
        pStream << m_Attackreports[i]->attackerOwnerID;
        pStream << m_Attackreports[i]->defenderX;
        pStream << m_Attackreports[i]->defenderY;
        pStream << m_Attackreports[i]->defenderDamage;
        pStream << m_Attackreports[i]->defenderID;
        pStream << m_Attackreports[i]->defenderOwnerID;
        pStream << m_Attackreports[i]->defenderKilled;
        pStream << m_Attackreports[i]->defenderSeesAttacker;
    }

    pStream << static_cast<qint32>(m_playerDataRecords.size());
    for (qint32 i = 0; i < m_playerDataRecords.size(); i++)
    {
        Filesupport::writeMap(pStream, m_playerDataRecords[i].killedUnits);
        Filesupport::writeMap(pStream, m_playerDataRecords[i].lostUnits);
        Filesupport::writeMap(pStream, m_playerDataRecords[i].producedUnits);
    }
}

void GameRecorder::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    qint32 size = 0;
    pStream >> size;
    m_Record.clear();
    for (qint32 i = 0; i < size; i++)
    {
        m_Record.append(spDayToDayRecord::create());
        m_Record[i]->deserializeObject(pStream);
    }
    pStream >> size;
    m_destroyedUnits.clear();
    m_lostUnits.clear();
    m_damageDealt.clear();
    m_attackNumbers.clear();
    m_deployedUnits.clear();
    for (qint32 i = 0; i < size; i++)
    {
        if (version > 1)
        {
            quint32 value = 0;
            pStream >> value;
            m_destroyedUnits.append(value);
            pStream >> value;
            m_lostUnits.append(value);
            pStream >> value;
            m_damageDealt.append(value);
            pStream >> value;
            m_attackNumbers.append(value);
            pStream >> value;
            m_deployedUnits.append(value);
        }
        else
        {
            quint64 value = 0;
            pStream >> value;
            m_destroyedUnits.append(static_cast<quint32>(value));
            pStream >> value;
            m_lostUnits.append(static_cast<quint32>(value));
            pStream >> value;
            m_damageDealt.append(static_cast<quint32>(value));
            pStream >> value;
            m_attackNumbers.append(static_cast<quint32>(value));
            pStream >> value;
            m_deployedUnits.append(static_cast<quint32>(value));
        }
    }
    if (version > 2)
    {
        pStream >> m_mapTime;
        pStream >> m_deployLimit;
    }
    m_Attackreports.clear();
    if (version > 3)
    {
        qint32 size = 0;
        pStream >> size;
        for (qint32 i = 0; i < size; i++)
        {
            m_Attackreports.append(spAttackReport::create());
            pStream >> m_Attackreports[i]->day;
            pStream >> m_Attackreports[i]->attackerKilled;
            pStream >> m_Attackreports[i]->attackerDamage;
            pStream >> m_Attackreports[i]->attackerX;
            pStream >> m_Attackreports[i]->attackerY;
            pStream >> m_Attackreports[i]->attackerID;
            pStream >> m_Attackreports[i]->attackerOwnerID;
            pStream >> m_Attackreports[i]->defenderX;
            pStream >> m_Attackreports[i]->defenderY;
            pStream >> m_Attackreports[i]->defenderDamage;
            pStream >> m_Attackreports[i]->defenderID;
            pStream >> m_Attackreports[i]->defenderOwnerID;
            pStream >> m_Attackreports[i]->defenderKilled;
            if (version > 4)
            {
                pStream >> m_Attackreports[i]->defenderSeesAttacker;
            }
            else
            {
                m_Attackreports[i]->defenderSeesAttacker = true;
            }
        }
    }
    if (version > 5)
    {
        pStream >> size;
        for (qint32 i = 0; i < size; i++)
        {
            PlayerData data;
            data.killedUnits = Filesupport::readMap<QString, qint32, QMap>(pStream);
            data.lostUnits = Filesupport::readMap<QString, qint32, QMap>(pStream);
            data.producedUnits = Filesupport::readMap<QString, qint32, QMap>(pStream);
            m_playerDataRecords.append(data);
        }
    }
}

void GameRecorder::newDay()
{
    qint32 playerCount = GameMap::getInstance()->getPlayerCount();
    if (m_destroyedUnits.size() == 0)
    {
        for (qint32 i = 0; i < playerCount; i++)
        {
            m_destroyedUnits.append(0);
            m_lostUnits.append(0);
            m_damageDealt.append(0);
            m_attackNumbers.append(0);
            m_deployedUnits.append(0);
            m_playerDataRecords.append(PlayerData());
        }
    }
    m_Record.append(spDayToDayRecord::create(playerCount));
}

void GameRecorder::lostUnit(qint32 player, QString unitId)
{
    if (player >= 0 && player < m_lostUnits.size())
    {
        m_lostUnits[player]++;
        if (!unitId.isEmpty())
        {
            qint32 value = m_playerDataRecords[player].lostUnits.value(unitId);
            value++;
            m_playerDataRecords[player].lostUnits.insert(unitId, value);
        }
    }
}

quint32 GameRecorder::getLostUnits(qint32 player)
{
    if (player >= 0 && player < m_lostUnits.size())
    {
        return m_lostUnits[player];
    }
    return 0;
}

void GameRecorder::destroyedUnit(qint32 player, QString unitId)
{
    if (player >= 0 && player < m_destroyedUnits.size())
    {
        m_destroyedUnits[player]++;
        if (!unitId.isEmpty())
        {
            qint32 value = m_playerDataRecords[player].killedUnits.value(unitId);
            value++;
            m_playerDataRecords[player].killedUnits.insert(unitId, value);
        }
    }
}

quint32 GameRecorder::getDestroyedUnits(qint32 player)
{
    if (player >= 0 && player < m_destroyedUnits.size())
    {
        return m_destroyedUnits[player];
    }
    return 0;
}

void GameRecorder::buildUnit(qint32 player, QString unitId)
{
    if (player >= 0 && player < m_deployedUnits.size())
    {
        m_deployedUnits[player]++;
        if (!unitId.isEmpty())
        {
            qint32 value = m_playerDataRecords[player].producedUnits.value(unitId);
            value++;
            m_playerDataRecords[player].producedUnits.insert(unitId, value);
        }
    }
}

quint32 GameRecorder::getBuildedUnits(qint32 player)
{
    if (player >= 0 && player < m_deployedUnits.size())
    {
        return m_deployedUnits[player];
    }
    return 0;
}

void GameRecorder::attacked(qint32 player, float damage)
{
    if (player >= 0 && player < m_attackNumbers.size())
    {
        m_attackNumbers[player]++;
        m_damageDealt[player] += damage;
    }
}

quint32 GameRecorder::getDeployLimit() const
{
    return m_deployLimit;
}

void GameRecorder::setDeployLimit(const quint32 &deployLimit)
{
    m_deployLimit = deployLimit;
}

qint32 GameRecorder::getMapTime() const
{
    return m_mapTime;
}

void GameRecorder::setMapTime(const qint32 &mapTime)
{
    m_mapTime = mapTime;
}

void GameRecorder::updatePlayerData(qint32 player)
{
    spGameMap pMap = GameMap::getInstance();
    if (m_Record.size() > 0 && pMap.get() != nullptr)
    {
        m_Record[m_Record.size() - 1]->addPlayerRecord(player, pMap->getCurrentDay());
    }
}

void GameRecorder::addSpecialEvent(qint32 player, GameEnums::GameRecord_SpecialEvents event)
{
    spGameMap pMap = GameMap::getInstance();
    if (m_Record.size() > 0 && pMap.get() != nullptr)
    {
        m_Record[m_Record.size() - 1]->addSpecialEvent(player, pMap->getCurrentDay(), event);
    }
}

void GameRecorder::logAttack(qint32 day,
                             qint32 attackerDamage, qint32 attackerX, qint32 attackerY, QString attackerID, qint32 attackerOwnerID, bool attackerKilled,
                             qint32 defenderDamage, qint32 defenderX, qint32 defenderY, QString defenderID, qint32 defenderOwnerID, bool defenderKilled, bool defenderSeesAttacker)
{
    m_Attackreports.append(spAttackReport::create());
    m_Attackreports[m_Attackreports.size() - 1]->day = day;
    m_Attackreports[m_Attackreports.size() - 1]->attackerKilled = attackerKilled;
    m_Attackreports[m_Attackreports.size() - 1]->attackerDamage = attackerDamage;
    m_Attackreports[m_Attackreports.size() - 1]->attackerX = attackerX;
    m_Attackreports[m_Attackreports.size() - 1]->attackerY = attackerY;
    m_Attackreports[m_Attackreports.size() - 1]->attackerID = attackerID;
    m_Attackreports[m_Attackreports.size() - 1]->attackerOwnerID = attackerOwnerID;
    m_Attackreports[m_Attackreports.size() - 1]->defenderX = defenderX;
    m_Attackreports[m_Attackreports.size() - 1]->defenderY = defenderY;
    m_Attackreports[m_Attackreports.size() - 1]->defenderDamage = defenderDamage;
    m_Attackreports[m_Attackreports.size() - 1]->defenderID = defenderID;
    m_Attackreports[m_Attackreports.size() - 1]->defenderOwnerID = defenderOwnerID;
    m_Attackreports[m_Attackreports.size() - 1]->defenderKilled = defenderKilled;
    m_Attackreports[m_Attackreports.size() - 1]->defenderSeesAttacker = defenderSeesAttacker;
}

QVector<spAttackReport> GameRecorder::getAttackLog(qint32 player)
{
    QVector<spAttackReport> ret;
    for (qint32 i = 0; i < m_Attackreports.size(); i++)
    {
        if ((m_Attackreports[i]->attackerOwnerID == player) ||
            (m_Attackreports[i]->defenderOwnerID == player))
        {
            ret.append(m_Attackreports[i].get());
        }
    }
    return ret;
}

const QVector<GameRecorder::PlayerData> & GameRecorder::getPlayerDataRecords() const
{
    return m_playerDataRecords;
}

GameRecorder::Rang GameRecorder::calculateRang(qint32 player, QVector3D& scorePoints)
{
    spGameMap pMap = GameMap::getInstance();
    qint32 score = 0;
    if (pMap.get() != nullptr)
    {
        qint32 winnerTeam = pMap->getWinnerTeam();
        qint32 mapSize = pMap->getMapWidth() * pMap->getMapHeight();
        // calc speed points
        qint32 mapTime = (pMap->getMapWidth() + pMap->getMapHeight());
        if (m_mapTime > 0)
        {
            mapTime = m_mapTime;
        }
        if (pMap->getCurrentDay() < mapTime)
        {
            scorePoints.setX(200 - (pMap->getCurrentDay() * 100 / mapTime));
        }
        else
        {
            scorePoints.setX(100 - ((pMap->getCurrentDay() - mapTime) * 100 / (3 * mapTime)));
        }
        if (scorePoints.x() < 0)
        {
            scorePoints.setX(0);
        }
        Player* pPlayer = pMap->getPlayer(player);
        if (pPlayer->getTeam() != winnerTeam && winnerTeam >= 0)
        {
            qint32 lostDay = 0;
            for (qint32 i = 0; i < m_Record.size(); i++)
            {
                if (i == m_Record.size() - 1)
                {
                    lostDay = pMap->getCurrentDay();
                }
                else if (m_Record[i]->getPlayerRecord(player)->getUnits() == -1)
                {
                    lostDay = i - 1;
                    break;
                }
            }
            scorePoints.setX(0.8f * ((scorePoints.x() * lostDay) / pMap->getCurrentDay()));
        }
        // Force
        qint32 power = (m_damageDealt[player] + (m_destroyedUnits[player] * 140));
        if (m_attackNumbers[player] > 0)
        {
            power /= m_attackNumbers[player];
        }
        else
        {
            power = 0;
        }
        if (power >= 100)
        {
            scorePoints.setY(scorePoints.y() + power);
        }
        else if (power > 50)
        {
            scorePoints.setY(scorePoints.y() + (2 * power) - 100);
        }
        else
        {
            scorePoints.setY(scorePoints.y() + power);
        }
        if (scorePoints.y() < 0)
        {
            scorePoints.setY(0);
        }

        // technique
        quint32 deployLimit = static_cast<quint32>(mapSize / 9);
        if (m_deployLimit > 0)
        {
            deployLimit = m_deployLimit;
        }
        float techScore1 = 0;
        float techScore2 = 0;
        float techScore3 = 0;
        if (m_lostUnits[player] > 0)
        {
            techScore1 = (m_destroyedUnits[player] / static_cast<float>(m_lostUnits[player])) * 0.75f;
        }
        else
        {
            techScore1 = 2.0f;
        }
        if (techScore1 > 2.0f)
        {
            techScore1 = 2.0f;
        }
        quint32 deployed = m_deployedUnits[player];
        quint32 startUnits = static_cast<quint32>(m_Record[0]->getPlayerRecord(player)->getUnits());
        if (m_Record.size() > 0 &&
            startUnits > 0)
        {
            deployed += startUnits;
        }
        if (deployed > 0)
        {
            techScore2 = (1.0f - (m_lostUnits[player] / static_cast<float>(deployed))) * 2.0f;
        }
        else
        {
            techScore2 = 2.0f;
        }
        if (techScore2 > 2.0f)
        {
            techScore2 = 2.0f;
        }
        else if (techScore2 < 0)
        {
            techScore2 = 0;
        }
        if (m_deployedUnits[player] > 0)
        {
            techScore3 = deployLimit / static_cast<float>(m_deployedUnits[player]);
        }
        else
        {
            techScore3 = 2.0f;
        }
        if (techScore3 > 2.0f)
        {
            techScore3 = 2.0f;
        }

        scorePoints.setZ((techScore1 + techScore2 + techScore3)  * 100 / 3);
        if (scorePoints.z() < 0)
        {
            scorePoints.setZ(0);
        }
        // cap score points
        if (scorePoints.x() > 150)
        {
            scorePoints.setX(150);
        }
        if (scorePoints.y() > 150)
        {
            scorePoints.setY(150);
        }
        if (scorePoints.z() > 150)
        {
            scorePoints.setZ(150);
        }
        score = scorePoints.x() + scorePoints.y() + scorePoints.z();
    }
    return getRank(score);
}


GameRecorder::Rang GameRecorder::getRank(qint32 score)
{
    if (score >= 330)
    {
        return GameRecorder::Rang::S;
    }
    else if (score >= 280)
    {
        return GameRecorder::Rang::A;
    }
    else if (score >= 250)
    {
        return GameRecorder::Rang::B;
    }
    else if (score >= 200)
    {
        return GameRecorder::Rang::C;
    }
    else
    {
        return GameRecorder::Rang::D;
    }
}

oxygine::ResAnim* GameRecorder::getRankAnim(GameRecorder::Rang rang)
{
    oxygine::ResAnim* pAnim = nullptr;
    switch (rang)
    {
        case GameRecorder::Rang::S:
        {
            pAnim = GameManager::getInstance()->getResAnim("s_rang");
            break;
        }
        case GameRecorder::Rang::A:
        {
            pAnim = GameManager::getInstance()->getResAnim("a_rang");
            break;
        }
        case GameRecorder::Rang::B:
        {
            pAnim = GameManager::getInstance()->getResAnim("b_rang");
            break;
        }
        case GameRecorder::Rang::C:
        {
            pAnim = GameManager::getInstance()->getResAnim("c_rang");
            break;
        }
        case GameRecorder::Rang::D:
        {
            pAnim = GameManager::getInstance()->getResAnim("d_rang");
            break;
        }
    }
    return pAnim;
}
