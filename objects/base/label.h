#ifndef LABEL_H
#define LABEL_H

#include "objects/base/tooltip.h"

class Label;
typedef oxygine::intrusive_ptr<Label> spLabel;

class Label : public Tooltip
{
    Q_OBJECT
public:
    Label(qint32 width);
    /**
     * @brief setText
     * @param str
     */
    void setText(QString str);
    /**
     * @brief setHtmlText
     * @param str
     */
    void setHtmlText(QString str);
    /**
     * @brief setStyle
     * @param st
     */
    void setStyle(const oxygine::TextStyle& st);
    /**
     * @brief getTextRect
     * @param localScale
     * @return
     */
    const oxygine::Rect& getTextRect(float localScale = 1.0f)
    {
        return _textField->getTextRect(localScale);
    }
    /**
     * @brief setWidth
     * @param width
     */
    virtual void setWidth(float width) override;
protected:
    oxygine::spTextField _textField;
    oxygine::spClipRectActor _clipRect;
};

#endif // LABEL_H
