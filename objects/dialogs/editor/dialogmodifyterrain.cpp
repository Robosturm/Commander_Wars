#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "coreengine/interpreter.h"
#include "coreengine/globalutils.h"

#include "resource_management/objectmanager.h"
#include "resource_management/terrainmanager.h"
#include "resource_management/fontmanager.h"

#include "coreengine/interpreter.h"

#include "game/gamemap.h"

#include "objects/dialogs/editor/dialogmodifyterrain.h"
#include "objects/dialogs/filedialog.h"
#include "objects/base/label.h"
#include "objects/base/checkbox.h"

DialogModifyTerrain::DialogModifyTerrain(GameMap* pMap, Terrain* pTerrain)
    : m_pTerrain(pTerrain),
      m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("DialogModifyTerrain");
#endif
    Interpreter::setCppOwnerShip(this);

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - m_OkButton->getScaledWidth() / 2,
                            oxygine::Stage::getStage()->getHeight() - 30 - m_OkButton->getScaledHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigFinished();
    });

    m_pPanel = spPanel::create(true, QSize(oxygine::Stage::getStage()->getWidth() - 60, oxygine::Stage::getStage()->getHeight() - 110),
                                     QSize(oxygine::Stage::getStage()->getWidth() - 60, oxygine::Stage::getStage()->getHeight() - 110));
    m_pPanel->setPosition(30, 30);
    pSpriteBox->addChild(m_pPanel);
    load();
    connect(this, &DialogModifyTerrain::sigFinished, this, &DialogModifyTerrain::remove, Qt::QueuedConnection);
    connect(this, &DialogModifyTerrain::sigChangePalette, this, &DialogModifyTerrain::changePalette, Qt::QueuedConnection);
    connect(this, &DialogModifyTerrain::sigTerrainClicked, this, &DialogModifyTerrain::terrainClicked, Qt::QueuedConnection);
    connect(this, &DialogModifyTerrain::sigShowLoadDialog, this, &DialogModifyTerrain::showLoadDialog, Qt::QueuedConnection);
    connect(this, &DialogModifyTerrain::sigLoadCustomSprite, this, &DialogModifyTerrain::loadCustomSprite, Qt::QueuedConnection);
}

void DialogModifyTerrain::load()
{
    Mainapp::getInstance()->pauseRendering();
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    m_pPanel->clearContent();
    qint32 y = 20;
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    oxygine::TextStyle style2 = oxygine::TextStyle(FontManager::getMainFont48());
    auto style3 = style2;
    style3.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    spLabel pLabel = spLabel::create(m_pPanel->getScaledWidth() - 50);
    pLabel->setStyle(style3);
    pLabel->setHtmlText(tr("Edit terrain"));
    pLabel->setPosition(25, y);
    m_pPanel->addItem(pLabel);
    y += pLabel->getHeight() + 10;

    pLabel = spLabel::create(m_pPanel->getScaledWidth() - 50);
    pLabel->setStyle(style2);
    pLabel->setHtmlText(tr("Information"));
    pLabel->setPosition(10, y);
    m_pPanel->addItem(pLabel);
    y += pLabel->getHeight() + 10;

    pLabel = spLabel::create(190);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Name:"));
    pLabel->setPosition(10, y);
    m_pPanel->addItem(pLabel);
    spTextbox pTextbox = spTextbox::create(m_pPanel->getContentWidth() - 100 - 200 - pLabel->getScaledWidth());
    pTextbox->setTooltipText(tr("Custom Name of the Terrain. Empty name equals the default name."));
    pTextbox->setPosition(200 + 20 + pLabel->getX(), y);
    pTextbox->setCurrentText(m_pTerrain->getTerrainName());
    connect(pTextbox.get(), &Textbox::sigTextChanged, m_pTerrain, [this](QString value)
    {
        m_pTerrain->setTerrainName(value, !value.isEmpty());
    }, Qt::QueuedConnection);
    m_pPanel->addItem(pTextbox);
    y += pLabel->getHeight() + 10;

    pLabel = spLabel::create(190);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Description:"));
    pLabel->setPosition(10, y);
    pTextbox = spTextbox::create(m_pPanel->getContentWidth() - 100 - 200 - pLabel->getScaledWidth());
    pTextbox->setTooltipText(tr("Custom Description of the Terrain. Empty description equals the default description."));
    pTextbox->setPosition(200 + 20 + pLabel->getX(), y);
    pTextbox->setCurrentText(m_pTerrain->getTerrainDescription());
    connect(pTextbox.get(), &Textbox::sigTextChanged, m_pTerrain, &Terrain::setTerrainDescription, Qt::QueuedConnection);
    m_pPanel->addItem(pTextbox);
    m_pPanel->addItem(pLabel);
    y += pLabel->getHeight() + 10;

    pLabel = spLabel::create(190);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Palette:"));
    pLabel->setPosition(10, y);
    m_pPanel->addItem(pLabel);
    spDropDownmenu pDropDownmenu = spDropDownmenu::create(400, Terrain::getPaletteNames());
    pDropDownmenu->setTooltipText(tr("Changes the palette used by the terrain."));
    pDropDownmenu->setPosition(200 + 20 + pLabel->getX(), y);
    if (m_selectedPalette >=  0)
    {
        pDropDownmenu->setCurrentItem(m_selectedPalette);
    }
    else
    {
        pDropDownmenu->setCurrentItemText(Terrain::getPaletteName(m_pTerrain->getPalette()));
        m_selectedPalette = 0;
    }
    connect(pDropDownmenu.get(), &DropDownmenu::sigItemChanged, this, [this](qint32 item)
    {
        m_selectedPalette = item;
        emit sigChangePalette(Terrain::getPaletteId(item, m_pTerrain->getTerrainID()));
    }, Qt::QueuedConnection);
    m_pPanel->addItem(pDropDownmenu);
    y += pLabel->getHeight() + 10;

    spLabel pTextfield = spLabel::create(180);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Terrain Style"));
    pTextfield->setPosition(10, y);
    m_pPanel->addItem(pTextfield);
    m_pTextbox = spTextbox::create(m_pPanel->getContentWidth() - 100 - 200 - pTextfield->getScaledWidth());
    m_pTextbox->setTooltipText(tr("Current select terrain image or terrain path or empty for default selection."));
    m_pTextbox->setPosition(200 + 20 + pTextfield->getX(), y);
    connect(m_pTextbox.get(), &Textbox::sigTextChanged, this, [this](QString text)
    {
        emit sigLoadCustomSprite(text);
    });
    m_pPanel->addItem(m_pTextbox);
    y += pTextfield->getHeight() + 20;

    oxygine::spButton pButtonDefault = pObjectManager->createButton(tr("Default"), 250);
    pButtonDefault->setPosition(10, y);
    m_pPanel->addItem(pButtonDefault);
    pButtonDefault->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigTerrainClicked("");
    });

    oxygine::spButton pButtonSelect = pObjectManager->createButton(tr("Select Image"), 250);
    pButtonSelect->setPosition(10 + 20 + pButtonDefault->getScaledWidth(), y);
    m_pPanel->addItem(pButtonSelect);
    pButtonSelect->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigShowLoadDialog();
    });
    y += pButtonSelect->getHeight() + 20;

    loadBaseImageview(y, m_pTerrain);
    loadOverlayview(y, m_pTerrain);

    m_pPanel->setContentHeigth(y + 60);
    if (m_pTerrain->getFixedSprite())
    {
        m_pTextbox->setCurrentText(m_pTerrain->getTerrainSpriteName());
    }
    else
    {
        m_pTextbox->setCurrentText("");
    }
    Mainapp::getInstance()->continueRendering();
}

void DialogModifyTerrain::loadBaseImageview(qint32 & y, Terrain* pTerrain)
{
    qint32 x = 20;
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    oxygine::TextStyle style2 = oxygine::TextStyle(FontManager::getMainFont48());

    QStringList pTerrainStyles = pTerrain->getTerrainSprites();
    spLabel pLabel = spLabel::create(m_pPanel->getScaledWidth() - 50);
    pLabel->setStyle(style2);
    pLabel->setHtmlText(tr("Base image"));
    pLabel->setPosition(10, y);
    m_pPanel->addItem(pLabel);
    y += pLabel->getHeight() + GameMap::getImageSize() * 2 + 10;

    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    for (qint32 i = 0; i < pTerrainStyles.size(); i++)
    {
        QString id = pTerrainStyles[i];
        oxygine::ResAnim* pAnim = pTerrainManager->getResAnim(id, oxygine::error_policy::ep_ignore_error);
        if (pAnim != nullptr)
        {
            spTerrain pSprite = Terrain::createTerrain(pTerrain->getBaseTerrainID(), -2, -2, "", m_pMap);
            pSprite->setFixedSprite(true);
            pSprite->setTerrainSpriteName(id);
            pSprite->setPalette(pTerrain->getPalette());
            pSprite->loadSprites();
            pSprite->setScale(2.0f);
            pSprite->setPosition(x, y);
            pSprite->addClickListener([this, id](oxygine::Event*)
            {
                emit sigTerrainClicked(id);
            });
            m_pPanel->addItem(pSprite);

            x += GameMap::getImageSize() * 3 ;
            if (x > m_pPanel->getContentWidth() - 100 - pSprite->getScaledWidth() && i < pTerrainStyles.size() - 1)
            {
                x = 20;
                y += GameMap::getImageSize() * 3;
            }
        }
    }
    y += GameMap::getImageSize() * 3;
}

void DialogModifyTerrain::changePalette(const QString newPalette)
{
    m_pTerrain->setPalette(newPalette);
    auto overlays = m_pTerrain->getCustomOverlays();
    QStringList palettes;
    for (auto & overlay : overlays)
    {
        palettes.append(newPalette);
    }
    m_pTerrain->setCustomOverlays(overlays, palettes);
    load();
}

void DialogModifyTerrain::loadOverlayview(qint32 & y, Terrain* pTerrain)
{
    QStringList overlayTerrainStyles = pTerrain->getOverlayTerrainSprites();
    if (overlayTerrainStyles.size() > 0)
    {
        connect(this, &DialogModifyTerrain::sigOverlayChanged, this, &DialogModifyTerrain::overlayChanged, Qt::QueuedConnection);
        qint32 x = 20;
        oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
        style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        style.multiline = false;
        oxygine::TextStyle style2 = oxygine::TextStyle(FontManager::getMainFont48());

        QStringList selectedOverlayTerrainStyles = pTerrain->getCustomOverlays();
        spLabel pLabel = spLabel::create(m_pPanel->getScaledWidth() - 50);
        pLabel->setStyle(style2);
        pLabel->setHtmlText(tr("Overlays"));
        pLabel->setPosition(10, y);
        m_pPanel->addItem(pLabel);
        y += pLabel->getHeight() + 10;
        QString palette = pTerrain->getPalette();

        TerrainManager* pTerrainManager = TerrainManager::getInstance();
        for (qint32 i = 0; i < overlayTerrainStyles.size(); i++)
        {
            QString id = overlayTerrainStyles[i];
            oxygine::ResAnim* pAnim = pTerrainManager->getResAnim(id, oxygine::error_policy::ep_ignore_error);
            if (pAnim != nullptr)
            {
                constexpr qint32 scale = 2;
                oxygine::spColorRectSprite pRect = oxygine::spColorRectSprite::create();
                pRect->setPosition(x, y);
                pRect->setSize(GameMap::getImageSize() * scale, GameMap::getImageSize() * scale);
                pRect->setColor(QColor(100, 100, 100, 100));

                oxygine::spSprite pSprite = oxygine::spSprite::create();
                if (!palette.isEmpty())
                {
                    oxygine::spResAnim pPaletteAnim = oxygine::spResAnim(pTerrainManager->getResAnim(palette, oxygine::error_policy::ep_ignore_error));
                    if (pPaletteAnim.get() != nullptr)
                    {
                        pSprite->setColorTable(pPaletteAnim, true);
                    }
                }
                pSprite->setResAnim(pAnim);
                pSprite->setSize(pAnim->getSize());
                pSprite->setScale(static_cast<float>(GameMap::getImageSize()) * scale / static_cast<float>(pAnim->getWidth()));
                pRect->addChild(pSprite);
                m_pPanel->addItem(pRect);

                bool selected = selectedOverlayTerrainStyles.contains(id);
                spCheckbox pCheckbox = spCheckbox::create();
                pCheckbox->setPosition(x + pRect->getScaledWidth() + 10, y + pRect->getScaledHeight() / 2 - pCheckbox->getScaledHeight() / 2);
                pCheckbox->setChecked(selected);
                connect(pCheckbox.get(), &Checkbox::checkChanged, this, [this, id](bool checked)
                {
                    emit sigOverlayChanged(id, checked);
                }, Qt::QueuedConnection);
                m_pPanel->addItem(pCheckbox);

                x += GameMap::getImageSize() * 3 + pCheckbox->getScaledWidth();
                if (x > m_pPanel->getContentWidth() - 100 - pSprite->getScaledWidth() && i < overlayTerrainStyles.size() - 1)
                {
                    x = 20;
                    y += GameMap::getImageSize() * 3;
                }
            }
        }
        y += GameMap::getImageSize() * 3;
    }
}

void DialogModifyTerrain::remove()
{
    if (m_changed)
    {
        QVector<QPoint> points({QPoint(m_pTerrain->getX(), m_pTerrain->getY())});
       m_pMap->updateSpritesOfTiles(points, true);
    }
    detach();
}

void DialogModifyTerrain::terrainClicked(QString id)
{
    m_pTerrain->setFixedSprite(!id.isEmpty());
    m_pTerrain->setTerrainSpriteName(id);
    m_pTerrain->loadSprites();
    m_pTextbox->setCurrentText(id);
    m_changed = true;
}

void DialogModifyTerrain::showLoadDialog()
{    
    QStringList wildcards;
    wildcards.append("*.png");
    QString path = Settings::getInstance()->getUserPath() + "customTerrainImages";
    spFileDialog fileDialog = spFileDialog::create(path, wildcards, false, m_pMap->getMapName(), true, tr("Load"));
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &DialogModifyTerrain::loadCustomSprite, Qt::QueuedConnection);
    addChild(fileDialog);    
}

void DialogModifyTerrain::loadCustomSprite(QString id)
{
    QString imageId = GlobalUtils::makePathRelative(id);
    if (QFile::exists(imageId))
    {
        terrainClicked(imageId);
    }
}

void DialogModifyTerrain::overlayChanged(QString id, bool selected)
{
    if (selected)
    {
        QString palette = Terrain::getPaletteId(m_selectedPalette, m_pTerrain->getTerrainID());
        if (palette.isEmpty())
        {
            palette = m_pTerrain->getDefaultPalette();
        }
        m_pTerrain->addCustomOverlay(id, palette);
    }
    else
    {
        m_pTerrain->removeCustomOverlay(id);
    }
    m_pTerrain->setFixedOverlaySprites(m_pTerrain->getCustomOverlays().size() > 0);
    m_pTerrain->loadSprites();
}
