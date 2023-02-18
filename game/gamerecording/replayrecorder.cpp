#include <QByteArray>
#include <QDateTime>

#include "game/gamerecording/replayrecorder.h"

#include "game/gamemap.h"
#include "game/gameaction.h"

#include "coreengine/settings.h"
#include "coreengine/interpreter.h"
#include "coreengine/filesupport.h"
#include "coreengine/gameconsole.h"

#include "gameinput/basegameinputif.h"


ReplayRecorder::ReplayRecorder(GameMap* pMap)
    : m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("ReplayRecorder");
#endif
    Interpreter::setCppOwnerShip(this);
}

ReplayRecorder::~ReplayRecorder()
{
    CONSOLE_PRINT("Saving record", GameConsole::eDEBUG);
    if (m_recording)
    {
        m_dailyMapPos = m_recordFile.pos();
        m_dailyBuffer.seek(0);
        Filesupport::writeByteArray(m_stream, m_dailyBuffer.buffer());
        m_recordFile.seek(m_countPos);
        m_stream << m_count;
        m_recordFile.seek(m_posOfDailyMapPos);
        m_stream << m_dailyMapPos;
        m_recordFile.flush();
    }
    m_recordFile.close();
}

void ReplayRecorder::startRecording()
{    
    if (Settings::getRecord() && m_pMap != nullptr && !Settings::getAiSlave())
    {
        CONSOLE_PRINT("Starting recording", GameConsole::eDEBUG);
        // compress script enviroment
        QByteArray data = Interpreter::getInstance()->getRuntimeData().toUtf8();
        data = qCompress(data);
        QString currentDate = QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss");
        QString fileName = "data/records/" + m_pMap->getMapName() + "-" + currentDate + ".rec";
        m_recordFile.setFileName(fileName);
        m_recordFile.open(QIODevice::WriteOnly);
        m_dailyBuffer.open(QIODevice::ReadWrite);
        m_stream << static_cast<qint32>(data.size());
        for (qint32 i = 0; i < data.size(); i++)
        {
            m_stream << static_cast<qint8>(data.at(i));
        }
        QStringList mods = Settings::getMods();
        Filesupport::writeVectorList(m_stream, mods);
        m_countPos = m_recordFile.pos();
        m_stream << m_count;
        m_posOfDailyMapPos = m_recordFile.pos();
        m_stream << m_dailyMapPos;
        m_pMap->serializeObject(m_stream);
        m_recordFile.flush();
        m_recording = true;
        m_currentDay = m_pMap->getCurrentDay();
    }
}

void ReplayRecorder::recordAction(spGameAction pAction)
{
    
    if (m_recording && !pAction->getIsLocal() && m_pMap != nullptr)
    {
        qint32 curDay = m_pMap->getCurrentDay();
        if (m_currentDay != curDay && curDay > 1)
        {
            m_currentDay = curDay;
            qint64 curPos = m_dailyBuffer.pos();
            qint64 size = 0;
            m_dailyStream << size;
            m_dailyStream << m_pMap->getCurrentDay();
            m_dailyStream << m_count;
            m_dailyStream << m_recordFile.pos();
            
            m_pMap->serializeObject(m_dailyStream);
            qint64 seekPos = m_dailyBuffer.pos();
            m_dailyBuffer.seek(curPos);
            size = (seekPos - curPos);
            m_dailyStream << size;
            m_dailyBuffer.seek(seekPos);
        }
        pAction->serializeObject(m_stream);
        m_recordFile.flush();
        m_count++;
    }
}

bool ReplayRecorder::loadRecord(const QString & filename)
{
    m_recordFile.setFileName(filename);
    if (m_recordFile.exists())
    {
        m_recordFile.open(QIODevice::ReadOnly);
        QByteArray envData;
        qint32 size = 0;
        m_stream >> size;
        for (qint32 i = 0; i < size; i++)
        {
            qint8 value = 0;
            m_stream >> value;
            envData.append(value);
        }
        envData = qUncompress(envData);
        m_mods = Filesupport::readVectorList<QString, QList>(m_stream);
        if (m_mods == Settings::getMods())
        {
            QString interpreterEnvironment(envData);
            if (Interpreter::reloadInterpreter(interpreterEnvironment))
            {
                m_stream >> m_count;
                m_stream >> m_dailyMapPos;
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

spGameAction ReplayRecorder::nextAction()
{
    if (m_playing)
    {
        if (!m_stream.atEnd() &&
            m_recordFile.pos() < m_dailyMapPos &&
            m_progress < m_count)
        {
            m_progress++;
            spGameAction pAction = spGameAction::create(m_pMap);
            pAction->deserializeObject(m_stream);
            return pAction;
        }
    }
    return spGameAction();
}

void ReplayRecorder::seekToDay(qint32 day)
{
    if (day <= 1)
    {
        seekToStart();
    }
    else
    {        
        m_pMap->clearMap();
        m_recordFile.seek(m_dailyMapPos);
        // not needed size of the buffer array
        qint32 bufferSize = 0;
        m_stream >> bufferSize;
        bool found = false;
        while (!found && !m_stream.atEnd())
        {
            qint64 seekPos = m_recordFile.pos();
            qint64 size = 0;
            m_stream >> size;
            qint32 curDay;
            qint32 curCount;
            m_stream >> curDay;
            m_stream >> curCount;
            if (curDay == day)
            {
                qint64 seekPos = 0;
                m_stream >> seekPos;
                m_progress = curCount;
                m_pMap->deserializeObject(m_stream);
                m_pMap->setIsHumanMatch(false);
                for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
                {
                    m_pMap->getPlayer(i)->setBaseGameInput(BaseGameInputIF::createAi(m_pMap, GameEnums::AiTypes::AiTypes_ProxyAi));
                }
                m_recordFile.seek(seekPos);
                found = true;
            }
            else
            {
                m_recordFile.seek(seekPos + size);
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

qint32 ReplayRecorder::getDayFromPosition(qint32 count)
{
    qint64 curPos = m_recordFile.pos();
    m_recordFile.seek(m_dailyMapPos);
    // not needed size of the buffer array
    qint32 bufferSize = 0;
    m_stream >> bufferSize;
    bool found = false;
    qint32 rDay = 1;
    while (!found && !m_stream.atEnd())
    {
        qint64 seekPos = m_recordFile.pos();
        qint64 size = 0;
        m_stream >> size;
        qint32 day;
        qint32 curCount;
        m_stream >> day;
        m_stream >> curCount;
        if (curCount < count)
        {
            rDay = day;
            m_recordFile.seek(seekPos + size);
        }
        else
        {
            found = true;
        }
    }
    m_recordFile.seek(curPos);
    return rDay;
}
