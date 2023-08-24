#pragma once

#include <QString>
#include <QImage>

class BaseGamemenu;
class Minimap;

class GamemapImageSaver final
{
public:
    static void saveMapAsImage(QString filename, BaseGamemenu& menu);
    static void saveMapAsImage(Minimap* pMinimap, QImage & img);
    GamemapImageSaver() = delete;
    ~GamemapImageSaver() = delete;
};

