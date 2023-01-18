#pragma once

#include <QObject>

#include "mapsupport/mapfilter.h"
#include "ui_reader/createdgui.h"

class MapSelectionFilterDialog;
using spMapSelectionFilterDialog = oxygine::intrusive_ptr<MapSelectionFilterDialog>;

class MapSelectionFilterDialog final : public CreatedGui
{
    Q_OBJECT
public:
    explicit MapSelectionFilterDialog(MapFilter* filter);
    ~MapSelectionFilterDialog();
public slots:
    void exit();
    void addToFilter(GameEnums::MapFilterFlags flag, bool active, bool isOptional);
    void removeFromFilter(GameEnums::MapFilterFlags flag);
    bool getFlagActive(GameEnums::MapFilterFlags flag) const;
    void setFlagActive(GameEnums::MapFilterFlags flag, bool isActive);
    bool getFlagOptional(GameEnums::MapFilterFlags flag) const;
    void setFlagOptional(GameEnums::MapFilterFlags flag, bool isOptional);

    void setMinHeight(qint32 value);
    qint32 getMinHeight() const;
    void setMaxHeight(qint32 value);
    qint32 getMaxHeight() const;
    void setMinWidth(qint32 value);
    qint32 getMinWidth() const;
    void setMaxWidth(qint32 value);
    qint32 getMaxWidth() const;
    void setMinPlayer(qint32 value);
    qint32 getMinPlayer() const;
    void setMaxPlayer(qint32 value);
    qint32 getMaxPlayer() const;
    QString getMapAuthor() const;
    void setMapAuthor(const QString &newMapAuthor);
    QString getMapName() const;
    void setMapName(const QString &newMapName);

private slots:
    void remove();
private:
    MapFilter* m_mapFilter;
};

Q_DECLARE_INTERFACE(MapSelectionFilterDialog, "MapSelectionFilterDialog");
