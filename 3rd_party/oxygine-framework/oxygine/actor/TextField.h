#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/TextStyle.h"
#include "3rd_party/oxygine-framework/oxygine/VisualStyle.h"
#include "3rd_party/oxygine-framework/oxygine/text_utils/Node.h"

namespace oxygine
{
    class TextField;
    using spTextField = intrusive_ptr<TextField>;
    class TextField : public Actor
    {
    public:
        explicit TextField();
        virtual ~TextField() = default;

        /**Returns current text style*/
        const TextStyle& getStyle() const
        {
#ifdef GRAPHICSUPPORT
            return m_style;
#else
            return m_dummyTextStyle;
#endif
        }
        /**Returns text bounds*/
        const Rect& getTextRect() const;
        /**Returns current text*/
        const QString & getText() const;
        const Font & getFont() const;

        TextStyle::HorizontalAlign getHAlign() const;
        bool getMultiline() const;

        virtual bool getBounds(RectF&) const override;
        /**Overwrites TextStyle Horizontal align*/
        void setHAlign(TextStyle::HorizontalAlign align);
        /**Overwrites TextStyle multiline*/
        void setMultiline(bool multiline);
        /**Overwrites TextStyle color*/
        void setStyleColor(const QColor&);
        const QColor& getStyleColor() const;

        /**Overwrites TextStyle font.*/
        void setFont(Font & rs);

        void setStyle(const TextStyle& st);
        /**Changes text utf-8 string*/
        void setText(const QString & str, bool lock = true);
        /**Changes text.
        Supports pseudo html style tags:  <br/> - break line,  <div color='FFFFFF'/>colored text </div>, <div c='FFFFFF'/>colored text</div>,
        in pair with oxygine-freetype you may use 'opt': <div opt=123>custom option</div>, <div c='ffffff' opt=123>custom option with color</div>
        example:
        This is <div c='ff0000'/> colored text <div/>supports  <div c='00ff00AA'/>nested tags</div><br/> with broken line.
        */
        void setHtmlText(const QString & str);

        virtual bool isOn(const Vector2& localPosition, float) override;
        virtual void doRender(const RenderState&) override;
        text::Node* getRootNode();

        using TweenColor = Property<QColor, const QColor&, TextField, &TextField::getStyleColor, &TextField::setStyleColor>;
    protected:
        virtual void sizeChanged(const Vector2& size) override;
        void rebuildText(bool lock = true);

    protected:
#ifdef GRAPHICSUPPORT
        QString  m_text;
        TextStyle m_style;

        text::spNode m_root;
        Rect m_textRect{0, 0, 0, 0};
        bool m_htmlText = false;
#else
        static TextStyle m_dummyTextStyle;
        static Rect m_dummyRect;
        static QString m_dummyText;
        static QColor m_dummyColor;
#endif
    };
}
