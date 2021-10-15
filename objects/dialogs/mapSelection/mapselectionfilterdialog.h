#pragma once

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "mapsupport/mapfilter.h"
#include "ui_reader/createdgui.h"

class MapSelectionFilterDialog;
using spMapSelectionFilterDialog = oxygine::intrusive_ptr<MapSelectionFilterDialog>;

class MapSelectionFilterDialog : public CreatedGui
{
    Q_OBJECT
public:
    explicit MapSelectionFilterDialog(MapFilter & filter);
    virtual ~MapSelectionFilterDialog();
signals:
    void sigFinished();
public slots:
    void exit();
    void addToFilter(GameEnums::MapFilterFlags flag, bool isOptional);
    void removeFromFilter(GameEnums::MapFilterFlags flag);
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
    const QString &getMapAuthor() const;
    void setMapAuthor(const QString &newMapAuthor);
    const QString &getMapName() const;
    void setMapName(const QString &newMapName);

private slots:
    void remove();
private:
    MapFilter & m_mapFilter;
};

