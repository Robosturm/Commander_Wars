#include "checkbox.h"

#include "resource_management/objectmanager.h"

#include "coreengine/mainapp.h"

Checkbox::Checkbox()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("checkbox");
    setResAnim(pAnim);
    addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(16, 16, 16, 0)), 300);
        if (m_Checked)
        {
            setAnimFrame(pAnim, 2);
        }
        else
        {
            setAnimFrame(pAnim, 1);
        }
    });

    addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(0, 0, 0, 0)), 300);
        if (m_Checked)
        {
            setAnimFrame(pAnim, 3);
        }
        else
        {
            setAnimFrame(pAnim, 0);
        }
    });

    addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
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
    this->setSize(pAnim->getWidth(), pAnim->getHeight());
}

bool Checkbox::getChecked() const
{
    return m_Checked;
}

void Checkbox::setChecked(bool Checked)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
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
    pApp->continueThread();
}
