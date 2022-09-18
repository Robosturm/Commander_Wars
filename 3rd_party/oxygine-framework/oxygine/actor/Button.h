#pragma once
#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"

namespace oxygine
{
    class Button;
    using spButton = intrusive_ptr<Button>;
    class Button : public QObject, public Box9Sprite
    {
        Q_OBJECT
    public:

        explicit Button();
        virtual ~Button() = default;

        qint32 getRow() const
        {
            return m_row;
        }
        virtual void setResAnim(const ResAnim* r, qint32  col = 0, qint32 row = 0) override;
        /**Sets which row from ResAnim should be used. Default value is 0. Could be used for CheckBoxes*/
        void setRow(qint32 row);

    protected:
        enum state
        {
            stateNormal,
            stateOvered,
            statePressed
        };
        void updateButtonState(state s);
    protected:
        state m_state;
        const ResAnim* m_resAnim;
        qint32 m_row;
    private:
        void _mouseEvent(Event* event);
        void setState(state s);

    private:
        pointer_index m_btnPressed;
        pointer_index m_btnOvered;
        qint32 m_touchUpCbId{0};
    };
}
