#ifndef GAMEANIMATIONDIALOG_H
#define GAMEANIMATIONDIALOG_H

#include "game/gameanimation.h"

#include "game/GameEnums.h"

#include <QTimer>
#include <QTime>

class GameAnimationDialog : public GameAnimation
{
    Q_OBJECT
public:
    GameAnimationDialog(quint32 frameTime);

    virtual void restart() override;
signals:
    void sigStartFinishTimer();
    void sigRightClick();
public slots:
    void rightClick();
    virtual bool onFinished() override;
    /**
     * @brief setPositionTop wether dialog appears on top or bottom of the game
     * @param value
     */
    void setPositionTop(bool value);
    /**
     * @brief setColor
     */
    void setColor(QColor color);
    /**
     * @brief setDialog
     * @param text
     */
    void setDialog(QString text);
    /**
     * @brief setCO
     * @param coid
     */
    void setCO(QString coid, GameEnums::COMood mood);
    /**
     * @brief setFinishDelay sets if the dialog disappears automatically if all text was displayed -1 for infinite
     * @param value
     */
    void setFinishDelay(qint32 valueMs);
    /**
     * @brief setTextSpeed
     * @param speed
     */
    void setTextSpeed(qint32 speed);

protected:
    virtual void update(const oxygine::UpdateState& us) override;
    void startFinishTimer();
private:
    QString m_Text;
    qint32 writePosition{0};
    qint32 autoFinishMs{-1};
    QTimer finishTimer;

    oxygine::spSprite m_TextMask;
    oxygine::spSprite m_TextBackground;
    oxygine::spSprite m_COSprite;

    oxygine::spTextField m_TextField;
    QTime textTimer;
    qint32 textSpeed{100};

    float textScale{1.7f};
};

#endif // GAMEANIMATIONDIALOG_H
