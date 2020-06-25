#include "label.h"

Label::Label(qint32 width)
{
    _clipRect = new oxygine::ClipRectActor();
    _clipRect->setWidth(width);
    _clipRect->setHeight(28);
    setWidth(width);
    _textField = new oxygine::TextField();
    _clipRect->addChild(_textField);
    addChild(_clipRect);
}

void Label::setWidth(qint32 width)
{
    _clipRect->setWidth(width);
    oxygine::Sprite::setWidth(width);
}

void Label::setText(QString str)
{
    _textField->setHtmlText(str);
    setTooltipText(str);
}

void Label::setHtmlText(QString str)
{
    _textField->setHtmlText(str);
    _clipRect->setHeight(getTextRect().getHeight() * 1.4f);
    setTooltipText(str);
}

void Label::setStyle(const oxygine::TextStyle& st)
{
    _textField->setStyle(st);
    _clipRect->setHeight(getTextRect().getHeight() * 1.4f);
}
