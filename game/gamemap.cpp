#include "game/gamemap.h"
#include "coreengine/mainapp.h"

const QString GameMap::m_JavascriptName = "map";
const qint32 GameMap::frameTime = 150;

GameMap::GameMap(qint32 width, qint32 heigth)
    : width(width),
      heigth(heigth)
{
    Interpreter* pInterpreter = Mainapp::getInstance()->getInterpreter();
    pInterpreter->setGlobal(m_JavascriptName, pInterpreter->newQObject(this));

    for (qint32 y = 0; y < heigth; y++)
    {
        fields.append(new QVector<Terrain*>());
        for (qint32 x = 0; x < width; x++)
        {
            // test code
            Terrain* pTerrain = new Terrain("REAF", x, y);
            oxygine::Actor::addChild(pTerrain);
            fields[y]->append(pTerrain);
            pTerrain->setPosition(x * 24.0f, y * 24.0f);
        }
    }
    updateTerrainSprites();
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

void GameMap::updateTerrainSprites()
{
    // update terrain sprites
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            fields.at(x)->at(y)->loadSprites();
        }
    }
}

QVector<QVector<Terrain *> *> GameMap::getFields() const
{
    return fields;
}

qint32 GameMap::getWidth() const
{
    return width;
}

void GameMap::setWidth(const qint32 &value)
{
    width = value;
}

qint32 GameMap::getHeigth() const
{
    return heigth;
}

void GameMap::setHeigth(const qint32 &value)
{
    heigth = value;
}

void GameMap::getField(qint32& x, qint32& y, Directions direction)
{
    switch (direction)
    {
        case GameMap::Directions::North:
        {
            x++;
            break;
        }
        case GameMap::Directions::NorthEast:
        {
            x++;
            y++;
            break;
        }
        case GameMap::Directions::East:
        {
            x++;
            break;
        }
        case GameMap::Directions::SouthEast:
        {
            y--;
            x++;
            break;
        }
        case GameMap::Directions::South:
        {
            y--;
            break;
        }
        case GameMap::Directions::SouthWest:
        {
            y--;
            x--;
            break;
        }
        case GameMap::Directions::West:
        {
            x--;
            break;
        }
        case GameMap::Directions::NorthWest:
        {
            y++;
            x--;
            break;
        }
}
