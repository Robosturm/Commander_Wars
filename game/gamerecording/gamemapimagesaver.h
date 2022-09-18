#pragma once

#include <QString>

class GamemapImageSaver final
{
public:
    static void saveMapAsImage(QString filename);
    GamemapImageSaver() = delete;
    ~GamemapImageSaver() = delete;
};

