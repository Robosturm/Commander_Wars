#ifndef MAPEDITDIALOG_H
#define MAPEDITDIALOG_H

#include <QObject>

#include "oxygine-framework.h"

#include "objects/panel.h"

#include "objects/textbox.h"

#include "objects/spinbox.h"

class MapEditDialog;
typedef oxygine::intrusive_ptr<MapEditDialog> spMapEditDialog;


class MapEditDialog : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit MapEditDialog(QString mapName, qint32 mapWidth, qint32 mapHeigth);
    virtual ~MapEditDialog() = default;

signals:

public slots:

private:
    oxygine::spButton m_OkButton;
    spTextbox m_MapName;
    spSpinBox m_MapWidth;

};

#endif // MAPEDITDIALOG_H
