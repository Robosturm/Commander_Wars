#ifndef MAPEDITDIALOG_H
#define MAPEDITDIALOG_H

#include <QObject>

#include "ui_reader/createdgui.h"
#include "game/GameEnums.h"

class MapEditDialog;
using spMapEditDialog = std::shared_ptr<MapEditDialog>;

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
    explicit MapEditDialog(MapEditInfo info, const QString & confirmMessage);
    virtual ~MapEditDialog();
    Q_INVOKABLE void setMapName(QString name);
    Q_INVOKABLE QString getMapName() const;
    Q_INVOKABLE void setMapAuthor(QString name);
    Q_INVOKABLE QString getMapAuthor() const;
    Q_INVOKABLE void setMapDescription(QString name);
    Q_INVOKABLE QString getMapDescription() const;
    Q_INVOKABLE void setMapScript(QString name);
    Q_INVOKABLE QString getMapScript() const;
    Q_INVOKABLE void setMapWidth(qint32 value);
    Q_INVOKABLE qint32 getMapWidth() const;
    Q_INVOKABLE void setMapHeight(qint32 value);
    Q_INVOKABLE qint32 getMapHeight() const;
    Q_INVOKABLE void setMapPlayerCount(qint32 value);
    Q_INVOKABLE qint32 getMapPlayerCount() const;
    Q_INVOKABLE void setMapTurnLimit(qint32 value);
    Q_INVOKABLE qint32 getMapTurnLimit() const;
    Q_INVOKABLE void setMapDeployLimit(qint32 value);
    Q_INVOKABLE qint32 getMapDeployLimit() const;
    Q_INVOKABLE void setMapFlag(GameEnums::MapFilterFlags flag, bool value);
    Q_INVOKABLE bool getMapFlag(GameEnums::MapFilterFlags flag) const;
    Q_INVOKABLE void cancel();
    Q_INVOKABLE void finished();
    Q_INVOKABLE void showSelectScript();
signals:
    void editFinished(MapEditDialog::MapEditInfo info);
    void sigCanceled();
public slots:
    void onConfirm();

private slots:
    void scriptFileChanged(QString file);
    void remove();
private:
    MapEditInfo m_info;
    QString m_confirmMessage;
};

Q_DECLARE_INTERFACE(MapEditDialog, "MapEditDialog");

#endif // MAPEDITDIALOG_H
