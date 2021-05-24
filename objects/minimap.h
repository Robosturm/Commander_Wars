#ifndef MINIMAP_H
#define MINIMAP_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

class GameMap;
typedef oxygine::intrusive_ptr<GameMap> spGameMap;

class Minimap;
typedef oxygine::intrusive_ptr<Minimap> spMinimap;

class Minimap : public QObject, public oxygine::Actor
{
        Q_OBJECT
    public:
        static const qint32 IMAGE_SIZE{4};
        explicit Minimap();
        virtual ~Minimap();

        void clear();
    signals:
        void clicked(qint32 x, qint32 y, bool updateMinimapPosition);
    public slots:
        void updateMinimap(spGameMap pMap, bool useVision = false);
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

};

#endif // MINIMAP_H
