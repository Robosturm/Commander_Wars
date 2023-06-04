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

signals:
    void sigOk(const QString mapFile, GameEnums::Directions direction);
    void sigCanceled();
    void sigRemove();
public slots:
    void onOk();

    QString getMapFile() const;
    void setMapFile(const QString & newMapFile);

    GameEnums::Directions getDirection() const;
    void setDirection(const GameEnums::Directions & newDirection);

    void cancel();
    void mapFileChanged(QString file);
    void showSelectMap();
private slots:
    void remove();
private:
    QString m_mapFile;
    GameEnums::Directions m_direction{GameEnums::Directions_North};
};

#endif // DIALOGEXTENDMAP_H
