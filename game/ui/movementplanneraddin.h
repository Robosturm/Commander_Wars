#pragma once

#include <QObject>
#include <QString>

#include "ui_reader/createdgui.h"

#include "coreengine/scriptvariables.h"

class MovementPlanner;
class GameMap;
class MovementPlannerAddIn;
using spMovementPlannerAddIn = oxygine::intrusive_ptr<MovementPlannerAddIn>;

class MovementPlannerAddIn final : public CreatedGui
{
    Q_OBJECT
public:
    explicit MovementPlannerAddIn(QString addIn, GameMap* pMap, MovementPlanner* pPlanner);
    ~MovementPlannerAddIn() = default;

    QString getAddIn() const;
    /**
     * @brief show
     */
    void show();
public slots:
    /**
     * @brief hide
     */
    void hide();
    /**
     * @brief getVariables
     * @return
     */
    inline ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
    /**
     * @brief addSprite
     * @param name
     * @param x
     * @param y
     */
    void addSprite(QString spriteID, qint32 x, qint32 y, float offsetX = 0, float offsetY = 0, QColor color = Qt::white, qint32 frameTime = 150);
    /**
     * @brief removeAllSprites
     */
    void removeAllSprites();
    /**
     * @brief onMenuInputDone
     */
    void onMenuInputDone();
    /**
     * @brief getMap
     * @return
     */
    GameMap* getMap() const;
    /**
     * @brief getPlanner
     * @return
     */
    MovementPlanner* getPlanner() const;
private:
    QString m_addIn;
    ScriptVariables m_Variables;
    QVector<oxygine::spSprite> m_sprites;
    GameMap* m_pMap{nullptr};
    MovementPlanner* m_pPlanner{nullptr};
};

Q_DECLARE_INTERFACE(MovementPlannerAddIn, "MovementPlannerAddIn");
