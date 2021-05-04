#ifndef SCRIPTEVENTANIMATION_H
#define SCRIPTEVENTANIMATION_H

#include "scriptevent.h"

#include "objects/base/textbox.h"

class ScriptEventAnimation;
using spScriptEventAnimation = oxygine::intrusive_ptr<ScriptEventAnimation>;

class ScriptEventAnimation : public ScriptEvent
{
    Q_OBJECT
public:
    ScriptEventAnimation();
    /**
     * @brief readEvent
     * @param rStream
     */
    virtual void readEvent(QTextStream& rStream) override;
    /**
     * @brief writeEvent
     * @param rStream
     */
    virtual void writeEvent(QTextStream& rStream) override;
    /**
     * @brief showEditEvent
     */
    virtual void showEditEvent(spScriptEditor pScriptEditor) override;
    /**
     * @brief getVersion
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 0;
    }
    /**
     * @brief getDescription
     * @return
     */
    virtual QString getDescription() override
    {
        return tr("Animation");
    }
    /**
     * @brief getSprite
     * @return
     */
    QString getSprite() const;
    /**
     * @brief setSprite
     * @param value
     */
    void setSprite(const QString &value);
    /**
     * @brief getFrames
     * @return
     */
    qint32 getFrames() const;
    /**
     * @brief setFrames
     * @param value
     */
    void setFrames(const qint32 &value);
    /**
     * @brief getX
     * @return
     */
    qint32 getX() const;
    /**
     * @brief setX
     * @param value
     */
    void setX(const qint32 &value);

    qint32 getY() const;
    void setY(const qint32 &value);
    /**
     * @brief getOffsetX
     * @return
     */
    qint32 getOffsetX() const;
    /**
     * @brief setOffsetX
     * @param value
     */
    void setOffsetX(const qint32 &value);
    /**
     * @brief getOffsetY
     * @return
     */
    qint32 getOffsetY() const;
    /**
     * @brief setOffsetY
     * @param value
     */
    void setOffsetY(const qint32 &value);
    /**
     * @brief getColor
     * @return
     */
    QColor getColor() const;
    /**
     * @brief setColor
     * @param value
     */
    void setColor(const QColor &value);
    /**
     * @brief getSleepAfterFinish
     * @return
     */
    qint32 getSleepAfterFinish() const;
    /**
     * @brief setSleepAfterFinish
     * @param value
     */
    void setSleepAfterFinish(const qint32 &value);
    /**
     * @brief getScaleX
     * @return
     */
    float getScaleX() const;
    /**
     * @brief setScaleX
     * @param value
     */
    void setScaleX(float value);
    /**
     * @brief getScaleY
     * @return
     */
    float getScaleY() const;
    /**
     * @brief setScaleY
     * @param value
     */
    void setScaleY(float value);
    /**
     * @brief getDelay
     * @return
     */
    qint32 getDelay() const;
    /**
     * @brief setDelay
     * @param value
     */
    void setDelay(const qint32 &value);
signals:
    void sigShowLoadDialog();
public slots:
    void showLoadDialog();
    void selectAnimation(QString id);
private:
    QString sprite;
    qint32 frames{0};
    qint32 x{0};
    qint32 y{0};
    qint32 offsetX{0};
    qint32 offsetY{0};
    QColor color{Qt::white};
    qint32 sleepAfterFinish{0};
    float scaleX;
    float scaleY;
    qint32 delay;

    spTextbox m_pTextbox;
};

#endif // SCRIPTEVENTANIMATION_H
