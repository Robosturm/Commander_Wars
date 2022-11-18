#include "objects/base/checkbox.h"

#include "resource_management/objectmanager.h"

#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"
#include "coreengine/audiothread.h"

Checkbox::Checkbox()
{
#ifdef GRAPHICSUPPORT
    setObjectName("Checkbox");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("checkbox");
    if (pAnim != nullptr)
    {
        setResAnim(pAnim);
        addEventListener(oxygine::TouchEvent::OVER, [this, pAnim](oxygine::Event*)
        {
            addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
            if (m_Checked)
            {
                setAnimFrame(pAnim, 2);
            }
            else
            {
                setAnimFrame(pAnim, 1);
            }
        });

        addEventListener(oxygine::TouchEvent::OUTX, [this, pAnim](oxygine::Event*)
        {
            addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
            if (m_Checked)
            {
                setAnimFrame(pAnim, 3);
            }
            else
            {
                setAnimFrame(pAnim, 0);
            }
        });

        addEventListener(oxygine::TouchEvent::CLICK, [this, pAnim](oxygine::Event*)
        {
            m_Checked = !m_Checked;
            if (m_Checked)
            {
                setAnimFrame(pAnim, 2);
            }
            else
            {
                setAnimFrame(pAnim, 1);
            }
            Mainapp::getInstance()->getAudioThread()->playSound("checkbox.wav");
            emit checkChanged(m_Checked);
        });
        setSize(pAnim->getWidth(), pAnim->getHeight());
    }
}

bool Checkbox::getChecked() const
{
    return m_Checked;
}

void Checkbox::setChecked(bool Checked)
{    
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("checkbox");
    m_Checked = Checked;
    if (m_Checked)
    {
        setAnimFrame(pAnim, 3);
    }
    else
    {
        setAnimFrame(pAnim, 0);
    }    
}
