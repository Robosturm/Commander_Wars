#pragma once

#include <QObject>

class BaseTableItem : public QObject
{
    Q_OBJECT
public:
    explicit BaseTableItem(QObject *parent = nullptr);

signals:

};

