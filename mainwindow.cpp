#include "mainwindow.h"

#include "resource_management/unitspritemanager.h"


Mainwindow::Mainwindow()
{
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    // test
    oxygine::spSprite sprite = new oxygine::Sprite();
    addChild(sprite);
    oxygine::ResAnim* myAnim = pUnitSpriteManager->getResAnim("hoellium");
    sprite->addTween(oxygine::Sprite::TweenAnim(myAnim), 500, -1);
    sprite->setPosition(400, 400);

    sprite->setScaleX(2);
    sprite->setScaleY(3);

    oxygine::spSprite sprite1 = new oxygine::Sprite();
    addChild(sprite1);
    myAnim = pUnitSpriteManager->getResAnim("hoellium+maske");
    sprite1->addTween(oxygine::Sprite::TweenAnim(myAnim, 0), 500, -1);
    sprite1->setColor(oxygine::Color(255, 255, 0));
    sprite1->setPosition(400, 400);

    oxygine::spSprite sprite2 = new oxygine::Sprite();
    // set draw priority all we need to do for z-ordering
    sprite2->setPriority(-20);
    addChild(sprite2);
    myAnim = pUnitSpriteManager->getResAnim("hoellium+maske");
    sprite2->addTween(oxygine::Sprite::TweenAnim(myAnim, 0), 500, -1);
    sprite2->setColor(oxygine::Color(0, 255, 0));
    sprite2->setPosition(405, 405);

}
