#ifndef DIALOGCOSTYLE_H
#define DIALOGCOSTYLE_H

#include <QObject>

#include "oxygine-framework.h"

class DialogCOStyle;
typedef oxygine::intrusive_ptr<DialogCOStyle> spDialogCOStyle;

class DialogCOStyle : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogCOStyle(QString coid);
    virtual ~DialogCOStyle() = default;
signals:
    void sigFinished();
    void sigCancel();
public slots:
};

#endif // DIALOGCOSTYLE_H
