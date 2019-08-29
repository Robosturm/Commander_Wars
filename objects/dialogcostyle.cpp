#include "dialogcostyle.h"

#include "qfile.h"

#include "coreengine/mainapp.h"

#include "coreengine/userdata.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

#include "resource_management/cospritemanager.h"

#include "spritingsupport/spritecreator.h"

DialogCOStyle::DialogCOStyle(QString coid)
    : QObject(),
      m_currentCOID(coid)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    m_pSpriteBox = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    m_pSpriteBox->setResAnim(pAnim);
    m_pSpriteBox->setSize(pApp->getSettings()->getWidth(), pApp->getSettings()->getHeight());
    m_pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    m_pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(m_pSpriteBox);
    m_pSpriteBox->setPosition(0, 0);
    m_pSpriteBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Dialogs));

    // ok button
    oxygine::spButton pOkButton = pObjectManager->createButton(tr("Apply"), 150);
    pOkButton->setPosition(pApp->getSettings()->getWidth() - pOkButton->getWidth() - 30, pApp->getSettings()->getHeight() - 30 - pOkButton->getHeight());
    m_pSpriteBox->addChild(pOkButton);
    pOkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        Userdata::getInstance()->addCOStyle(m_currentCOID, m_ResFilePath, colorTable, maskTable);
        pCOSpriteManager->loadResAnim(m_currentCOID, m_ResFilePath, colorTable, maskTable);
        this->getParent()->removeChild(this);
        emit sigFinished();
    });

    // cancel button
    oxygine::spButton pExitButton = pObjectManager->createButton(tr("Cancel"), 150);
    pExitButton->setPosition(30, pApp->getSettings()->getHeight() - 30 - pExitButton->getHeight());
    m_pSpriteBox->addChild(pExitButton);
    pExitButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        this->getParent()->removeChild(this);
        emit sigCancel();
    });
    qint32 heigth = Settings::getHeight() - 220;
    qint32 width = Settings::getWidth() / 2 - 80;
    qint32 pixelSize = width;
    if (pixelSize > heigth)
    {
        pixelSize = heigth;
    }
    pixelSize = pixelSize / 256;
    if (pixelSize > 2)
    {
        pixelSize = 2;
    }

    m_pColorSelector = new ColorSelector(Qt::white, pixelSize);
    m_pColorSelector->setPosition(30, 30);
    m_pSpriteBox->addChild(m_pColorSelector);
    connect(m_pColorSelector.get(), &ColorSelector::sigSelecetedColorChanged, this, &DialogCOStyle::selecetedColorChanged, Qt::QueuedConnection);

    QSize size(Settings::getWidth() - m_pColorSelector->getWidth() - 50, heigth);
    m_pCOPanel = new Panel(true, size, size);
    m_pCOPanel->setPosition(Settings::getWidth() - size.width() - 30, 30);
    m_pSpriteBox->addChild(m_pCOPanel);


    m_Styles = pCOSpriteManager->getCOStyles(coid);
    m_Styles.push_front("");
    addCOStyle(m_Styles[0], true);
    for (qint32 i = 1; i < m_Styles.size(); i++)
    {
        addCOStyle(m_Styles[i], false);
    }

    oxygine::spTextField textField = new oxygine::TextField();
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    style.multiline = false;
    textField->setStyle(style);
    textField->setHtmlText(tr("Predefined Styles").toStdString().c_str());
    textField->setPosition(Settings::getWidth() / 2 - 10 - textField->getTextRect().getWidth(),  Settings::getHeight() - 70);
    textField->attachTo(m_pSpriteBox);

    connect(this, &DialogCOStyle::sigCOStyleChanged, this, &DialogCOStyle::changeCOStyle, Qt::QueuedConnection);

    QSize size2(Settings::getWidth() - 60, 100);
    m_pPixelPanel = new Panel(true, size2, size2);
    m_pPixelPanel->setPosition(30, Settings::getHeight() - 175);
    m_pSpriteBox->addChild(m_pPixelPanel);

    changeCOStyle(0);
}

void DialogCOStyle::selecetedColorChanged(QColor color)
{
    if (currentPixel < maskTable.width())
    {
        maskTable.setPixelColor(currentPixel, 0, color);
        m_Pixels[currentPixel]->setColor(color.red(), color.green(), color.blue(), 255);
        m_update = true;
    }
}

void DialogCOStyle::changeCOStyle(qint32 index)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    for (qint32 i = 0; i < m_Styles.size(); i++)
    {
        oxygine::ResAnim* pAnim = pCOSpriteManager->getResAnim((m_currentCOID + m_Styles[i] + "+nrm").toStdString().c_str());
        m_pCOSprites[i]->setResAnim(pAnim);
    }
    m_CurrentIndex = index;
    if (m_pPredefinedStyles.get() != nullptr)
    {
        m_pPredefinedStyles->detach();
        m_pPredefinedStyles = nullptr;
    }
    oxygine::ResAnim* pAnim =pCOSpriteManager->oxygine::Resources::getResAnim((m_currentCOID + "+nrm").toStdString().c_str());
    QString filePath = pAnim->getResPath().c_str();
    QString style = m_Styles[index];
    filePath = filePath.replace("+nrm.png", "");
    m_ResFilePath = filePath + style;
    QFile file(m_ResFilePath + "+table.png");
    if (file.exists())
    {
        QVector<QString> items;
        baseColorTable.load(m_ResFilePath + "+table.png");
        colorTable = baseColorTable.copy(0, 0, baseColorTable.width(), 1);
        maskTable = baseColorTable.copy(0, 0, baseColorTable.width(), 1);
        for (qint32 i = 0; i < baseColorTable.height(); i++)
        {
            items.append(tr("CO Style ") + QString::number(i));
        }
        m_pPredefinedStyles = new DropDownmenu(200, items, true);
        m_pPredefinedStyles->setPosition(Settings::getWidth() / 2 + 10, Settings::getHeight() - 70 - m_pPredefinedStyles->getHeight());
        m_pSpriteBox->addChild(m_pPredefinedStyles);
    }
    else
    {
        QImage src(m_ResFilePath + "+nrm.png");
        baseColorTable = SpriteCreator::createColorTable(src);
        colorTable = baseColorTable.copy(0, 0, baseColorTable.width(), 1);
        maskTable = baseColorTable.copy(0, 0, baseColorTable.width(), 1);
        QVector<QString> items;
        items.append(tr("CO Style ") + QString::number(0));
        m_pPredefinedStyles = new DropDownmenu(200, items, true);
        m_pPredefinedStyles->setPosition(Settings::getWidth() / 2 + 10, Settings::getHeight() - 70 - m_pPredefinedStyles->getHeight());
        m_pSpriteBox->addChild(m_pPredefinedStyles);
    }
    connect(m_pPredefinedStyles.get(), &DropDownmenu::sigItemChanged, [=](qint32 item)
    {
        pApp->suspendThread();
        maskTable = baseColorTable.copy(0, item, baseColorTable.width(), 1);
        for (qint32 i = 0; i < maskTable.width(); i++)
        {
            QColor color = maskTable.pixelColor(i, 0);
            m_Pixels[i]->setColor(color.red(), color.green(), color.blue(), 255);
        }
        m_update = true;
        pApp->continueThread();
    });
    m_pPixelPanel->clearContent();
    m_Pixels.clear();
    qint32 y = 10;
    qint32 xStep = 0;

    m_PixelsSelector = new oxygine::ColorRectSprite();
    m_PixelsSelector->setColor(oxygine::Color(32, 200, 32, 255));
    m_PixelsSelector->setSize(20, 20);
    m_PixelsSelector->setPosition(xStep * 22 - 2 + 5, 10 - 2);
    m_PixelsSelector->setPriority(-1);
    m_pPixelPanel->addItem(m_PixelsSelector);
    for (qint32 i = 0; i < maskTable.width(); i++)
    {
        oxygine::spColorRectSprite pixel = new oxygine::ColorRectSprite();
        pixel->setSize(16, 16);
        QColor color = maskTable.pixelColor(i, 0);
        pixel->setColor(color.red(), color.green(), color.blue(), 255);
        pixel->setPosition(xStep * 22 + 5, y);
        pixel->addClickListener([=](oxygine::Event* pEvent)
        {
            pEvent->stopPropagation();
            m_PixelsSelector->setPosition(xStep * 22 - 2 + 5, y - 2);
            currentPixel = i;

        });

        m_pPixelPanel->addItem(pixel);
        m_Pixels.append(pixel);
        xStep++;
        if (xStep * 22 + 60 + 5 > Settings::getWidth() - 100)
        {
            xStep = 0;
            y += 22;
        }
    }
    m_pPixelPanel->setContentHeigth(y + 40);
    pApp->continueThread();
}

void DialogCOStyle::addCOStyle(QString style, bool select)
{
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("topbar+dropdown");
    oxygine::spBox9Sprite pBox = new oxygine::Box9Sprite();
    pBox->setResAnim(pAnim);
    pAnim = pCOSpriteManager->getResAnim((m_currentCOID + style + "+nrm").toStdString().c_str());
    float scale = (m_pCOPanel->getHeight() - 120) / pAnim->getHeight();
    pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pBox->setSize(scale * pAnim->getWidth() + 20, scale * pAnim->getHeight() + 40);
    pBox->setPosition(pBox->getWidth() * m_pCOSprites.size() + 20, 20);
    pBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    m_pCOPanel->addItem(pBox);
    // add some event handling :)
    qint32 index = m_pCOBoxes.size();
    pBox->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        for (qint32 i = 0; i < m_pCOBoxes.size(); i++)
        {
            if (m_pCOBoxes[i].get() != pBox.get())
            {
                m_pCOBoxes[i]->addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(0, 0, 0, 0)), 300);
            }
        }
        pBox->addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(32, 200, 32, 0)), 300);
        emit sigCOStyleChanged(index);
    });
    if (select)
    {
        for (qint32 i = 0; i < m_pCOBoxes.size(); i++)
        {
            if (m_pCOBoxes[i].get() != pBox.get())
            {
                m_pCOBoxes[i]->addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(0, 0, 0, 0)), 300);
            }
        }
        pBox->setAddColor(oxygine::Color(32, 200, 32, 0));
    }
    oxygine::spSprite pCO = new oxygine::Sprite();
    pCO->setResAnim(pAnim);
    pCO->setScale(scale);
    pCO->setPosition(10, 10);
    pBox->addChild(pCO);
    m_pCOPanel->setContentWidth(pBox->getX() + pBox->getWidth() + 50);

    m_pCOSprites.append(pCO);
    m_pCOBoxes.append(pBox);
    m_pResAnims.append(nullptr);
}

void DialogCOStyle::update(const oxygine::UpdateState& us)
{
    if (m_update)
    {
        Mainapp* pApp = Mainapp::getInstance();
        COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
        oxygine::ResAnim* pAnim = pCOSpriteManager->getResAnim((m_currentCOID + m_Styles[m_CurrentIndex] + "+nrm").toStdString().c_str());
        m_pResAnims[m_CurrentIndex] = SpriteCreator::createAnim(m_ResFilePath + "+nrm.png", colorTable, maskTable, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor());
        m_pCOSprites[m_CurrentIndex]->setResAnim(m_pResAnims[m_CurrentIndex].get());
        m_update = false;
    }
    oxygine::Actor::update(us);
}
