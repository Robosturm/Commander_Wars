#ifndef LABEL_H
#define LABEL_H

#include "objects/base/tooltip.h"

class Label;
typedef oxygine::intrusive_ptr<Label> spLabel;

class Label : public Tooltip
{
    Q_OBJECT
public:
    explicit Label(qint32 width);
    virtual ~Label() = default;
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
     * @brief setStyle
     * @param st
     */
    oxygine::TextStyle getStyle();
    /**
     * @brief getTextRect
     * @param localScale
     * @return
     */
    const oxygine::Rect& getTextRect()
    {
        return m_textField->getTextRect();
    }
    /**
     * @brief setWidth
     * @param width
     */
    virtual void setWidth(float width) override;
    /**
     * @brief setHeigth
     * @param height
     */
    virtual void setHeight(float height) override;
public slots:
    virtual void focusedLost() override {}
protected:
    oxygine::spTextField m_textField;
    oxygine::spClipRectActor m_clipRect;
};

#endif // LABEL_H
