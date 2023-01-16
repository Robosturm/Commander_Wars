#pragma once

#include <QString>

class BaseGamemenu;

class GamemapImageSaver final
{
public:
    static void saveMapAsImage(QString filename, BaseGamemenu& menu);
    GamemapImageSaver() = delete;
    ~GamemapImageSaver() = delete;
};

