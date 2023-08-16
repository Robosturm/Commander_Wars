#ifndef DIALOGMODIFYTERRAIN_H
#define DIALOGMODIFYTERRAIN_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"

#include "objects/base/panel.h"
#include "objects/base/textbox.h"

class GameMap;
class Terrain;
class DialogModifyTerrain;
using spDialogModifyTerrain = std::shared_ptr<DialogModifyTerrain>;

class DialogModifyTerrain final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogModifyTerrain(GameMap* pMap, Terrain* pTerrain);
   virtual ~DialogModifyTerrain() = default;
signals:
    void sigFinished();
    void sigTerrainClicked(QString id);
    void sigShowLoadDialog();
    void sigOverlayChanged(QString id, bool selected);
    void sigChangePalette(const QString newPalette);
    void sigLoadCustomSprite(QString id);
public slots:
    void terrainClicked(QString id);
    void showLoadDialog();
    void loadCustomSprite(QString id);
    void remove();
    void overlayChanged(QString id, bool selected);
    void changePalette(const QString newPalette);
private:
    void loadBaseImageview(qint32 & y, Terrain* pTerrain);
    void loadOverlayview(qint32 & y, Terrain* pTerrain);
    void load();
private:
    Terrain* m_pTerrain{nullptr};
    spPanel m_pPanel;
    spTextbox m_pTextbox;
    oxygine::spButton m_OkButton;
    GameMap* m_pMap{nullptr};
    qint32 m_selectedPalette{-1};
    bool m_changed{false};

};

#endif // DIALOGMODIFYTERRAIN_H
