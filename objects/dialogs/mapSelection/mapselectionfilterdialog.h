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
    Q_INVOKABLE bool getFlagActive(GameEnums::MapFilterFlags flag) const;
    Q_INVOKABLE void setFlagActive(GameEnums::MapFilterFlags flag, bool isActive);
    Q_INVOKABLE bool getFlagOptional(GameEnums::MapFilterFlags flag) const;
    Q_INVOKABLE void setFlagOptional(GameEnums::MapFilterFlags flag, bool isOptional);
    Q_INVOKABLE void setMinHeight(qint32 value);
    Q_INVOKABLE qint32 getMinHeight() const;
    Q_INVOKABLE void setMaxHeight(qint32 value);
    Q_INVOKABLE qint32 getMaxHeight() const;
    Q_INVOKABLE void setMinWidth(qint32 value);
    Q_INVOKABLE qint32 getMinWidth() const;
    Q_INVOKABLE void setMaxWidth(qint32 value);
    Q_INVOKABLE qint32 getMaxWidth() const;
    Q_INVOKABLE void setMinPlayer(qint32 value);
    Q_INVOKABLE qint32 getMinPlayer() const;
    Q_INVOKABLE void setMaxPlayer(qint32 value);
    Q_INVOKABLE qint32 getMaxPlayer() const;
    Q_INVOKABLE QString getMapAuthor() const;
    Q_INVOKABLE void setMapAuthor(const QString newMapAuthor);
    Q_INVOKABLE QString getMapName() const;
    Q_INVOKABLE void setMapName(const QString newMapName);
    Q_INVOKABLE void exit();
    Q_INVOKABLE void addToFilter(GameEnums::MapFilterFlags flag, bool active, bool isOptional);
    Q_INVOKABLE void removeFromFilter(GameEnums::MapFilterFlags flag);

private slots:
    void remove();
private:
    MapFilter* m_mapFilter;
};

Q_DECLARE_INTERFACE(MapSelectionFilterDialog, "MapSelectionFilterDialog");
