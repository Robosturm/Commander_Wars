#ifndef MINIMAP_H
#define MINIMAP_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"

class GameMap;
class GameMenue;
class Minimap;
using spMinimap = std::shared_ptr<Minimap>;

class Minimap final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    static const qint32 IMAGE_SIZE{4};
    explicit Minimap();
    ~Minimap() = default;

    void clear();    
    void setMenu(GameMenue* pMenu);

signals:
    void clicked(qint32 x, qint32 y, bool updateMinimapPosition);
public slots:
    void updateMinimap(const GameMap* pMap, bool useVision = false);
private:
    struct MinimapFieldInfo
    {
        QString terrainId;
        qint32 terrainOwner;
        QString unitId;
        qint32 unitPlayer;
        oxygine::spSprite background;
        oxygine::spSprite unit;
    };
    QVector<MinimapFieldInfo> m_Items;
    GameMenue* m_pGamemenu{nullptr};
};

#endif // MINIMAP_H
