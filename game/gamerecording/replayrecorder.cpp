#include <QByteArray>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>

#include "network/JsonKeys.h"
#include "game/gamerecording/replayrecorder.h"

#include "game/gamemap.h"
#include "game/gameaction.h"
#include "objects/minimap.h"

#include "coreengine/settings.h"
#include "coreengine/interpreter.h"
#include "coreengine/filesupport.h"
#include "coreengine/gameconsole.h"
#include "coreengine/mainapp.h"

#include "gameinput/basegameinputif.h"

static const char* const RECORD_INFO_MARKER = "CoWRecord";

ReplayRecorder::ReplayRecorder(GameMap* pMap)
    : m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("ReplayRecorder");
#endif
    Interpreter::setCppOwnerShip(this);
    m_stream.setVersion(QDataStream::Version::Qt_6_5);
}

ReplayRecorder::~ReplayRecorder()
{
    CONSOLE_PRINT("Saving record with stream pos " + QString::number(m_recordFile.pos()), GameConsole::eDEBUG);
    if (m_recording)
    {
        m_recordFile.seek(m_countPos);
        m_stream << m_count;
        m_recordFile.flush();
    }
    m_recordFile.close();
}

void ReplayRecorder::requestReplayStart()
{
    emit startReplay();
}

void ReplayRecorder::startRecording(const QString & file)
{    
    if (Settings::getInstance()->getRecord() && m_pMap != nullptr && !Settings::getInstance()->getAiSlave())
    {
        QString fileName = file;
        if (fileName.isEmpty())
        {
            QString currentDate = QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss");
            fileName = "data/records/" + m_pMap->getMapName() + "-" + currentDate + ".rec";
        }

        CONSOLE_PRINT("Starting recording", GameConsole::eDEBUG);
        // compress script enviroment
        QByteArray data = Interpreter::getInstance()->getRuntimeData().toUtf8();
        data = qCompress(data);
        m_recordFile.setFileName(fileName);
        m_recordFile.open(QIODevice::WriteOnly);
        m_stream << VERSION;
        m_stream << QString(RECORD_INFO_MARKER);
        m_stream << createRecordJson();
        qint64 streamStartPos = m_recordFile.pos();
        m_stream << m_streamStart;
        m_stream << static_cast<qint32>(data.size());
        for (qint32 i = 0; i < data.size(); i++)
        {
            m_stream << static_cast<qint8>(data.at(i));
        }
        QStringList mods = Settings::getInstance()->getMods();
        Filesupport::writeVectorList(m_stream, mods);
        m_winnerTeamPos = m_recordFile.pos();
        m_stream << m_winnerTeam;
        m_countPos = m_recordFile.pos();
        m_stream << m_count;
        m_pMap->serializeObject(m_stream);
        // seek to start and write size
        m_streamStart = m_recordFile.pos();
        m_recordFile.seek(streamStartPos);
        m_stream << m_streamStart;
        m_recordFile.seek(m_streamStart);
        m_recordFile.flush();
        m_recording = true;
        m_currentDay = m_pMap->getCurrentDay();
        m_currentPlayer = m_pMap->getCurrentPlayer()->getPlayerID();
        m_pMap->setRecordFile(fileName);
    }
}

bool ReplayRecorder::continueRecording(const QString & file)
{
    bool success = false;
    m_recordFile.setFileName(file);
    if (m_recordFile.exists())
    {
        QByteArray envData;
        if (validRecord(envData))
        {
            QString interpreterEnvironment(envData);
            if (interpreterEnvironment == Interpreter::getInstance()->getRuntimeData())
            {
                m_recording = true;
                m_currentDay = m_pMap->getCurrentDay();
                m_recordFile.close();
                m_recordFile.open(QIODevice::WriteOnly | QIODevice::Append);
                m_recordFile.seek(m_recordFile.size());
                CONSOLE_PRINT("Continue record at stream pos " + QString::number(m_recordFile.pos()), GameConsole::eDEBUG);
                success = m_pMap->getReplayActionCount() == m_count;
            }
        }
    }
    return success;
}

void ReplayRecorder::recordAction(const spGameAction & action)
{
    
    if (m_recording && !action->getIsLocal() && m_pMap != nullptr)
    {
        qint32 curDay = m_pMap->getCurrentDay();
        qint32 curPlayer = m_pMap->getCurrentPlayer()->getPlayerID();
        bool started = (curPlayer >= 1 && curDay >= 1) || curDay > 1;
        if ((m_currentDay != curDay || m_currentPlayer != curPlayer)
            && started)
        {
            writeMapState();
            qint64 curPos = m_recordFile.pos();
            m_recordFile.seek(m_countPos);
            m_stream << m_count;
            m_recordFile.flush();
            m_recordFile.seek(curPos);
        }
        writeAction(action);
    }
}

void ReplayRecorder::writeAction(const spGameAction & action)
{
    qint64 nextSeekPos = 0;
    HeaderInfo info(Type::Action);
    m_stream << info.m_version;
    m_stream << static_cast<qint32>(info.m_type);
    qint64 curPos = m_recordFile.pos();
    m_stream << nextSeekPos;
    action->serializeObject(m_stream);
    // seek to start and write size
    nextSeekPos = m_recordFile.pos();
    m_recordFile.seek(curPos);
    m_stream << nextSeekPos;
    m_recordFile.seek(nextSeekPos);
    m_recordFile.flush();
    m_count++;
}

void ReplayRecorder::writeWinnerTeam()
{
    if (m_recording && m_pMap != nullptr)
    {
        if (m_pMap->getWinnerTeam() > 0)
        {
            auto curPos = m_recordFile.pos();
            m_recordFile.seek(m_winnerTeamPos);
            m_stream << m_pMap->getWinnerTeam();
            m_recordFile.seek(curPos);
        }
    }
}

void ReplayRecorder::writeMapState()
{
    m_currentDay = m_pMap->getCurrentDay();
    m_currentPlayer = m_pMap->getCurrentPlayer()->getPlayerID();
    qint64 nextSeekPos = 0;
    HeaderInfo info(Type::Map);
    m_stream << info.m_version;
    m_stream << static_cast<qint32>(info.m_type);
    qint64 curPos = m_recordFile.pos();
    m_stream << nextSeekPos;
    m_stream << m_pMap->getCurrentDay();
    m_stream << m_pMap->getCurrentPlayer()->getPlayerID();
    m_stream << m_count;
    m_pMap->serializeObject(m_stream);
    // seek to start and write size
    nextSeekPos = m_recordFile.pos();
    m_recordFile.seek(curPos);
    m_stream << nextSeekPos;
    m_recordFile.seek(nextSeekPos);
}

qint32 ReplayRecorder::getWinnerTeam()
{
    return m_winnerTeam;
}

bool ReplayRecorder::loadRecord(const QString & filename)
{
    m_recordFile.setFileName(filename);
    if (m_recordFile.exists())
    {
        QByteArray envData;
        if (validRecord(envData))
        {
            QString interpreterEnvironment(envData);
            if (Interpreter::reloadInterpreter(interpreterEnvironment))
            {
                // load map
                m_mapPos = m_recordFile.pos();
                m_pMap->clearMap();
                m_pMap->deserializeObject(m_stream);
                m_pMap->setIsHumanMatch(false);
                m_progress = 0;
                // swap out all ai's / or players with a proxy ai.
                for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
                {
                    m_pMap->getPlayer(i)->setBaseGameInput(BaseGameInputIF::createAi(m_pMap, GameEnums::AiTypes::AiTypes_ProxyAi));
                }
                m_playing = true;
            }
        }
    }
    return m_playing;
}

bool ReplayRecorder::validRecord(QByteArray & envData)
{
    m_recordFile.open(QIODevice::ReadOnly);
    bool success = readRecordInfo(m_stream, m_recordJson, m_version);
    if (success)
    {
        m_stream >> m_streamStart;
        qint32 size = 0;
        m_stream >> size;
        for (qint32 i = 0; i < size; i++)
        {
            qint8 value = 0;
            m_stream >> value;
            envData.append(value);
        }
        m_mods = Filesupport::readVectorList<QString, QList>(m_stream);
        if (m_version > 2)
        {
            m_winnerTeamPos = m_recordFile.pos();
            m_stream >> m_winnerTeam;
        }
        m_countPos = m_recordFile.pos();
        m_stream >> m_count;
        envData = qUncompress(envData);
        success = m_mods == Settings::getInstance()->getMods();
    }
    return success;
}

bool ReplayRecorder::readRecordInfo(QDataStream & stream, QByteArray & jsonRecordInfo, qint32 & version)
{
    stream >> version;
    QString marker;
    stream >> marker;
    bool success = marker == RECORD_INFO_MARKER;
    if (success)
    {
        if (version > 1)
        {
            stream >> jsonRecordInfo;
        }
    }
    return success;
}

bool ReplayRecorder::readRecordInfo(QDataStream & stream, QJsonObject & jsonRecordInfo)
{
    qint32 version = 0;
    QByteArray data;
    bool ret = readRecordInfo(stream, data, version);
    jsonRecordInfo = QJsonDocument::fromJson(data).object();
    return ret;
}

qint32 ReplayRecorder::getCount() const
{
    return m_count;
}

spGameAction ReplayRecorder::nextAction()
{
    if (m_playing)
    {
        if (!m_stream.atEnd() &&            
            m_progress < m_count)
        {
            m_progress++;
            bool success = true;
            seekToNextType(Type::Action, success);
            if (success)
            {
                spGameAction pAction = MemoryManagement::create<GameAction>(m_pMap);
                pAction->deserializeObject(m_stream);
                return pAction;
            }
        }
    }
    return spGameAction();
}

void ReplayRecorder::onPostAction()
{

}

ReplayRecorder::HeaderInfo ReplayRecorder::seekToNextType(Type type, bool & success)
{
    HeaderInfo info(m_stream);
    success = true;
    while (info.m_type != type && success)
    {
        m_recordFile.seek(info.m_nextSeekPos);
        if (!m_stream.atEnd())
        {
            info = HeaderInfo(m_stream);
        }
        else
        {
            success = false;
        }
    }
    return info;
}

QString ReplayRecorder::getRecordJson() const
{
    return m_recordJson;
}

void ReplayRecorder::seekToDay(IReplayReader::DayInfo dayInfo)
{
    if (dayInfo.day <= 1)
    {
        seekToStart();
    }
    else
    {        
        m_pMap->clearMap();
        if (dayInfo.day <= m_pMap->getCurrentDay())
        {
            m_recordFile.seek(m_streamStart);
        }
        bool found = false;
        while (!found && !m_stream.atEnd())
        {
            bool success = true;
            HeaderInfo info = seekToNextType(Type::Map, success);
            if (success)
            {
                qint32 curDay;
                qint32 curPlayer = 0;
                m_stream >> curDay;
                if (info.m_version > 1)
                {
                    m_stream >> curPlayer;
                }
                if (curDay == dayInfo.day &&
                    curPlayer == dayInfo.player)
                {
                    found = true;
                    m_stream >> m_progress;
                    m_pMap->deserializeObject(m_stream);
                    m_pMap->setIsHumanMatch(false);
                    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
                    {
                        m_pMap->getPlayer(i)->setBaseGameInput(BaseGameInputIF::createAi(m_pMap, GameEnums::AiTypes::AiTypes_ProxyAi));
                    }
                }
                else
                {
                    m_recordFile.seek(info.m_nextSeekPos);
                }
            }
            else
            {
                found = false;
            }
        }        
    }
}

void ReplayRecorder::seekToStart()
{    
    m_recordFile.seek(m_mapPos);
    m_pMap->detach();
    m_pMap->clearMap();
    m_pMap->deserializeObject(m_stream);
    m_pMap->setIsHumanMatch(false);
    m_progress = 0;
    // swap out all ai's / or players with a proxy ai.
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        m_pMap->getPlayer(i)->setBaseGameInput(BaseGameInputIF::createAi(m_pMap, GameEnums::AiTypes::AiTypes_ProxyAi));
    }    
}

IReplayReader::DayInfo ReplayRecorder::getDayFromPosition(qint32 count)
{
    qint64 curPos = m_recordFile.pos();
    DayInfo dayInfo;
    dayInfo.day = 1;
    dayInfo.player = 1;
    m_recordFile.seek(m_streamStart);
    bool found = false;
    while (!found && !m_stream.atEnd())
    {
        bool success = true;
        HeaderInfo info = seekToNextType(Type::Map, success);
        if (success)
        {
            qint32 curDay;
            qint32 player = 0;
            qint32 curCount;
            m_stream >> curDay;
            if (info.m_version > 1)
            {
                m_stream >> player;
            }
            m_stream >> curCount;
            if (curCount < count)
            {
                dayInfo.day = curDay;
                dayInfo.player = player;
                m_recordFile.seek(info.m_nextSeekPos);
            }
            else
            {
                found = true;
            }
        }
        else
        {
            found = true;
        }
    }
    m_recordFile.seek(curPos);
    return dayInfo;
}

QByteArray ReplayRecorder::createRecordJson() const
{
    if (m_pMap != nullptr)
    {
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_MAPNAME, m_pMap->getMapName());
        data.insert(JsonKeys::JSONKEY_MAPAUTHOR, m_pMap->getMapAuthor());
        data.insert(JsonKeys::JSONKEY_MAPDESCRIPTION, m_pMap->getMapDescription());
        data.insert(JsonKeys::JSONKEY_MAPWIDTH, m_pMap->getMapWidth());
        data.insert(JsonKeys::JSONKEY_MAPHEIGHT, m_pMap->getMapHeight());
        QJsonArray playerData;
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); ++i)
        {
            QJsonObject obj;
            auto* pPlayer = m_pMap->getPlayer(i);
            obj.insert(JsonKeys::JSONKEY_USERNAME, pPlayer->getPlayerNameId());
            QJsonArray coData;
            for (qint32 i = 0; i < pPlayer->getCoCount(); ++i)
            {
                QJsonObject coInfo;
                auto* pCO = pPlayer->getCO(i);
                if (pCO != nullptr)
                {
                    coInfo.insert(JsonKeys::JSONKEY_COID, pCO->getCoID());
                    coInfo.insert(JsonKeys::JSONKEY_CONAME, pCO->getCOName());
                }
                else
                {
                    coInfo.insert(JsonKeys::JSONKEY_COID, "");
                    coInfo.insert(JsonKeys::JSONKEY_CONAME, "");
                }
                coData.append(coInfo);
            }
            obj.insert(JsonKeys::JSONKEY_COS, coData);
            playerData.append(obj);
        }
        data.insert(JsonKeys::JSONKEY_PLAYERDATA, playerData);
        if (!Mainapp::getInstance()->getNoUi())
        {
            // we can only render an image if opengl and textures are loaded / available
            Minimap minimap;
            minimap.updateMinimap(m_pMap);
            QImage image;
            Mainapp::getInstance()->saveMapAsImage(&minimap, &image);
            QByteArray imageArray;
            QBuffer buffer(&imageArray);
            buffer.open(QIODevice::WriteOnly);
            image.save(&buffer, "PNG");
            buffer.close();
            data.insert(JsonKeys::JSONKEY_MINIMAPDATA, GlobalUtils::toJsonArray(imageArray));
        }
        QJsonArray modsInfos;
        Settings* pSettings = Settings::getInstance();
        auto activeMods = pSettings->getActiveMods();
        for (auto mod : activeMods)
        {
            QJsonObject modInfo;
            modInfo.insert(JsonKeys::JSONKEY_MODPATH, mod);
            modInfo.insert(JsonKeys::JSONKEY_MODNAME, pSettings->getModName(mod));
            modInfo.insert(JsonKeys::JSONKEY_MODVERSION, pSettings->getModVersion(mod));
            modsInfos.append(modInfo);
        }
        data.insert(JsonKeys::JSONKEY_MODINFOS, modsInfos);

        QJsonDocument doc(data);
        return doc.toJson(QJsonDocument::Compact);
    }
    return "";
}
