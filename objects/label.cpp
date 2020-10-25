#include "label.h"

#include "coreengine/mainapp.h"

Label::Label(qint32 width)
{
    _clipRect = new oxygine::ClipRectActor();
    _clipRect->setWidth(width);
    _clipRect->setHeight(28);
    _textField = new oxygine::TextField();
    _textField->setWidth(width);
    setWidth(width);
    _clipRect->addChild(_textField);
    addChild(_clipRect);
}

void Label::setWidth(float width)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    _clipRect->setWidth(width);
    _textField->setWidth(width);
    oxygine::Sprite::setWidth(width);
    pApp->continueThread();
}

void Label::setText(QString str)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    _textField->setHtmlText(str);
    setTooltipText(str);
    pApp->continueThread();
}

void Label::setHtmlText(QString str)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    _textField->setHtmlText(str);
    _clipRect->setHeight(getTextRect().getHeight() * 1.4f);
    setTooltipText(str);
    pApp->continueThread();
}

void Label::setStyle(const oxygine::TextStyle& st)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    _textField->setStyle(st);
    _clipRect->setHeight(getTextRect().getHeight() * 1.4f);
    pApp->continueThread();
}
