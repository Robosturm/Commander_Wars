#ifndef DIALOGMODIFYTERRAIN_H
#define DIALOGMODIFYTERRAIN_H

#include <QObject>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/panel.h"

#include "objects/base/textbox.h"

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
    void sigTerrainClicked(QString id);
    void sigShowLoadDialog();
public slots:
    void terrainClicked(QString id);
    void showLoadDialog();
    void loadCustomSprite(QString id);
private:
    Terrain* m_pTerrain{nullptr};
    spPanel m_pPanel;
    spTextbox m_pTextbox;
    oxygine::spButton m_OkButton;

};

#endif // DIALOGMODIFYTERRAIN_H
