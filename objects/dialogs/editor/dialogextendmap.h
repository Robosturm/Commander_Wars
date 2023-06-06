#ifndef DIALOGEXTENDMAP_H
#define DIALOGEXTENDMAP_H

#include "game/GameEnums.h"
#include "ui_reader/createdgui.h"

class DialogExtendMap;
using spDialogExtendMap = oxygine::intrusive_ptr<DialogExtendMap>;

class DialogExtendMap final : public CreatedGui
{
    Q_OBJECT
public:
    explicit DialogExtendMap();
    ~DialogExtendMap() = default;
    Q_INVOKABLE QString getMapFile() const;
    Q_INVOKABLE void setMapFile(const QString newMapFile);
    Q_INVOKABLE GameEnums::Directions getDirection() const;
    Q_INVOKABLE void setDirection(const GameEnums::Directions newDirection);
    Q_INVOKABLE void cancel();
    Q_INVOKABLE void mapFileChanged(QString file);
    Q_INVOKABLE void showSelectMap();
    Q_INVOKABLE void onOk();
signals:
    void sigOk(const QString mapFile, GameEnums::Directions direction);
    void sigCanceled();
    void sigRemove();

private slots:
    void remove();
private:
    QString m_mapFile;
    GameEnums::Directions m_direction{GameEnums::Directions_North};
};

#endif // DIALOGEXTENDMAP_H
