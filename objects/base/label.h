#ifndef LABEL_H
#define LABEL_H

#include "objects/base/tooltip.h"
#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"
#include "3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.h"

class Label;
using spLabel = oxygine::intrusive_ptr<Label>;

class Label final : public Tooltip
{
    Q_OBJECT
public:
    explicit Label(qint32 width, bool showBox = false);
    ~Label() = default;
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
    const auto& getTextRect()
    {
        return m_textField->getTextRect();
    }
    /**
     * @brief setWidth
     * @param width
     */
    virtual void setWidth(qint32 width) override;
    /**
     * @brief setHeigth
     * @param height
     */
    virtual void setHeight(qint32 height) override;
public slots:
    virtual void focusedLost() override {}
protected:
    oxygine::spTextField m_textField;
    oxygine::spClipRectActor m_clipRect;
    qint32 m_clipOffset{0};
};

#endif // LABEL_H
