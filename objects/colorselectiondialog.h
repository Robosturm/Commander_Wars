#ifndef COLORSELECTIONDIALOG_H
#define COLORSELECTIONDIALOG_H

#include <QObject>

#include <QColor>
#include <QVector>

#include "oxygine-framework.h"

#include "objects/panel.h"

#include "objects/spinbox.h"

class ColorSelectionDialog;
typedef oxygine::intrusive_ptr<ColorSelectionDialog> spColorSelectionDialog;

class ColorSelectionDialog : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit ColorSelectionDialog(QColor color);

    virtual ~ColorSelectionDialog() = default;
signals:
    void editFinished(QColor color);
    void canceled();
    void sigSelecetedColorChanged(QColor color);
public slots:
    void selecetedColorChanged(QColor color);
private:
    oxygine::spButton m_OkButton;
    oxygine::spButton m_ExitButton;
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
};

#endif // COLORSELECTIONDIALOG_H
