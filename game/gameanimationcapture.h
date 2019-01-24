#ifndef GAMEANIMATIONCAPTURE_H
#define GAMEANIMATIONCAPTURE_H

#include <QColor>

#include "game/gameanimation.h"

class GameAnimationCapture : public GameAnimation
{
    Q_OBJECT
public:
    explicit GameAnimationCapture(qint32 startPoints, qint32 endPoints, qint32 maxPoints);

public slots:
    /**
     * @brief addBackgroundSprite adds a background sprite with no functionality. Besides looking pretty.
     * @param spriteID string id of the sprite in the res file
     */
    void addBackgroundSprite(QString spriteID);
    /**
     * @brief addSoldierSprite adds a soldier animation to this capture animation that does some cool jumping and moving down and up
     * @param spriteID string id of the sprite in the res file
     * @param color color we want to add to the sprite only applied if addColor is true
     * @param addColor true if we want to repaint the sprite with the unit color
     */
    void addSoldierSprite(QString spriteID, QColor color, bool addColor);
    /**
     * @brief addBuildingSprite adds the cool building sprite that's getting streched in and out.
     * @param spriteID string id of the sprite in the res file
     * @param startColor color of the current owner
     * @param capturedColor color of the capturing player
     * @param addColor true if we want to repaint the sprite with the unit color
     */
    void addBuildingSprite(QString spriteID, QColor startColor, QColor capturedColor, bool addColor);
private:
    qint32 m_startPoints{0};
    qint32 m_endPoints{0};
    qint32 m_maxPoints{0};
    float m_BuildingStartPos{20.0f};
    float m_BuildingEndPos{65.0f};
    static const qint32 capturingFactor;
    static const qint32 jumpSprites;
    static const qint32 ayeAyeSprites;
    static const qint32 jumpingCount;
};

#endif // GAMEANIMATIONCAPTURE_H
