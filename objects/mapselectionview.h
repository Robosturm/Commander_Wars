#ifndef MAPSELECTIONVIEW_H
#define MAPSELECTIONVIEW_H

#include <QObject>

#include <qfileinfo.h>

#include "objects/panel.h"

#include "oxygine-framework.h"

#include "objects/mapselection.h"

#include "objects/minimap.h"

#include "game/campaign.h"

class GameMap;

class MapSelectionView;
typedef oxygine::intrusive_ptr<MapSelectionView> spMapSelectionView;

class MapSelectionView : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit MapSelectionView();

    inline MapSelection* getMapSelection()
    {
        return m_pMapSelection.get();
    }
    inline GameMap* getCurrentMap()
    {
        return m_pCurrentMap;
    }
    inline void setCurrentMap(GameMap* pMap)
    {
        m_pCurrentMap = pMap;
    }
    inline QFileInfo getCurrentFile()
    {
        return m_currentMapFile;
    }
    inline spCampaign getCurrentCampaign()
    {
        return m_CurrentCampaign;
    }
    inline void setCurrentCampaign(spCampaign pCampaign)
    {
        m_CurrentCampaign = pCampaign;
    }
signals:

public slots:
    void loadMap(QFileInfo info);

    void updateMapData();
private:
    spCampaign m_CurrentCampaign;
    /**
      * @brief m_currentMapFile stores the current map info
      */
    QFileInfo m_currentMapFile;
    GameMap* m_pCurrentMap{nullptr};
    // map selection
    spMapSelection m_pMapSelection;
    spPanel m_MapInfo;
    oxygine::spTextField m_MapName;
    oxygine::spTextField m_MapAuthor;
    oxygine::spTextField m_MapDescription;
    spMinimap m_pMinimap;
    spPanel m_MinimapPanel;
    QVector<oxygine::spTextField> m_BuildingCountTexts;
    oxygine::spBox9Sprite m_pBuildingBackground;
};

#endif // MAPSELECTIONVIEW_H
