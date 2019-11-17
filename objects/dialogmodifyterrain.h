#ifndef DIALOGMODIFYTERRAIN_H
#define DIALOGMODIFYTERRAIN_H

#include <QObject>

#include "oxygine-framework.h"

#include "objects/panel.h"

class Terrain;

class DialogModifyTerrain;
typedef oxygine::intrusive_ptr<DialogModifyTerrain> spDialogModifyTerrain;

class DialogModifyTerrain : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogModifyTerrain(Terrain* pTerrain);

signals:
    void sigFinished();
public slots:

private:
    Terrain* m_pTerrain{nullptr};
    spPanel m_pPanel;
    oxygine::spButton m_OkButton;
};

#endif // DIALOGMODIFYTERRAIN_H
