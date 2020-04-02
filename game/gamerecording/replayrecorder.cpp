#include "replayrecorder.h"

#include "coreengine/settings.h"
#include "coreengine/interpreter.h"
#include "game/gamemap.h"
#include "game/gameaction.h"

#include <QByteArray>

ReplayRecorder::ReplayRecorder()
    : QObject()
{

}

void ReplayRecorder::startRecording()
{
    if (Settings::getRecord())
    {
        // compress script enviroment
        QByteArray data = Interpreter::getRuntimeData().toUtf8();
        data = qCompress(data);
        m_recordFile.setFileName("test.rec");
        m_recordFile.open(QIODevice::ReadWrite);
        m_stream << static_cast<qint32>(data.size());
        for (qint32 i = 0; i < data.size(); i++)
        {
            m_stream << static_cast<qint8>(data.at(i));
        }
        GameMap::getInstance()->serializeObject(m_stream);
        m_recording = true;

    }
}

void ReplayRecorder::recordAction(GameAction* pAction)
{
    if (m_recording)
    {
        pAction->serializeObject(m_stream);
    }
}

//// test code
//m_recordFile.close();
//m_recordFile.open(QIODevice::ReadWrite);
//QByteArray test;
//qint32 size = 0;
//m_stream >> size;
//for (qint32 i = 0; i < size; i++)
//{
//    qint8 value = 0;
//    m_stream >> value;
//    test.append(value);
//}
//test = qUncompress(test);
//QString str(test);
