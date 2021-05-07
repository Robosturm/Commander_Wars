#include "filedialog.h"

#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"
#include "objects/dialogs/dialogmessagebox.h"

#include "QDir"

FileDialog::FileDialog(QString startFolder, QVector<QString> wildcards, QString startFile, bool preview)
    : m_preview(preview)
{
    setObjectName("FileDialog");
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("filedialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // current folder
    m_CurrentFolder = spTextbox::create(Settings::getWidth() - 60);
    m_CurrentFolder->setPosition(30, 30);
    pSpriteBox->addChild(m_CurrentFolder);
    m_CurrentFolder->setCurrentText(startFolder);
    connect(m_CurrentFolder.get(), &Textbox::sigTextChanged, this, &FileDialog::showFolder, Qt::QueuedConnection);
    // folder file selection
    m_MainPanel = spPanel::create(true, QSize(Settings::getWidth() - 60, Settings::getHeight() - 210), QSize(Settings::getWidth() - 60, Settings::getHeight() - 300));
    m_MainPanel->setPosition(30, 30 + m_CurrentFolder->getHeight() + 10);
    pSpriteBox->addChild(m_MainPanel);
    // file folder
    m_CurrentFile = spTextbox::create(Settings::getWidth() - 60 - 160);
    m_CurrentFile->setPosition(30, m_MainPanel->getY() + m_MainPanel->getHeight() + 10);
    m_CurrentFile->setCurrentText(startFile);
    pSpriteBox->addChild(m_CurrentFile);
    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(m_CurrentFile->getWidth() + 30 + 10, m_CurrentFile->getY());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        QString file = m_CurrentFolder->getCurrentText() + "/" + m_CurrentFile->getCurrentText();

        QStringList items = m_DropDownmenu->getCurrentItemText().split((";"));
        for (qint32 i = 0; i < items.size(); i++)
        {
            items[i] = items[i].replace("*", "");
        }
        bool found = false;
        for (qint32 i = 0; i < items.size(); i++)
        {
            if (file.endsWith(items[i]))
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            file += items[0];
        }
        QDir folder(m_CurrentFolder->getCurrentText());
        if (folder.exists())
        {
            emit sigFileSelected(file);
        }
        detach();
    });
    // drop down menu
    m_DropDownmenu = spDropDownmenu::create(m_CurrentFile->getWidth(), wildcards);
    pSpriteBox->addChild(m_DropDownmenu);
    m_DropDownmenu->setPosition(30, m_CurrentFile->getY() + m_CurrentFile->getHeight() + 10);
    connect(m_DropDownmenu.get(), &DropDownmenu::sigItemChanged, this, &FileDialog::filterChanged, Qt::QueuedConnection);
    // cancel button
    m_CancelButton = pObjectManager->createButton(tr("Cancel"), 150);
    m_CancelButton->setPosition(m_DropDownmenu->getWidth() + 30 + 10, m_DropDownmenu->getY());
    pSpriteBox->addChild(m_CancelButton);
    m_CancelButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigCancel();
        detach();
    });

    // go folder up
    pAnim = pObjectManager->getResAnim("filedialogitems");
    oxygine::spBox9Sprite pBox = oxygine::spBox9Sprite::create();
    pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pBox->setResAnim(pAnim);
    oxygine::spTextField textField = oxygine::spTextField::create();
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    textField->setStyle(style);
    textField->setHtmlText("..");
    textField->attachTo(pBox);
    pBox->setSize(m_MainPanel->getWidth() - 50, 40);
    textField->setHeight(40);
    textField->setWidth(pBox->getWidth() - 18);
    textField->setX(8);
    pBox->setPosition(0, 0);
    pBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    this->m_MainPanel->addItem(pBox);
    // add some event handling :)
    pBox->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        pBox->addTween(oxygine::Sprite::TweenAddColor(QColor(32, 200, 32, 0)), oxygine::timeMS(300));
    });
    pBox->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        pBox->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    pBox->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        QDir dir(m_CurrentFolder->getCurrentText());
        if (dir.cdUp() && m_CurrentFolder->getCurrentText() != "")
        {
            emit sigShowFolder(dir.absolutePath());
        }
        else
        {
            emit sigShowFolder("");
        }
    });
    connect(this, &FileDialog::sigShowFolder, this, &FileDialog::showFolder, Qt::QueuedConnection);
    showFolder(startFolder);
    connect(pApp, &Mainapp::sigKeyDown, this, &FileDialog::KeyInput, Qt::QueuedConnection);
    pApp->continueRendering();
}

FileDialog::~FileDialog()
{
}

void FileDialog::filterChanged(qint32)
{
    showFolder(m_CurrentFolder->getCurrentText());
}

bool FileDialog::getPreview() const
{
    return m_preview;
}

void FileDialog::setPreview(bool preview)
{
    m_preview = preview;
}

void FileDialog::showFolder(QString folder)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    // clean up current panel items
    for (qint32 i = 0; i < m_Items.size(); i++)
    {
        m_MainPanel->removeItem(m_Items[i]);
    }

    folder = folder.replace("\\", "/");
    m_Items.clear();
    m_ResAnims.clear();

    QDir dir(folder);
    if (!dir.exists())
    {
        folder = "";
    }

    // this is the we have to do all the work function of the file dialog...
    // we want the root folder
    QFileInfoList infoList;
    if (folder == "")
    {
        infoList = QDir::drives();
    }
    else
    {
        QString list = m_DropDownmenu->getCurrentItemText();
        infoList.append(QDir(folder).entryInfoList(QDir::Dirs));
        infoList.append(QDir(folder).entryInfoList(list.split(";"), QDir::Files));
    }
    qint32 itemCount = 0;
    for (qint32 i = 1; i < infoList.size(); i++)
    {
        QString myPath = infoList[i].absoluteFilePath();
        if (myPath == folder)
        {
            // skip ourself
            continue;
        }
        ObjectManager* pObjectManager = ObjectManager::getInstance();
        oxygine::ResAnim* pAnim = pObjectManager->getResAnim("filedialogitems");
        oxygine::spBox9Sprite pBox = oxygine::spBox9Sprite::create();
        pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
        pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
        pBox->setResAnim(pAnim);
        oxygine::spTextField textField = oxygine::spTextField::create();
        oxygine::TextStyle style = FontManager::getMainFont24();
        style.color = FontManager::getFontColor();
        style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
        style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        style.multiline = false;
        textField->setStyle(style);

        textField->attachTo(pBox);
        pBox->setSize(m_MainPanel->getWidth() - 50, 40);
        textField->setHeight(40);
        textField->setWidth(pBox->getWidth() - 18);
        textField->setX(13);
        textField->setY(5);
        pBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
        this->m_MainPanel->addItem(pBox);
        // add some event handling :)
        pBox->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
        {
            pBox->addTween(oxygine::Sprite::TweenAddColor(QColor(32, 200, 32, 0)), oxygine::timeMS(300));
        });
        pBox->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
        {
            pBox->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
        });
        pBox->setPosition(0, 40 + itemCount * 40);

        // loop through all entries :)
        if (infoList[i].isDir())
        {
            textField->setHtmlText(infoList[i].absoluteFilePath().replace(folder, ""));
            pBox->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
            {
                emit sigShowFolder(myPath);
            });
        }
        else if (infoList[i].isFile())
        {
            QString file = infoList[i].fileName();
            textField->setHtmlText(file);
            pBox->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
            {
                m_CurrentFile->setCurrentText(file);
            });
            if (m_preview)
            {
                QImage img(infoList[i].filePath());
                oxygine::spSingleResAnim pAnim = oxygine::spSingleResAnim::create();
                Mainapp::getInstance()->loadResAnim(pAnim, img);
                m_ResAnims.append(pAnim);
                oxygine::spSprite pSprite = oxygine::spSprite::create();
                pSprite->setResAnim(pAnim.get());
                pSprite->setScaleX(30.0f / pAnim->getWidth());
                pSprite->setScaleY(30.0f / pAnim->getHeight());
                pSprite->setPosition(8, 5);
                pBox->addChild(pSprite);
                textField->setX(53);
            }
        }
        else
        {
            // not possible i hope
        }
        m_Items.append(pBox);
        itemCount++;
    }
    m_MainPanel->setContentHeigth(itemCount * 40 + 50);
    m_CurrentFolder->setCurrentText(folder);
    pApp->continueRendering();
}

void FileDialog::deleteItem()
{
    QFile::remove(m_CurrentFolder->getCurrentText() + "/" +
                  m_CurrentFile->getCurrentText());
    showFolder(m_CurrentFolder->getCurrentText());
    m_focused = true;
}

void FileDialog::KeyInput(oxygine::KeyEvent event)
{
    // for debugging
    Qt::Key cur = event.getKey();
    if (!m_CurrentFolder->getFocused() &&
        !m_CurrentFile->getFocused() &&
        m_focused)
    {
        switch(cur)
        {
            case Qt::Key_Delete:
            {
                if (QFile::exists(m_CurrentFolder->getCurrentText() + "/" +
                                  m_CurrentFile->getCurrentText()))
                {
                    m_focused = false;
                    spDialogMessageBox pSurrender = spDialogMessageBox::create(tr("Do you want to delete the item ") + m_CurrentFolder->getCurrentText() + "/" +
                                                                               m_CurrentFile->getCurrentText() + "?", true);
                    connect(pSurrender.get(), &DialogMessageBox::sigOk, this, &FileDialog::deleteItem, Qt::QueuedConnection);
                    connect(pSurrender.get(), &DialogMessageBox::sigCancel, [=]()
                    {
                        m_focused = true;
                    });
                    addChild(pSurrender);
                }
                break;
            }
            default:
            {
                break;
            }
        }
        
    }
}
