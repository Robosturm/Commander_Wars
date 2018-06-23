#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <QObject>
#include <QVector>

#include "oxygine-framework.h"

#include "game/terrain.h"

class GameMap : public QObject, public oxygine::Actor
{
    Q_OBJECT

    Q_PROPERTY(QVector<QVector<Terrain*>*> fields READ getFields)
public:
    explicit GameMap(qint32 width, qint32 heigth);
    explicit GameMap(QString map);
    virtual ~GameMap();

    QVector<QVector<Terrain *> *> getFields() const;

signals:

public slots:

private:
    QVector<QVector<Terrain*>*> fields;

    static const QString m_JavascriptName;

};

#endif // GAMEMAP_H
