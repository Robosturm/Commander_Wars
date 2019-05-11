#ifndef DIALOGUSERNAME_H
#define DIALOGUSERNAME_H

#include <QObject>

#include "oxygine-framework.h"

#include "objects/textbox.h"

class DialogUsername : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogUsername();
    virtual ~DialogUsername() = default;

signals:

public slots:

private:
    oxygine::spButton m_OkButton;
    spTextbox m_pTextbox;
};

#endif // DIALOGUSERNAME_H
