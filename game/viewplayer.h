#ifndef VIEWPLAYER_H
#define VIEWPLAYER_H

#include "game/player.h"
#include "gameinput/humanplayerinput.h"

class Viewplayer;
typedef oxygine::intrusive_ptr<Viewplayer> spViewplayer;

class Viewplayer : public Player
{
    Q_OBJECT
public:
    enum ViewType
    {
        CurrentTeam = -3,
        AllTeams  = -2,
        Map  = -1,
    };

    explicit Viewplayer(GameMap* pMap);
    virtual ~Viewplayer() = default;
    /**
     * @brief getColorTableAnim
     * @return
     */
    virtual oxygine::spResAnim getColorTableAnim() const override;
public slots:
    /**
     * @brief getFieldVisible
     * @param x
     * @param y
     */
    virtual bool getFieldVisible(qint32 x, qint32 y) override;
    /**
     * @brief getFieldVisibleType
     * @param x
     * @param y
     * @return
     */
    virtual GameEnums::VisionType getFieldVisibleType(qint32 x, qint32 y) override;
    /**
     * @brief getFieldDirectVisible
     * @param x
     * @param y
     * @return if true a stealthed unit will be made visible by this players view
     */
    virtual bool getFieldDirectVisible(qint32 x, qint32 y) override;
    /**
     * @brief getViewType
     * @return
     */
    qint32 getViewType() const;
    /**
     * @brief setViewType
     * @param ViewType
     */
    void setViewType(const qint32 &ViewType);
private:
    qint32 m_ViewType{static_cast<qint32>(ViewType::CurrentTeam)};
    HumanPlayerInput m_input;
};

#endif // VIEWPLAYER_H
