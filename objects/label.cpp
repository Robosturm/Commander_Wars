#include "label.h"

Label::Label(qint32 width, qint32 heigth)
{
    _clipRect = new oxygine::ClipRectActor();
    _clipRect->setSize(width, heigth);
    _textField = new oxygine::TextField();
    _clipRect->addChild(_textField);
    addChild(_clipRect);
}

void Label::setText(QString str)
{
    _textField->setHtmlText(str);
    setTooltipText(str);
}

void Label::setHtmlText(QString str)
{
    _textField->setHtmlText(str);
    setTooltipText(str);
}

void Label::setStyle(const oxygine::TextStyle& st)
{
    _textField->setStyle(st);
}
