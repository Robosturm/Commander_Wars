#ifndef MAPEDITDIALOG_H
#define MAPEDITDIALOG_H

#include <QObject>

#include "ui_reader/createdgui.h"
#include "game/GameEnums.h"

class MapEditDialog;
using spMapEditDialog = oxygine::intrusive_ptr<MapEditDialog>;

class MapEditDialog final : public CreatedGui
{
    Q_OBJECT
public:
    struct MapEditInfo
    {
        QString mapName;
        QString author;
        QString description;
        QString scriptFile;
        qint32 mapWidth{20};
        qint32 mapHeigth{20};
        qint32 playerCount{4};
        qint32 turnLimit{0};
        quint32 deployLimit{0};
        GameEnums::MapFilterFlags mapFlags{GameEnums::MapFilterFlags_None};
    };
    explicit MapEditDialog(MapEditInfo info);
    ~MapEditDialog();

signals:
    void editFinished(MapEditDialog::MapEditInfo info);
    void sigCanceled();
public slots:
    void showSelectScript();

    void setMapName(QString name);
    QString getMapName() const;
    void setMapAuthor(QString name);
    QString getMapAuthor() const;
    void setMapDescription(QString name);
    QString getMapDescription() const;
    void setMapScript(QString name);
    QString getMapScript() const;
    void setMapWidth(qint32 value);
    qint32 getMapWidth() const;
    void setMapHeight(qint32 value);
    qint32 getMapHeight() const;
    void setMapPlayerCount(qint32 value);
    qint32 getMapPlayerCount() const;
    void setMapTurnLimit(qint32 value);
    qint32 getMapTurnLimit() const;
    void setMapDeployLimit(qint32 value);
    qint32 getMapDeployLimit() const;
    void setMapFlag(GameEnums::MapFilterFlags flag, bool value);
    bool getMapFlag(GameEnums::MapFilterFlags flag) const;

    void cancel();
    void finished();
private slots:
    void scriptFileChanged(QString file);
    void remove();
private:
    MapEditInfo m_info;
};

Q_DECLARE_INTERFACE(MapEditDialog, "MapEditDialog");

#endif // MAPEDITDIALOG_H
