#ifndef KO_H
#define KO_H

#include <QObject>


class KO : public QObject
{
    Q_OBJECT
public:
    explicit KO(QObject *parent = nullptr);

signals:

public slots:
};

#endif // KO_H
