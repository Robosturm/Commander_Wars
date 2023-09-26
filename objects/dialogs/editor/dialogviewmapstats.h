#pragma once

#include <QObject>
#include "ui_reader/createdgui.h"

class GameMap;
class DialogViewMapStats;
using spDialogViewMapStats = std::shared_ptr<DialogViewMapStats>;

class DialogViewMapStats final : public CreatedGui
{
    Q_OBJECT
public:
    explicit DialogViewMapStats(GameMap* pMap);
    virtual ~DialogViewMapStats();
    void init();
signals:
    void sigClosed();
public slots:
    GameMap* getMap();
    void close();
protected slots:
    void remove();
private:
    GameMap* m_pMap{nullptr};
};

