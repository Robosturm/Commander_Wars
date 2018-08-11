#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QObject>
#include "oxygine-framework.h"

class FileDialog : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit FileDialog();

signals:

public slots:
};

#endif // FILEDIALOG_H
