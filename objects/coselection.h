#ifndef COSELECTION_H
#define COSELECTION_H

#include <QObject>

#include <QColor>

#include "oxygine-framework.h"

class COSelection : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit COSelection();
    virtual ~COSelection();

signals:
    void coSelected(QString coid);
public slots:
    void colorChanged(QColor color);
};

#endif // COSELECTION_H
