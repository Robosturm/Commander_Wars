#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "coreengine/audiothread.h"
#include "coreengine/mainapp.h"
#include "objects/base/label.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"

ObjectManager::ObjectManager()
    : RessourceManagement<ObjectManager>("/objects/res.xml", "")
{
#ifdef GRAPHICSUPPORT
    setObjectName("ObjectManager");
#endif
    Interpreter::setCppOwnerShip(this);
    loadRessources("/cursor/res.xml");
}

oxygine::spButton ObjectManager::createButton(QString text, qint32 width, QString tooltip, QString resAnim)
{
    oxygine::spButton pButton = oxygine::spButton::create();
    pButton->setResAnim(ObjectManager::getInstance()->getResAnim(resAnim));
    pButton->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));

    //Create Actor with Text and add it to button as child
    spLabel textField = spLabel::create(width - 10);
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    textField->setStyle(style);
    textField->setHtmlText(text);
    if (width < 0)
    {
        width = textField->getTextRect().getWidth() + 10;
        if (!Settings::getSmallScreenDevice())
        {
            if (width < 180)
            {
                width = 180;
            }
        }
    }
    textField->setWidth(width - 10);
    textField->setPosition(width / 2 - textField->getTextRect().getWidth() / 2, 5);
    if (textField->getX() < 5)
    {
        textField->setX(5);
    }
    textField->setTooltipText(tooltip);
    pButton->setSize(width, 40);
    pButton->addChild(textField);

    oxygine::Sprite* ptr = pButton.get();
    pButton->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });

    pButton->addEventListener(oxygine::TouchEvent::OUTX, [=](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    pButton->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event*)
    {
        Mainapp::getInstance()->getAudioThread()->playSound("button.wav");
    });
    return pButton;
}


oxygine::spButton ObjectManager::createIconButton(QString icon, qint32 size)
{
    oxygine::spButton pButton = oxygine::spButton::create();
    oxygine::ResAnim* pAnim = ObjectManager::getInstance()->getResAnim("button_square");
    pButton->setResAnim(pAnim);
    pButton->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    pButton->setSize(size, size);

    oxygine::spSprite pSprite = oxygine::spSprite::create();
    pAnim = ObjectManager::getInstance()->getResAnim(icon);
    pSprite->setResAnim(pAnim);
    pSprite->setPosition((size - pSprite->getScaledWidth()) / 2,
                         (size - pSprite->getScaledHeight()) / 2);
    pButton->addChild(pSprite);

    oxygine::Sprite* ptr = pButton.get();
    pButton->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });

    pButton->addEventListener(oxygine::TouchEvent::OUTX, [=](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    pButton->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event*)
    {
        Mainapp::getInstance()->getAudioThread()->playSound("button.wav");
    });
    return pButton;
}

oxygine::spButton ObjectManager::createIconButton(oxygine::spSprite pSprite, qint32 size)
{
    oxygine::spButton pButton = oxygine::spButton::create();
    pButton->setResAnim(ObjectManager::getInstance()->getResAnim("button_square"));
    pButton->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    pButton->setSize(size, size);
    pSprite->setPosition((size - pSprite->getScaledWidth()) / 2,
                         (size - pSprite->getScaledHeight()) / 2);
    pButton->addChild(pSprite);

    oxygine::Sprite* ptr = pButton.get();
    pButton->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });

    pButton->addEventListener(oxygine::TouchEvent::OUTX, [=](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    pButton->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event*)
    {
        Mainapp::getInstance()->getAudioThread()->playSound("button.wav");
    });
    return pButton;
}
