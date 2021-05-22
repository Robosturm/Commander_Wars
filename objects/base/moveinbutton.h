#pragma once

#include <QObject>
#include "3rd_party/oxygine-framework/oxygine-framework.h"

class MoveInButton;
using spMoveInButton = oxygine::intrusive_ptr<MoveInButton>;

class MoveInButton : public QObject, public oxygine::Actor
{
        Q_OBJECT
    public:
        explicit MoveInButton(oxygine::Actor* pParent, qint32 moveInSize, qint32 direction = -1, qint32 startY = -1);

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
};

