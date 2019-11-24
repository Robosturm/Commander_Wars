#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "resource_management/terrainmanager.h"

#include "resource_management/fontmanager.h"

#include "dialogmodifyterrain.h"

#include "game/gamemap.h"

#include "objects/filedialog.h"

DialogModifyTerrain::DialogModifyTerrain(Terrain* pTerrain)
    : QObject(),
      m_pTerrain(pTerrain)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(pApp->getSettings()->getWidth(), pApp->getSettings()->getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(pApp->getSettings()->getWidth() / 2 - m_OkButton->getWidth() / 2, pApp->getSettings()->getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        this->getParent()->removeChild(this);
        emit sigFinished();
    });

    m_pPanel = new Panel(true, QSize(pApp->getSettings()->getWidth() - 60, pApp->getSettings()->getHeight() - 110),
                                     QSize(pApp->getSettings()->getWidth() - 60, pApp->getSettings()->getHeight() - 110));
    m_pPanel->setPosition(30, 30);
    pSpriteBox->addChild(m_pPanel);


    QStringList pTerrainStyles = pTerrain->getTerrainSprites();

    qint32 x = 20;
    qint32 y = 20;

    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    for (qint32 i = 0; i < pTerrainStyles.size(); i++)
    {
        QString id = pTerrainStyles[i];
        oxygine::ResAnim* pAnim = pTerrainManager->getResAnim(id, oxygine::error_policy::ep_ignore_error);
        if (pAnim != nullptr)
        {
            spTerrain pSprite = Terrain::createTerrain(pTerrain->getBaseTerrainID(), -2, -2, "");
            pSprite->setTerrainSpriteName(id);
            pSprite->setFixedSprite(true);
            pSprite->loadSprites();
            pSprite->setScale(2);            
            pSprite->setPosition(x, y);
            pSprite->addClickListener([=](oxygine::Event*)
            {
                emit sigTerrainClicked(id);
            });
            m_pPanel->addItem(pSprite);

            x += GameMap::Imagesize * 3 ;
            if (x > m_pPanel->getContentWidth() - 60 - pSprite->getWidth() && i < pTerrainStyles.size() - 1)
            {
                x = 20;
                y += GameMap::Imagesize * 3;
            }
        }
    }
    y += GameMap::Imagesize * 3;

    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = QColor(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    oxygine::spTextField pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Terrain Style"));
    pTextfield->setPosition(10, y);
    m_pPanel->addItem(pTextfield);
    m_pTextbox = new Textbox(m_pPanel->getContentWidth() - 60 - 200 - pTextfield->getWidth());
    m_pTextbox->setPosition(200 + 20 + pTextfield->getX(), y);
    m_pPanel->addItem(m_pTextbox);
    y += 40;

    oxygine::spButton pButtonDefault = pObjectManager->createButton(tr("Default"), 150);
    pButtonDefault->setPosition(10, y);
    m_pPanel->addChild(pButtonDefault);
    pButtonDefault->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
         emit sigTerrainClicked("");
    });

    oxygine::spButton pButtonSelect = pObjectManager->createButton(tr("Select Image"), 150);
    pButtonSelect->setPosition(10 + 20 + pButtonDefault->getWidth(), y);
    m_pPanel->addChild(pButtonSelect);
    pButtonSelect->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
         emit sigShowLoadDialog();
    });

    connect(this, &DialogModifyTerrain::sigTerrainClicked, this, &DialogModifyTerrain::terrainClicked, Qt::QueuedConnection);
    connect(this, &DialogModifyTerrain::sigShowLoadDialog, this, &DialogModifyTerrain::showLoadDialog, Qt::QueuedConnection);

    m_pPanel->setContentHeigth(y + 60);
    if (pTerrain->getFixedSprite())
    {
        terrainClicked(pTerrain->getTerrainSpriteName());
    }
    else
    {
        terrainClicked("");
    }
}

void DialogModifyTerrain::terrainClicked(QString id)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pTerrain->setFixedSprite(!id.isEmpty());
    m_pTerrain->setTerrainSpriteName(id);
    m_pTextbox->setCurrentText(id);
    m_pTerrain->loadSprites();
    pApp->continueThread();
}

void DialogModifyTerrain::showLoadDialog()
{
    QVector<QString> wildcards;
    wildcards.append("*.png");
    QString path = QCoreApplication::applicationDirPath() + "/customTerrainImages";
    spFileDialog fileDialog = new FileDialog(path, wildcards, GameMap::getInstance()->getMapName());
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &DialogModifyTerrain::loadCustomSprite, Qt::QueuedConnection);
    this->addChild(fileDialog);
}

void DialogModifyTerrain::loadCustomSprite(QString id)
{
    QString imageId = id.replace(QCoreApplication::applicationDirPath() + "/", "");
    terrainClicked(imageId);
}
