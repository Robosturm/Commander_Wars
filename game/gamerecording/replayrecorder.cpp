#include "replayrecorder.h"

#include "coreengine/settings.h"
#include "coreengine/interpreter.h"
#include "game/gamemap.h"
#include "game/gameaction.h"
#include "gameinput/basegameinputif.h"

#include <QByteArray>
#include <QDateTime>

ReplayRecorder::ReplayRecorder()
    : QObject()
{

}

ReplayRecorder::~ReplayRecorder()
{
    m_recordFile.flush();
    m_recordFile.close();
    if (playing)
    {
        Interpreter::reloadInterpreter(Interpreter::getRuntimeData());
    }
}

void ReplayRecorder::startRecording()
{
    if (Settings::getRecord())
    {
        GameMap* pMap = GameMap::getInstance();
        // compress script enviroment
        QByteArray data = Interpreter::getRuntimeData().toUtf8();
        data = qCompress(data);
        QString currentDate = QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss");
        QString fileName = "data/records/" + pMap->getMapName() + "-" + currentDate + ".rec";
        m_recordFile.setFileName(fileName);
        m_recordFile.open(QIODevice::WriteOnly);
        m_stream << static_cast<qint32>(data.size());
        for (qint32 i = 0; i < data.size(); i++)
        {
            m_stream << static_cast<qint8>(data.at(i));
        }
        pMap->serializeObject(m_stream);
        m_recordFile.flush();
        m_recording = true;

    }
}

void ReplayRecorder::recordAction(GameAction* pAction)
{
    if (m_recording)
    {
        pAction->serializeObject(m_stream);
        m_recordFile.flush();
    }
}

void ReplayRecorder::loadRecord(QString filename)
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
        QString interpreterEnvironment(envData);
        Interpreter::reloadInterpreter(interpreterEnvironment);
        // load map
        GameMap* pMap = new GameMap(m_stream);
        // swap out all ai's / or players with a proxy ai.
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
            pMap->getPlayer(i)->setBaseGameInput(BaseGameInputIF::createAi(GameEnums::AiTypes::AiTypes_ProxyAi));
        }
        playing = true;
    }
}


void ReplayRecorder::nextAction()
{
    if (playing)
    {
        if (!m_stream.atEnd())
        {
            GameAction* pAction = new GameAction();
            pAction->deserializeObject(m_stream);
            // todo restream it
        }
    }
}
