#ifndef INGAMEINFOBAR_H
#define INGAMEINFOBAR_H

#include <QObject>

class IngameInfoBar : public QObject
{
    Q_OBJECT
public:
    explicit IngameInfoBar(QObject *parent = nullptr);

signals:

public slots:
};

#endif // INGAMEINFOBAR_H