#ifndef GAMEANIMATIONDIALOG_H
#define GAMEANIMATIONDIALOG_H

#include <QTimer>
#include <QElapsedTimer>
#include "3rd_party/oxygine-framework/oxygine/KeyEvent.h"

#include "game/gameanimation/gameanimation.h"
#include "game/GameEnums.h"

class GameAnimationDialog;
using spGameAnimationDialog = oxygine::intrusive_ptr<GameAnimationDialog>;

class GameAnimationDialog : public GameAnimation
{
    Q_OBJECT
public:
    static const qint32 dialogHeigth = 96;
    GameAnimationDialog(quint32 frameTime);

    virtual void restart() override;
    void finishDialog();
    void updateShownText();
signals:
    void sigStartFinishTimer();
    void sigRightClick();
    void sigLeftClick();
public slots:
    /**
     * @brief nextDialogStep
     */
    void nextDialogStep();
    /**
     * @brief keyInput
     * @param event
     */
    void keyInput(oxygine::KeyEvent event);
    /**
     * @brief loadCoSprite
     * @param coid
     * @param offsetX
     * @param offsetY
     * @param scale
     * @param flippedX
     */
    void loadCoSprite(QString coid, float offsetX, float offsetY, bool flippedX, float scale = 1.0f);
    /**
     * @brief onFinished
     * @param skipping
     * @return
     */
    virtual bool onFinished(bool skipping) override;
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
     * @brief setPlayerCO
     * @param player
     * @param co
     * @param mood
     */
    void setPlayerCO(qint32 player, quint8 co, GameEnums::COMood mood);
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
    /**
     * @brief loadBackground
     * @param file path relative to the Commander Wars.exe
     */
    void loadBackground(QString file);
protected:
    virtual void update(const oxygine::UpdateState& us) override;
    void startFinishTimer();
private:
    QString m_Text;
    qint32 m_writePosition{0};
    qint32 m_autoFinishMs{-1};
    QTimer m_finishTimer;

    oxygine::spSprite m_TextMask;
    oxygine::spSprite m_TextBackground;
    oxygine::spSprite m_COSprite;

    oxygine::spTextField m_TextField;
    QElapsedTimer m_textTimer;
    qint32 m_textSpeed;

    // background sprite handling
    oxygine::spSprite m_BackgroundSprite;
    oxygine::spResAnim m_BackgroundAnim;

    bool m_paused{false};
};

#endif // GAMEANIMATIONDIALOG_H
