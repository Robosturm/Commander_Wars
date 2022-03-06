#ifndef MOVEINBUTTON_H
#define MOVEINBUTTON_H


#include <QObject>
#include "3rd_party/oxygine-framework/oxygine-framework.h"

class MoveInButton;
using spMoveInButton = oxygine::intrusive_ptr<MoveInButton>;

class MoveInButton : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit MoveInButton(oxygine::Actor* pParent, qint32 moveInSize, qint32 direction = -1,
                          qint32 startOffset = -1, float buttonScale = 2.0f, bool useY = false);
    virtual ~MoveInButton() = default;
    bool getMovedOut() const;

signals:
    void sigMoved();
private:
    void addMoveTween();

private:
    oxygine::Actor* m_pParent{nullptr};
    qint32 m_moveInSize{0};
    qint32 m_direction{-1};
    bool m_finished{true};
    bool m_movedOut{false};
    bool m_useY{false};
    oxygine::spButton m_pButton;
};

#endif
