#include "coreengine/mainapp.h"

#include "game/createoutline.h"
#include "game/gamemap.h"

#include "resource_management/objectmanager.h"

void CreateOutline::addCursorRangeOutline(oxygine::spActor pOutlineSprite, QString prefix, qint32 range, QColor color)
{
    
    for (qint32 i = 0; i <= range; i++)
    {
        if (i == 0)
        {
            createOuterLeftRightOutline(pOutlineSprite, prefix, range, color);
        }
        else if (i == range)
        {
            createOuterTopBottomOutline(pOutlineSprite, prefix, range, color);
        }
        else
        {
            createOutline(pOutlineSprite, prefix, i, range, color);
        }
    }
    
}

void CreateOutline::createOuterLeftRightOutline(oxygine::spActor pOutlineSprite, QString prefix, qint32 range, QColor color)
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnimBottom = pObjectManager->getResAnim(prefix + "+bottom");
    oxygine::ResAnim* pAnimTop = pObjectManager->getResAnim(prefix + "+top");
    oxygine::ResAnim* pAnimLeft = pObjectManager->getResAnim(prefix + "+left");
    oxygine::ResAnim* pAnimRight = pObjectManager->getResAnim(prefix + "+right");

    oxygine::spSprite pSprite = oxygine::spSprite::create();
    if (pAnimLeft->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimLeft), oxygine::timeMS(pAnimLeft->getTotalFrames() * GameMap::frameTime), -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnimLeft);
    }
    pSprite->setX(-range * GameMap::getImageSize() - 2);
    pSprite->setColor(color);
    pOutlineSprite->addChild(pSprite);

    pSprite = oxygine::spSprite::create();
    if (pAnimTop->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimTop), oxygine::timeMS(pAnimTop->getTotalFrames() * GameMap::frameTime), -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnimTop);
    }
    pSprite->setX(-range * GameMap::getImageSize());
    pSprite->setY(-2);
    pSprite->setColor(color);
    pOutlineSprite->addChild(pSprite);

    pSprite = oxygine::spSprite::create();
    if (pAnimBottom->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimBottom), oxygine::timeMS(pAnimBottom->getTotalFrames() * GameMap::frameTime), -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnimBottom);
    }
    pSprite->setX(-range * GameMap::getImageSize());
    pSprite->setY(2);
    pSprite->setColor(color);
    pOutlineSprite->addChild(pSprite);

    pSprite = oxygine::spSprite::create();
    if (pAnimRight->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimRight), oxygine::timeMS(pAnimRight->getTotalFrames() * GameMap::frameTime), -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnimRight);
    }
    pSprite->setX(range * GameMap::getImageSize() + 2);
    pSprite->setColor(color);
    pOutlineSprite->addChild(pSprite);

    pSprite = oxygine::spSprite::create();
    if (pAnimTop->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimTop), oxygine::timeMS(pAnimTop->getTotalFrames() * GameMap::frameTime), -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnimTop);
    }
    pSprite->setX(range * GameMap::getImageSize());
    pSprite->setY(-2);
    pSprite->setColor(color);
    pOutlineSprite->addChild(pSprite);

    pSprite = oxygine::spSprite::create();
    if (pAnimBottom->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimBottom), oxygine::timeMS(pAnimBottom->getTotalFrames() * GameMap::frameTime), -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnimBottom);
    }
    pSprite->setX(range * GameMap::getImageSize());
    pSprite->setY(2);
    pSprite->setColor(color);
    pOutlineSprite->addChild(pSprite);
}

void CreateOutline::createOuterTopBottomOutline(oxygine::spActor pOutlineSprite, QString prefix, qint32 range, QColor color)
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnimBottom = pObjectManager->getResAnim(prefix + "+bottom");
    oxygine::ResAnim* pAnimTop = pObjectManager->getResAnim(prefix + "+top");
    oxygine::ResAnim* pAnimLeft = pObjectManager->getResAnim(prefix + "+left");
    oxygine::ResAnim* pAnimRight = pObjectManager->getResAnim(prefix + "+right");

    oxygine::spSprite pSprite = oxygine::spSprite::create();
    if (pAnimTop->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimTop), oxygine::timeMS(pAnimTop->getTotalFrames() * GameMap::frameTime), -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnimTop);
    }
    pSprite->setY(-range * GameMap::getImageSize() - 2);
    pSprite->setColor(color);
    pOutlineSprite->addChild(pSprite);

    pSprite = oxygine::spSprite::create();
    if (pAnimLeft->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimLeft), oxygine::timeMS(pAnimLeft->getTotalFrames() * GameMap::frameTime), -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnimLeft);
    }
    pSprite->setX(-2);
    pSprite->setY(-range * GameMap::getImageSize());
    pSprite->setColor(color);
    pOutlineSprite->addChild(pSprite);

    pSprite = oxygine::spSprite::create();
    if (pAnimRight->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimRight), oxygine::timeMS(pAnimRight->getTotalFrames() * GameMap::frameTime), -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnimRight);
    }
    pSprite->setY(-range * GameMap::getImageSize());
    pSprite->setX(2);
    pSprite->setColor(color);
    pOutlineSprite->addChild(pSprite);

    pSprite = oxygine::spSprite::create();
    if (pAnimBottom->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimBottom), oxygine::timeMS(pAnimBottom->getTotalFrames() * GameMap::frameTime), -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnimBottom);
    }
    pSprite->setY(range * GameMap::getImageSize() + 2);
    pSprite->setColor(color);
    pOutlineSprite->addChild(pSprite);

    pSprite = oxygine::spSprite::create();
    if (pAnimLeft->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimLeft), oxygine::timeMS(pAnimLeft->getTotalFrames() * GameMap::frameTime), -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnimLeft);
    }
    pSprite->setX(-2);
    pSprite->setY(range * GameMap::getImageSize());
    pSprite->setColor(color);
    pOutlineSprite->addChild(pSprite);

    pSprite = oxygine::spSprite::create();
    if (pAnimRight->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimRight), oxygine::timeMS(pAnimRight->getTotalFrames() * GameMap::frameTime), -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnimRight);
    }
    pSprite->setY(range * GameMap::getImageSize());
    pSprite->setX(2);
    pSprite->setColor(color);
    pOutlineSprite->addChild(pSprite);
}

void CreateOutline::createOutline(oxygine::spActor pOutlineSprite, QString prefix, qint32 i, qint32 range, QColor color)
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnimBottom = pObjectManager->getResAnim(prefix + "+bottom");
    oxygine::ResAnim* pAnimTop = pObjectManager->getResAnim(prefix + "+top");
    oxygine::ResAnim* pAnimLeft = pObjectManager->getResAnim(prefix + "+left");
    oxygine::ResAnim* pAnimRight = pObjectManager->getResAnim(prefix + "+right");

    // left top
    oxygine::spSprite pSprite = oxygine::spSprite::create();
    if (pAnimLeft->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimLeft), oxygine::timeMS(pAnimLeft->getTotalFrames() * GameMap::frameTime), -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnimLeft);
    }
    pSprite->setX(-(range - i) * GameMap::getImageSize() - 2);
    pSprite->setY(-i * GameMap::getImageSize());
    pSprite->setColor(color);
    pOutlineSprite->addChild(pSprite);

    pSprite = oxygine::spSprite::create();
    if (pAnimTop->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimTop), oxygine::timeMS(pAnimTop->getTotalFrames() * GameMap::frameTime), -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnimTop);
    }
    pSprite->setX(-(range - i) * GameMap::getImageSize());
    pSprite->setY(-i * GameMap::getImageSize() - 2);
    pSprite->setColor(color);
    pOutlineSprite->addChild(pSprite);

    // left bottom
    pSprite = oxygine::spSprite::create();
    if (pAnimLeft->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimLeft), oxygine::timeMS(pAnimLeft->getTotalFrames() * GameMap::frameTime), -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnimLeft);
    }
    pSprite->setX(-(range - i) * GameMap::getImageSize() - 2);
    pSprite->setY(i * GameMap::getImageSize());
    pSprite->setColor(color);
    pOutlineSprite->addChild(pSprite);

    pSprite = oxygine::spSprite::create();
    if (pAnimBottom->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimBottom), oxygine::timeMS(pAnimBottom->getTotalFrames() * GameMap::frameTime), -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnimBottom);
    }
    pSprite->setX(-(range - i) * GameMap::getImageSize());
    pSprite->setY((i) * GameMap::getImageSize() + 2);
    pSprite->setColor(color);
    pOutlineSprite->addChild(pSprite);

    // right top
    pSprite = oxygine::spSprite::create();
    if (pAnimRight->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimRight), oxygine::timeMS(pAnimRight->getTotalFrames() * GameMap::frameTime), -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnimRight);
    }
    pSprite->setX((range - i) * GameMap::getImageSize() + 2);
    pSprite->setY(-i * GameMap::getImageSize());
    pSprite->setColor(color);
    pOutlineSprite->addChild(pSprite);

    pSprite = oxygine::spSprite::create();
    if (pAnimTop->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimTop), oxygine::timeMS(pAnimTop->getTotalFrames() * GameMap::frameTime), -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnimTop);
    }
    pSprite->setX((range - i) * GameMap::getImageSize());
    pSprite->setY(-i * GameMap::getImageSize() - 2);
    pSprite->setColor(color);
    pOutlineSprite->addChild(pSprite);

    // right bottom
    pSprite = oxygine::spSprite::create();
    if (pAnimRight->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimRight), oxygine::timeMS(pAnimRight->getTotalFrames() * GameMap::frameTime), -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnimRight);
    }
    pSprite->setX((range - i) * GameMap::getImageSize() + 2);
    pSprite->setY(i * GameMap::getImageSize());
    pSprite->setColor(color);
    pOutlineSprite->addChild(pSprite);

    pSprite = oxygine::spSprite::create();
    if (pAnimBottom->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimBottom), oxygine::timeMS(pAnimBottom->getTotalFrames() * GameMap::frameTime), -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnimBottom);
    }
    pSprite->setX((range - i) * GameMap::getImageSize());
    pSprite->setY((i) * GameMap::getImageSize() + 2);
    pSprite->setColor(color);
    pOutlineSprite->addChild(pSprite);
}
