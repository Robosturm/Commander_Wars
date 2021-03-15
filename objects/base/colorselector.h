#ifndef COLORSELECTOR_H
#define COLORSELECTOR_H

#include <QObject>

#include <qcolor.h>

#include <qvector.h>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/spinbox.h"

class ColorSelector;
typedef oxygine::intrusive_ptr<ColorSelector> spColorSelector;

class ColorSelector : public QObject, public oxygine::Actor
{
    Q_OBJECT
    static constexpr qint32 pixelCount = 4;
public:
    explicit ColorSelector(QColor color, qint32 pixelSize);

    QColor getCurrentColor() const;

signals:
    void sigSelecetedColorChanged(QColor color);
public slots:
    void selecetedColorChanged(QColor color);
private:
    oxygine::spActor m_ColorDialog;
    spSpinBox m_SpinBoxRed;
    spSpinBox m_SpinBoxGreen;
    spSpinBox m_SpinBoxBlue;
    QColor m_CurrentColor;
    QVector<QVector<oxygine::spColorRectSprite>> m_RedGreenField;
    QVector<oxygine::spColorRectSprite> m_BlueField;
    oxygine::spSprite m_Cursor1;
    oxygine::spSprite m_Cursor2;
    qint32 pixelSize;
    bool m_barUpdating{false};
    bool m_boxUpdating{false};
};

#endif // COLORSELECTOR_H
