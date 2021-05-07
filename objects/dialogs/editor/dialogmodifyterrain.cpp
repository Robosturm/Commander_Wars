#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "resource_management/terrainmanager.h"

#include "resource_management/fontmanager.h"

#include "dialogmodifyterrain.h"

#include "game/gamemap.h"

#include "objects/dialogs/filedialog.h"
#include "objects/base/label.h"

DialogModifyTerrain::DialogModifyTerrain(Terrain* pTerrain)
    : QObject(),
      m_pTerrain(pTerrain)
{
    setObjectName("DialogModifyTerrain");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(Settings::getWidth() / 2 - m_OkButton->getWidth() / 2, Settings::getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigFinished();
        detach();
    });

    m_pPanel = spPanel::create(true, QSize(Settings::getWidth() - 60, Settings::getHeight() - 110),
                                     QSize(Settings::getWidth() - 60, Settings::getHeight() - 110));
    m_pPanel->setPosition(30, 30);
    pSpriteBox->addChild(m_pPanel);


    QStringList pTerrainStyles = pTerrain->getTerrainSprites();

    qint32 x = 20;
    qint32 y = 20;
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    spLabel pLabel = spLabel::create(190);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Name:"));
    pLabel->setPosition(10, y);
    spTextbox pTextbox = spTextbox::create(m_pPanel->getContentWidth() - 100 - 200 - pLabel->getWidth());
    pTextbox->setTooltipText(tr("Custom Name of the Terrain. Empty name equals the default name."));
    pTextbox->setPosition(200 + 20 + pLabel->getX(), y);
    pTextbox->setCurrentText(pTerrain->getTerrainName());
    connect(pTextbox.get(), &Textbox::sigTextChanged, pTerrain, &Terrain::setTerrainName, Qt::QueuedConnection);
    m_pPanel->addItem(pTextbox);
    m_pPanel->addItem(pLabel);
    y += 40;

    pLabel = spLabel::create(190);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Description:"));
    pLabel->setPosition(10, y);
    pTextbox = spTextbox::create(m_pPanel->getContentWidth() - 100 - 200 - pLabel->getWidth());
    pTextbox->setTooltipText(tr("Custom Description of the Terrain. Empty description equals the default description."));
    pTextbox->setPosition(200 + 20 + pLabel->getX(), y);
    pTextbox->setCurrentText(pTerrain->getTerrainDescription());
    connect(pTextbox.get(), &Textbox::sigTextChanged, pTerrain, &Terrain::setTerrainDescription, Qt::QueuedConnection);
    m_pPanel->addItem(pTextbox);
    m_pPanel->addItem(pLabel);
    y += 60;

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

            x += GameMap::getImageSize() * 3 ;
            if (x > m_pPanel->getContentWidth() - 100 - pSprite->getWidth() && i < pTerrainStyles.size() - 1)
            {
                x = 20;
                y += GameMap::getImageSize() * 3;
            }
        }
    }
    y += GameMap::getImageSize() * 3;



    spLabel pTextfield = spLabel::create(180);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Terrain Style"));
    pTextfield->setPosition(10, y);
    m_pPanel->addItem(pTextfield);
    m_pTextbox = spTextbox::create(m_pPanel->getContentWidth() - 100 - 200 - pTextfield->getWidth());
    m_pTextbox->setTooltipText(tr("Current select terrain image or terrain path or empty for default selection."));
    m_pTextbox->setPosition(200 + 20 + pTextfield->getX(), y);
    m_pPanel->addItem(m_pTextbox);
    y += 80;

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
    
    m_pTerrain->setFixedSprite(!id.isEmpty());
    m_pTerrain->setTerrainSpriteName(id);
    m_pTextbox->setCurrentText(id);
    m_pTerrain->loadSprites();
    
}

void DialogModifyTerrain::showLoadDialog()
{
    
    QVector<QString> wildcards;
    wildcards.append("*.png");
    QString path = QCoreApplication::applicationDirPath() + "/customTerrainImages";
    spFileDialog fileDialog = spFileDialog::create(path, wildcards, GameMap::getInstance()->getMapName(), true);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &DialogModifyTerrain::loadCustomSprite, Qt::QueuedConnection);
    this->addChild(fileDialog);
    
}

void DialogModifyTerrain::loadCustomSprite(QString id)
{
    QString imageId = id.replace(QCoreApplication::applicationDirPath() + "/", "");
    terrainClicked(imageId);
}
