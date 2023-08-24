#ifndef MAPSELECTIONVIEW_H
#define MAPSELECTIONVIEW_H

#include <QObject>

#include <qfileinfo.h>

#include "objects/base/panel.h"


#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/SlidingActor.h"

#include "objects/mapselection.h"

#include "objects/minimap.h"

#include "game/campaign.h"
#include "game/gamemap.h"
#include "game/building.h"


class MapSelectionView;
using spMapSelectionView = std::shared_ptr<MapSelectionView>;

class MapSelectionView final : public QObject, public oxygine::Actor, public FileSerializable
{
    Q_OBJECT
public:
    explicit MapSelectionView(QStringList filter, qint32 mapInfoHeight = -1);
    ~MapSelectionView() = default;
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& stream) const;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& stream);
    /**
     * @brief getVersion version of the file
     * @return
     */
    virtual qint32 getVersion() const
    {
        return 1;
    }

    inline MapSelection* getMapSelection()
    {
        return m_pMapSelection.get();
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
        return m_CurrentSetCampaign;
    }
    inline spCampaign getCurrentLoadedCampaign()
    {
        return m_CurrentLoadedCampaign;
    }
    inline void setCurrentSetCampaign(spCampaign pCampaign)
    {
        m_CurrentSetCampaign = pCampaign;
    }
    spMapSelection getMapSelection() const;

    const QFileInfo &getCurrentMapFile() const;

    spPanel getMinimapPanel() const;
    oxygine::spBox9Sprite getBuildingBackground() const;    
    spPanel getMapInfo() const;

    spGameMap getCurrentMap() const;
    void setCurrentMap(spGameMap newCurrentMap);
    /**
     * @brief getMinimap
     * @return
     */
    Minimap* getMinimap() const;

public slots:
    void loadMap(const QFileInfo & info, bool fast = true);
    void loadCurrentMap();
    void updateMapData();
private:
    /**
     * @brief loadMapVictoryInfo
     */
    void loadMapVictoryInfo();
private:
    QStringList m_filter;
    spCampaign m_CurrentSetCampaign;
    spCampaign m_CurrentLoadedCampaign;
    /**
      * @brief m_currentMapFile stores the current map info
      */
    QFileInfo m_currentMapFile;
    spGameMap m_pCurrentMap;
    // map selection
    spMapSelection m_pMapSelection;
    spPanel m_MapInfo;
    oxygine::spTextField m_MapName;
    oxygine::spTextField m_MapAuthor;
    oxygine::spTextField m_MapPlayerCount;
    oxygine::spTextField m_MapDescription;
    oxygine::spTextField m_MapTags;
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
