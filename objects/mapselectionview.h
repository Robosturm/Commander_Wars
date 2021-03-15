#ifndef MAPSELECTIONVIEW_H
#define MAPSELECTIONVIEW_H

#include <QObject>

#include <qfileinfo.h>

#include "objects/base/panel.h"


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/mapselection.h"

#include "objects/minimap.h"

#include "game/campaign.h"
#include "game/gamemap.h"
#include "game/building.h"

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
        return m_pCurrentMap.get();
    }
    inline void setCurrentMap(GameMap* pMap)
    {
        m_pCurrentMap = pMap;
    }
    inline QFileInfo getCurrentFile()
    {
        return m_currentMapFile;
    }
    inline void setCurrentFile(QString info)
    {
        m_currentMapFile = QFileInfo(info);
    }
    inline spCampaign getCurrentCampaign()
    {
        return m_CurrentCampaign;
    }
    inline void setCurrentCampaign(spCampaign pCampaign)
    {
        m_CurrentCampaign = pCampaign;
    }
    spMapSelection getMapSelection() const;

signals:

public slots:
    void loadMap(QFileInfo info, bool fast = true);
    void loadCurrentMap();
    void updateMapData();
private:
    /**
     * @brief loadMapVictoryInfo
     */
    void loadMapVictoryInfo();

private:
    spCampaign m_CurrentCampaign;
    /**
      * @brief m_currentMapFile stores the current map info
      */
    QFileInfo m_currentMapFile;
    spGameMap m_pCurrentMap{nullptr};
    // map selection
    spMapSelection m_pMapSelection;
    spPanel m_MapInfo;
    oxygine::spTextField m_MapName;
    oxygine::spTextField m_MapAuthor;
    oxygine::spTextField m_MapPlayerCount;
    oxygine::spTextField m_MapDescription;
    oxygine::spActor m_pVictoryInfo;
    spMinimap m_pMinimap;
    spPanel m_MinimapPanel;
    QVector<spBuilding> m_BuildingCountSprites;
    QVector<oxygine::spTextField> m_BuildingCountTexts;
    oxygine::spBox9Sprite m_pBuildingBackground;
    oxygine::spActor m_content;
    oxygine::spSlidingActor m_contentSlider;
};

#endif // MAPSELECTIONVIEW_H
