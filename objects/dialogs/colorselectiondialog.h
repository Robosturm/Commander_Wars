#ifndef COLORSELECTIONDIALOG_H
#define COLORSELECTIONDIALOG_H

#include <QObject>
#include <QColor>
#include <QVector>

#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"

#include "game/unit.h"
#include "game/player.h"
#include "game/building.h"

#include "objects/base/colorselector.h"

class ColorSelectionDialog;
using spColorSelectionDialog = std::shared_ptr<ColorSelectionDialog>;

class ColorSelectionDialog final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit ColorSelectionDialog(QColor color, bool showUnitPreview = false);
    ~ColorSelectionDialog() = default;
signals:
    void editFinished(QColor color);
    void canceled();
    void sigFinished();
private slots:
    void remove();
    void selecetedColorChanged(QColor color);
private:
    oxygine::spButton m_OkButton;
    oxygine::spButton m_ExitButton;
    spColorSelector m_pColorSelector;
    spPlayer m_pPlayer;
    spBuilding m_pBuilding;
    spUnit m_pUnit;
};

#endif // COLORSELECTIONDIALOG_H
