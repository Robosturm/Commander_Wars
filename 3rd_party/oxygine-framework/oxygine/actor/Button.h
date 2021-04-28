#pragma once
#include "../oxygine-include.h"
#include "Sprite.h"

namespace oxygine
{
    DECLARE_SMART(Button, spButton);

    class Button: public Sprite
    {
    public:

        Button();
        ~Button();

        qint32 getRow() const {return _row;}

        void setResAnim(const ResAnim* r, qint32  col = 0, qint32 row = 0) override;

        /**Sets which row from ResAnim should be used. Default value is 0. Could be used for CheckBoxes*/
        void setRow(qint32 row);

    protected:
        enum state
        {
            stateNormal,
            stateOvered,
            statePressed
        };
        virtual void updateButtonState(state s);

        state _state;
        const ResAnim* _resAnim;
        qint32 _row;

    private:
        pointer_index _btnPressed;
        pointer_index _btnOvered;

        void _mouseEvent(Event* event);

        void setState(state s);

        qint32 m_touchUpCbId{0};
    };
}
