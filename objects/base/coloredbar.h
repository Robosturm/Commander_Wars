#ifndef COLOREDBAR_H
#define COLOREDBAR_H

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.h"
#include "objects/base/label.h"
#include <QObject>

class ColoredBar;
using spColoredbar = std::shared_ptr<ColoredBar>;

class ColoredBar : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    ColoredBar(qint32 width, qint32 height, const QString & backSpriteId, const QString & foreSpriteId);

    Q_INVOKABLE void changeBar(float divider, QColor leftColor, QColor rightColor);
    Q_INVOKABLE void setText(const QString & text);
    Q_INVOKABLE Label* getLabel();
    Q_INVOKABLE void setIsVisible(bool vis);
    Q_INVOKABLE void setPosX(qint32 x);
    Q_INVOKABLE void setPosY(qint32 y);
private:
    oxygine::spSprite m_background;
    oxygine::spColorRectSprite m_leftBar;
    oxygine::spColorRectSprite m_rightBar;
    oxygine::spSprite m_foreground;
    spLabel m_label;
};

Q_DECLARE_INTERFACE(ColoredBar, "ColoredBar");

#endif // COLOREDBAR_H
