#ifndef HUMANPLAYERINPUT_H
#define HUMANPLAYERINPUT_H

#include <QVector>

#include "gameinput/basegameinputif.h"

#include "menue/gamemenue.h"

#include "oxygine-framework.h"

#include "gameinput/humanplayerinputmenu.h"

class GameAction;
class UnitPathFindingSystem;

class HumanPlayerInput : public BaseGameInputIF
{
    Q_OBJECT
public:
    enum class Arrows
    {
        LeftRight = 0,
        UpDown,
        UpRight,
        UpLeft,
        DownRight,
        DownLeft,
        Right,
        Left,
        Up,
        Down
    };

    HumanPlayerInput(GameMenue* pMenue);
    ~HumanPlayerInput();
    /**
     * @brief deleteArrow deletes the current unit path arrow
     */
    void deleteArrow();
    /**
     * @brief selectUnit selects the given target unit as active
     * @param x
     * @param y
     */
    void selectUnit(qint32 x, qint32 y);
    /**
     * @brief createCursorPath creates the arrow showing the current unit path
     * @param x
     * @param y
     */
    void createCursorPath(qint32 x, qint32 y);
    /**
     * @brief createActionMenu creates the menu for selecting the next action
     * @param actionIDs
     */
    void createActionMenu(QStringList actionIDs, qint32 x, qint32 y);
    /**
     * @brief finishAction finishes input and performs the action
     */
    void finishAction();
    /**
     * @brief cleanUpInput resets all internal data
     */
    void cleanUpInput();
signals:
    /**
     * @brief performAction signal with an action to be performed the action has to be deleted by the reciever of this slot. Only one slot can be connected to this signal
     * @param pAction
     */
    void performAction(GameAction* pAction);
public slots:
    void rightClick(qint32 x, qint32 y);
    void leftClick(qint32 x, qint32 y);
    void cursorMoved(qint32 x, qint32 y);
    void menuItemSelected(QString itemID);
    QStringList getEmptyActionList();
private:
    GameAction* m_pGameAction{nullptr};
    UnitPathFindingSystem* m_pUnitPathFindingSystem{nullptr};

    QVector<oxygine::spActor> m_Fields;
    QVector<QPoint> m_FieldPoints;

    QVector<oxygine::spActor> m_Arrows;
    QVector<QPoint> m_ArrowPoints;

    spHumanPlayerInputMenu m_CurrentMenu{nullptr};
};

#endif // HUMANPLAYERINPUT_H
