#include "game/gamemap.h"
#include "coreengine/mainapp.h"

const QString GameMap::m_JavascriptName = "map";

GameMap::GameMap(qint32 width, qint32 heigth)
{
    Interpreter* pInterpreter = Mainapp::getInstance()->getInterpreter();
    pInterpreter->setGlobal(m_JavascriptName, pInterpreter->newQObject(this));

    for (qint32 y = 0; y < heigth; y++)
    {
        fields.append(new QVector<Terrain*>());
        for (qint32 x = 0; x < width; x++)
        {
            Terrain* pTerrain = new Terrain("PLAINS", x, y);
            oxygine::Actor::addChild(pTerrain);
            fields[y]->append(pTerrain);
            pTerrain->setPosition(x * 24.0f, y * 24.0f);
        }
    }



}

GameMap::GameMap(QString map)
{

}

GameMap::~GameMap()
{
    // remove us from the interpreter again
    Interpreter* pInterpreter = Mainapp::getInstance()->getInterpreter();
    pInterpreter->deleteObject(m_JavascriptName);
}

QVector<QVector<Terrain *> *> GameMap::getFields() const
{
    return fields;
}
