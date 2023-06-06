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
    /**
     * @brief getMap
     * @return
     */
    Q_INVOKABLE GameMap* getMap() const;
    /**
     * @brief getPlanner
     * @return
     */
    Q_INVOKABLE MovementPlanner* getPlanner() const;
    /**
     * @brief getVariables
     * @return
     */
    Q_INVOKABLE inline ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
    /**
     * @brief hide
     */
    Q_INVOKABLE void hide();
    /**
     * @brief addSprite
     * @param name
     * @param x
     * @param y
     */
    Q_INVOKABLE void addSprite(QString spriteID, qint32 x, qint32 y, float offsetX = 0, float offsetY = 0, QColor color = Qt::white, qint32 frameTime = 150);
    /**
     * @brief removeAllSprites
     */
    Q_INVOKABLE void removeAllSprites();
    /**
     * @brief onMenuInputDone
     */
    Q_INVOKABLE void onMenuInputDone();

private:
    QString m_addIn;
    ScriptVariables m_Variables;
    QVector<oxygine::spSprite> m_sprites;
    GameMap* m_pMap{nullptr};
    MovementPlanner* m_pPlanner{nullptr};
};

Q_DECLARE_INTERFACE(MovementPlannerAddIn, "MovementPlannerAddIn");
