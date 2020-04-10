#ifndef LABEL_H
#define LABEL_H

#include "objects/tooltip.h"

class Label;
typedef oxygine::intrusive_ptr<Label> spLabel;

class Label : public Tooltip
{
    Q_OBJECT
public:
    Label(qint32 width, qint32 heigth = 100);
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
protected:
    oxygine::spTextField _textField;
    oxygine::spClipRectActor _clipRect;
};

#endif // LABEL_H
