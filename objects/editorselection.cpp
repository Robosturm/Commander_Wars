#include "objects/editorselection.h"

#include "resource_management/objectmanager.h"
#include "resource_management/terrainmanager.h"

#include "coreengine/mainapp.h"

const float EditorSelection::xFactor = 1.5f;
const float EditorSelection::yFactor = 2.5f;

EditorSelection::EditorSelection()
    : QObject()
{
    Mainapp* pMainapp = Mainapp::getInstance();
    ObjectManager* pObjectManager = ObjectManager::getInstance();

    oxygine::spBox9Sprite pSprite = new oxygine::Box9Sprite();
    pSprite->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pSprite->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    this->addChild(pSprite);
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("editorselection");
    pSprite->setResAnim(pAnim);

    pSprite->setSize(pMainapp->getSettings()->getWidth() / 4.0f, pMainapp->getSettings()->getHeight());

    this->setPosition(pMainapp->getSettings()->getWidth() - pSprite->getWidth(), 0);
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    m_Box = pSprite;

    m_CurrentSelector = new oxygine::Sprite();
    pAnim = pObjectManager->getResAnim("editor+selector");
    m_CurrentSelector->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    if (pAnim->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
        m_CurrentSelector->addTween(tween);
    }
    else
    {
        m_CurrentSelector->setResAnim(pAnim);
    }
    this->addChild(m_CurrentSelector);
    m_CurrentSelector->setPosition(frameSize, startH);

    // create terrains
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    // reserve vector size for fun and speed :D
    m_Terrains.reserve(pTerrainManager->getTerrainCount());
    for (qint32 i = 0; i < pTerrainManager->getTerrainCount(); i++)
    {
        m_Terrains.append(Terrain::createTerrain(pTerrainManager->getTerrainID(i), -10, -10));
        m_Terrains[i]->loadSprites();

        this->addChild(m_Terrains[i]);
    }
    updateTerrainView();


    addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            emit sigClicked(pTouchEvent->getPointer()->getPosition().x, pTouchEvent->getPointer()->getPosition().y);
        }
    });
    connect(this, SIGNAL(sigClicked(qint32,qint32)), this, SLOT(Clicked(qint32,qint32)));
}

void EditorSelection::updateTerrainView()
{
    qint32 posY = startH;
    qint32 xCounter = 0;
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    for (qint32 i = m_StartIndex; i < pTerrainManager->getTerrainCount(); i++)
    {
        qint32 posX = frameSize + xCounter * GameMap::Imagesize * xFactor;
        if (posX > m_Box->getWidth() - GameMap::Imagesize - frameSize)
        {
            posY += GameMap::Imagesize * yFactor;
            xCounter = 0;
            posX = frameSize;
        }
        m_Terrains[i]->setPosition(posX, posY);
        xCounter++;
    }
}

void EditorSelection::Clicked(qint32 x, qint32 y)
{
    qint32 curX = x - this->getPosition().x - frameSize;
    qint32 curY = y - startH;
    if ((curX >= 0) && (curY >= 0) && (curX < m_Box->getWidth() - GameMap::Imagesize - frameSize))
    {
        // check modulo fraction first
        qint32 xHit = curX % static_cast<qint32>(GameMap::Imagesize * xFactor);
        if (xHit <= GameMap::Imagesize)
        {
            xHit = curX / (GameMap::Imagesize * xFactor);
            qint32 yHit = curY % static_cast<qint32>(GameMap::Imagesize * yFactor);
            if (yHit <= GameMap::Imagesize)
            {
                qint32 xCount = (m_Box->getWidth() - GameMap::Imagesize - frameSize - frameSize) / (GameMap::Imagesize * xFactor) + 1;
                m_selectedIndex.setZ(xCount);
                yHit = curY / (GameMap::Imagesize * yFactor);
                // valid click?
                if (xHit + yHit * xCount < m_Terrains.size())
                {
                    m_selectedIndex.setX(xHit);
                    m_selectedIndex.setY(yHit);
                    m_CurrentSelector->setPosition(frameSize + xHit * GameMap::Imagesize * xFactor, startH + GameMap::Imagesize * yFactor * yHit);
                }
            }

        }
    }

}
