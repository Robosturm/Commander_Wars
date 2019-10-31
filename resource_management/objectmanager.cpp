#include "objectmanager.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

ObjectManager* ObjectManager::m_pInstance = nullptr;

ObjectManager* ObjectManager::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new ObjectManager();
    }
    return m_pInstance;
}

ObjectManager::ObjectManager()
{
    oxygine::Resources::loadXML("resources/objects/res.xml");
    oxygine::Resources::loadXML("resources/cursor/res.xml");

}

oxygine::spButton ObjectManager::createButton(QString text, qint32 width)
{
    oxygine::spButton pButton = new oxygine::Button();
    // pButton->setPosition(200, 200);
    pButton->setResAnim(ObjectManager::getInstance()->getResAnim("button"));
    pButton->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));


    //Create Actor with Text and add it to button as child
    oxygine::spTextField textField = new oxygine::TextField();
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = QColor(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_MIDDLE;
    style.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    style.multiline = false;
    textField->setStyle(style);
    textField->setHtmlText(text);
    textField->attachTo(pButton);
    if (width < 0)
    {
        width = textField->getTextRect().getWidth();
        if (width < 150)
        {
            width = 150;
        }
    }
    else
    {
        width -= 30;
    }
    pButton->setSize(width + 30, 40);
    textField->setSize(pButton->getSize());

    oxygine::Sprite* ptr = pButton.get();
    pButton->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });

    pButton->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    pButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        Mainapp::getInstance()->getAudioThread()->playSound("button.wav");
    });
    return pButton;
}
