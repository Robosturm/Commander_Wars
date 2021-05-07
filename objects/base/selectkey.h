#ifndef SELECTKEY_H
#define SELECTKEY_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "3rd_party/oxygine-framework/oxygine/KeyEvent.h"
#include "objects/base/tooltip.h"

class SelectKey;
typedef oxygine::intrusive_ptr<SelectKey> spSelectKey;

class SelectKey : public Tooltip
{
    Q_OBJECT
public:
    explicit SelectKey(Qt::Key code);
    virtual ~SelectKey() = default;

    static QString getKeycodeText(Qt::Key code);
signals:
    void sigKeyChanged(Qt::Key key);

public slots:
    void keyInput(oxygine::KeyEvent event);
    virtual void focusedLost() override;
protected:
    void setKeycode(Qt::Key code);
    virtual void focused() override;
private:
    oxygine::spButton m_Button;
    Qt::Key m_currentCode{Qt::Key_Space};
    bool m_active{false};

};

#endif // SELECTKEY_H
