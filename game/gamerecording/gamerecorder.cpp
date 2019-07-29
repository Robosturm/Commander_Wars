#include "gamerecorder.h"

#include "game/gamemap.h"

#include "game/player.h"

#include "game/co.h"

#include "coreengine/mainapp.h"

GameRecorder::GameRecorder()
    : QObject()
{
    moveToThread(Mainapp::getInstance()->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

void GameRecorder::serializeObject(QDataStream& pStream)
{
    pStream << getVersion();
    pStream << static_cast<qint32>(m_Record.size());
    for (qint32 i = 0; i < m_Record.size(); i++)
    {
        m_Record[i]->serializeObject(pStream);
    }
    pStream << static_cast<qint32>(destroyedUnits.size());
    for (qint32 i = 0; i < destroyedUnits.size(); i++)
    {
        pStream << destroyedUnits[i];
        pStream << lostUnits[i];
        pStream << damageDealt[i];
        pStream << attackNumbers[i];
        pStream << deployedUnits[i];
    }
    pStream << m_mapTime;
    pStream << m_deployLimit;
}

void GameRecorder::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    qint32 size = 0;
    pStream >> size;
    for (qint32 i = 0; i < size; i++)
    {
        m_Record.append(new DayToDayRecord());
        m_Record[i]->deserializeObject(pStream);
    }
    pStream >> size;
    for (qint32 i = 0; i < size; i++)
    {
        if (version > 1)
        {
            quint32 value = 0;
            pStream >> value;
            destroyedUnits.append(value);
            pStream >> value;
            lostUnits.append(value);
            pStream >> value;
            damageDealt.append(value);
            pStream >> value;
            attackNumbers.append(value);
            pStream >> value;
            deployedUnits.append(value);
        }
        else
        {
            quint64 value = 0;
            pStream >> value;
            destroyedUnits.append(static_cast<quint32>(value));
            pStream >> value;
            lostUnits.append(static_cast<quint32>(value));
            pStream >> value;
            damageDealt.append(static_cast<quint32>(value));
            pStream >> value;
            attackNumbers.append(static_cast<quint32>(value));
            pStream >> value;
            deployedUnits.append(static_cast<quint32>(value));
        }
    }
    pStream >> m_mapTime;
    pStream >> m_deployLimit;
}

void GameRecorder::newDay()
{
    qint32 playerCount = GameMap::getInstance()->getPlayerCount();
    if (destroyedUnits.size() == 0)
    {
        for (qint32 i = 0; i < playerCount; i++)
        {
            destroyedUnits.append(0);
            lostUnits.append(0);
            damageDealt.append(0);
            attackNumbers.append(0);
            deployedUnits.append(0);
        }
    }
    m_Record.append(new DayToDayRecord(playerCount));
}

void GameRecorder::lostUnit(qint32 player)
{
    if (player >= 0 && player < lostUnits.size())
    {
        lostUnits[player]++;
    }
}

void GameRecorder::destroyedUnit(qint32 player)
{
    if (player >= 0 && player < destroyedUnits.size())
    {
        destroyedUnits[player]++;
    }
}

void GameRecorder::buildUnit(qint32 player)
{
    if (player >= 0 && player < deployedUnits.size())
    {
        deployedUnits[player]++;
    }
}

void GameRecorder::attacked(qint32 player, float damage)
{
    if (player >= 0 && player < attackNumbers.size())
    {
        attackNumbers[player]++;
        damageDealt[player] += damage;
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
    m_Record[m_Record.size() - 1]->addPlayerRecord(player, GameMap::getInstance()->getCurrentDay());
}

void GameRecorder::addSpecialEvent(qint32 player, GameEnums::GameRecord_SpecialEvents event)
{
    m_Record[m_Record.size() - 1]->addSpecialEvent(player, GameMap::getInstance()->getCurrentDay(), event);
}

GameRecorder::Rang GameRecorder::calculateRang(qint32 player, QVector3D& scorePoints)
{
    GameMap* pMap = GameMap::getInstance();
    qint32 winnerTeam = pMap->getWinnerTeam();
    qint32 score = 0;
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
    qint32 power = (damageDealt[player] + (destroyedUnits[player] * 140));
    if (attackNumbers[player] > 0)
    {
       power /= attackNumbers[player];
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
    if (lostUnits[player] > 0)
    {
        techScore1 = (destroyedUnits[player] / static_cast<float>(lostUnits[player])) * 0.75f;
    }
    else
    {
        techScore1 = 2.0f;
    }
    if (techScore1 > 2.0f)
    {
        techScore1 = 2.0f;
    }
    quint32 deployed = deployedUnits[player];
    quint32 startUnits = static_cast<quint32>(m_Record[0]->getPlayerRecord(player)->getUnits());
    if (m_Record.size() > 0 &&
        startUnits > 0)
    {
        deployed += startUnits;
    }
    if (deployed > 0)
    {
        techScore2 = (1.0f - (lostUnits[player] / static_cast<float>(deployed))) * 2.0f;
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
    if (deployedUnits[player] > 0)
    {
        techScore3 = deployLimit / static_cast<float>(deployedUnits[player]);
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
    score = scorePoints.x() + scorePoints.y() + scorePoints.z();
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
