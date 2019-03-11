#ifndef MAPSELECTION_H
#define MAPSELECTION_H

#include <QObject>

#include <QVector>

#include "oxygine-framework.h"

class MapSelection;
typedef oxygine::intrusive_ptr<MapSelection> spMapSelection;

class MapSelection : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit MapSelection(qint32 heigth, qint32 width, QString folder);
    virtual ~MapSelection();

    QString getCurrentFolder()
    {
        return m_currentFolder;
    }
signals:
    void changeSelection(qint32 index);
    void itemClicked(QString item);
    void itemChanged(QString item);
public slots:
    void changeFolder(QString folder);
    void updateSelection(qint32 startIndex);
private:
    QString m_currentFolder;
    QString currentItem;
    qint32 currentIdx{0};
    QVector<oxygine::spTextField> m_Items;
    qint32 itemCount{0};
    static const qint32 itemHeigth{35};
    QStringList files;
    qint32 currentStartIndex{0};
    oxygine::spBox9Sprite m_SelectedItem;

};

#endif // MAPSELECTION_H
