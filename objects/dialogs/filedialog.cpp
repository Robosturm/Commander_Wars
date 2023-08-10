#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/res/SingleResAnim.h"

#include "objects/dialogs/filedialog.h"
#include "objects/dialogs/dialogmessagebox.h"
#include "objects/base/label.h"

#include "coreengine/mainapp.h"
#include "coreengine/globalutils.h"
#include "coreengine/interpreter.h"

#include "resource_management/objectmanager.h"


const char* const ROOT = "::::";

FileDialog::FileDialog(QString startFolder, const QStringList & wildcards, bool isSaveDialog, QString startFile, bool preview, QString acceptButtonName)
    : m_preview(preview),
      m_pathPrefix(Settings::getInstance()->getUserPath()),
      m_isSaveDialog(isSaveDialog)
{
#ifdef GRAPHICSUPPORT
    setObjectName("FileDialog");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    Interpreter::setCppOwnerShip(this);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("filedialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // current folder
    m_CurrentFolder = spTextbox::create(oxygine::Stage::getStage()->getWidth() - 60);
    m_CurrentFolder->setPosition(30, 30);
    pSpriteBox->addChild(m_CurrentFolder);
    m_CurrentFolder->setCurrentText(startFolder);
    connect(m_CurrentFolder.get(), &Textbox::sigTextChanged, this, &FileDialog::showFolder, Qt::QueuedConnection);
    // folder file selection
    m_MainPanel = spPanel::create(true, QSize(oxygine::Stage::getStage()->getWidth() - 60, oxygine::Stage::getStage()->getHeight() - 210), QSize(oxygine::Stage::getStage()->getWidth() - 60, oxygine::Stage::getStage()->getHeight() - 300));
    m_MainPanel->setPosition(30, 30 + m_CurrentFolder->getScaledHeight() + 10);
    pSpriteBox->addChild(m_MainPanel);
    // file folder
    m_CurrentFile = spTextbox::create(oxygine::Stage::getStage()->getWidth() - 60 - 160);
    m_CurrentFile->setPosition(30, m_MainPanel->getY() + m_MainPanel->getScaledHeight() + 45);
    m_CurrentFile->setCurrentText(startFile);
    pSpriteBox->addChild(m_CurrentFile);
    // ok button
    m_OkButton = pObjectManager->createButton(acceptButtonName, 150);
    m_OkButton->setPosition(m_CurrentFile->getScaledWidth() + 30 + 10, m_CurrentFile->getY());
    pSpriteBox->addChild(m_OkButton);
    // delete button
    oxygine::spButton pDeleteButton = pObjectManager->createButton(tr("Delete"), 150);
    pDeleteButton->setPosition(m_CurrentFile->getScaledWidth() + 30 + 10, m_CurrentFile->getY() - 40);
    pDeleteButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigShowDeleteQuestion();
    });
    pSpriteBox->addChild(pDeleteButton);
    // drop down menu
    m_DropDownmenu = spDropDownmenu::create(m_CurrentFile->getScaledWidth(), wildcards);
    pSpriteBox->addChild(m_DropDownmenu);
    m_DropDownmenu->setPosition(30, m_CurrentFile->getY() + m_CurrentFile->getScaledHeight());
    connect(m_DropDownmenu.get(), &DropDownmenu::sigItemChanged, this, &FileDialog::filterChanged, Qt::QueuedConnection);
    // cancel button
    m_CancelButton = pObjectManager->createButton(tr("Cancel"), 150);
    m_CancelButton->setPosition(m_DropDownmenu->getScaledWidth() + 30 + 10, m_DropDownmenu->getY());
    pSpriteBox->addChild(m_CancelButton);
    m_CancelButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigCancel();
    });
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        if (m_isSaveDialog &&
            (QFile::exists(m_CurrentFolder->getCurrentText() + "/" + m_CurrentFile->getCurrentText()) ||
             QFile::exists(Settings::getInstance()->getUserPath() + m_CurrentFolder->getCurrentText() + "/" + m_CurrentFile->getCurrentText())))
        {
            emit sigShowOverwriteWarning();
        }
        else
        {
            onFileSelected();
        }
    });
    connect(this, &FileDialog::sigShowOverwriteWarning, this, &FileDialog::showOverwriteWarning, Qt::QueuedConnection);
    connect(this, &FileDialog::sigShowFolder, this, &FileDialog::showFolder, Qt::QueuedConnection);
    showFolder(startFolder);
    connect(pApp, &Mainapp::sigKeyDown, this, &FileDialog::KeyInput, Qt::QueuedConnection);
    connect(this, &FileDialog::sigCancel, this, &FileDialog::remove, Qt::QueuedConnection);
    connect(this, &FileDialog::sigFinished, this, &FileDialog::remove, Qt::QueuedConnection);
    connect(this, &FileDialog::sigShowDeleteQuestion, this, &FileDialog::showDeleteQuestion, Qt::QueuedConnection);
    pApp->continueRendering();
}

void FileDialog::showOverwriteWarning()
{
    m_focused = false;
    spDialogMessageBox pDialogOverwrite = spDialogMessageBox::create(tr("Do you want to overwrite the file ") + m_CurrentFolder->getCurrentText() + "/" +
                                                                  m_CurrentFile->getCurrentText() + "?", true);
    connect(pDialogOverwrite.get(), &DialogMessageBox::sigOk, this, &FileDialog::onFileSelected, Qt::QueuedConnection);
    connect(pDialogOverwrite.get(), &DialogMessageBox::sigCancel, this, [this]()
    {
        m_focused = true;
    });
    addChild(pDialogOverwrite);
}

void FileDialog::onFileSelected()
{
    auto* pCurrentFolder = m_CurrentFolder.get();
    auto* pPtrCurrentFile = m_CurrentFile.get();
    auto* pPtrDropDownmenu = m_DropDownmenu.get();
    QString fileStart = m_pathPrefix + pCurrentFolder->getCurrentText();
    if (!fileStart.isEmpty())
    {
        fileStart += "/";
    }
    QString file = fileStart + pPtrCurrentFile->getCurrentText();
    QStringList items = pPtrDropDownmenu->getCurrentItemText().split((";"));
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
    emit sigFileSelected(file);
    emit sigFinished();
}

void FileDialog::remove()
{
    detach();
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
    CONSOLE_PRINT("Showing folder: " + folder, GameConsole::eDEBUG);
    folder = folder.replace("\\", "/");
    while (folder.contains("//"))
    {
        folder = folder.replace("//", "/");
    }
    folder = QDir(folder).absolutePath();
    folder = GlobalUtils::makePathRelative(folder);
    m_Items.clear();
    m_ResAnims.clear();

    QDir dir(Settings::getInstance()->getUserPath() + folder);
    QDir virtDir(oxygine::Resource::RCC_PREFIX_PATH + folder);
    if (!dir.exists() && !virtDir.exists())
    {
        if (!folder.isEmpty())
        {
            CONSOLE_PRINT("Using root cause given folder wasn't found: " + folder, GameConsole::eINFO);
        }
        folder = ROOT;
    }
    // we want the root folder
    QFileInfoList infoList;
    if (folder == ROOT)
    {
        infoList = QDir::drives();
    }
    else
    {
        QStringList list = m_DropDownmenu->getCurrentItemText().split(";");
        infoList = GlobalUtils::getInfoList(folder, list);
    }
    qint32 itemCount = 0;
    for (qint32 i = 0; i < infoList.size(); i++)
    {
        QString myPath;
        if (folder == ROOT)
        {
            myPath = infoList[i].canonicalFilePath();
        }
        else if (infoList[i].canonicalFilePath() != QCoreApplication::applicationDirPath() &&
                 infoList[i].canonicalFilePath() != QCoreApplication::applicationDirPath() + "/")
        {
            myPath = GlobalUtils::makePathRelative(infoList[i].canonicalFilePath());
        }
        else
        {
            myPath = infoList[i].canonicalFilePath();
        }
        if (myPath == folder)
        {
            // skip ourself
            continue;
        }
        ObjectManager* pObjectManager = ObjectManager::getInstance();
        oxygine::ResAnim* pAnim = pObjectManager->getResAnim("filedialogitems");
        oxygine::spBox9Sprite pBox = oxygine::spBox9Sprite::create();
        pBox->setResAnim(pAnim);
        pBox->setSize(m_MainPanel->getScaledWidth() - 70, 40);
        pBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
        // add some event handling :)
        auto* pPtrBox = pBox.get();
        pBox->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event*)
        {
            pPtrBox->addTween(oxygine::Sprite::TweenAddColor(QColor(32, 200, 32, 0)), oxygine::timeMS(300));
        });
        pBox->addEventListener(oxygine::TouchEvent::OUTX, [=](oxygine::Event*)
        {
            pPtrBox->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
        });
        pBox->setPosition(0, itemCount * 40);

        spLabel textField = spLabel::create(pBox->getScaledWidth() - 18);
        textField->setX(13);
        textField->setY(5);
        pBox->addChild(textField);

        // loop through all entries :)
        if (infoList[i].isDir())
        {
            if (folder == ROOT)
            {
                textField->setHtmlText(infoList[i].canonicalFilePath());
            }
            else
            {
                QString path = GlobalUtils::makePathRelative(infoList[i].filePath()).replace(folder, "");
                textField->setHtmlText(path);
            }
            pBox->addEventListener(oxygine::TouchEvent::CLICK, [this, myPath](oxygine::Event*)
            {
                emit sigShowFolder(myPath);
            });
        }
        else if (infoList[i].isFile())
        {
            QString fullPath = infoList[i].canonicalFilePath();
            QString file = infoList[i].fileName();
            textField->setHtmlText(file);
            auto* pCurrentFile = m_CurrentFile.get();
            pBox->addEventListener(oxygine::TouchEvent::CLICK, [this, fullPath, pCurrentFile, file](oxygine::Event*)
            {
                if (fullPath.startsWith(oxygine::Resource::RCC_PREFIX_PATH))
                {
                    m_pathPrefix = oxygine::Resource::RCC_PREFIX_PATH;
                }
                else
                {
                    m_pathPrefix = Settings::getInstance()->getUserPath();
                }
                pCurrentFile->setCurrentText(file);
            });
            if (m_preview)
            {
                QImage img(infoList[i].filePath());
                oxygine::spSingleResAnim pAnim = oxygine::spSingleResAnim::create();
                Mainapp::getInstance()->loadResAnim(pAnim, img, 1, 1, 1);
                m_ResAnims.append(pAnim);
                oxygine::spSprite pSprite = oxygine::spSprite::create();
                pSprite->setResAnim(pAnim.get());
                if (pAnim-> getWidth() > 0)
                {
                    pSprite->setScaleX(30.0f / static_cast<float>(pAnim->getWidth()));
                }
                if (pAnim->getHeight() > 0)
                {
                    pSprite->setScaleY(30.0f / static_cast<float>(pAnim->getHeight()));
                }
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
        m_MainPanel->addItem(pBox);
        itemCount++;
    }
    m_MainPanel->setContentHeigth(itemCount * 40 + 50);
    if (folder == ROOT)
    {
        m_CurrentFolder->setCurrentText("");
    }
    else
    {
        m_CurrentFolder->setCurrentText(folder);
    }
    pApp->continueRendering();
}

void FileDialog::showDeleteQuestion()
{
    if (QFile::exists(m_CurrentFolder->getCurrentText() + "/" +
                      m_CurrentFile->getCurrentText()))
    {
        m_focused = false;
        spDialogMessageBox pDialogRemove = spDialogMessageBox::create(tr("Do you want to delete the item ") + m_CurrentFolder->getCurrentText() + "/" +
                                                                      m_CurrentFile->getCurrentText() + "?", true);
        connect(pDialogRemove.get(), &DialogMessageBox::sigOk, this, &FileDialog::deleteItem, Qt::QueuedConnection);
        connect(pDialogRemove.get(), &DialogMessageBox::sigCancel, this, [this]()
        {
            m_focused = true;
        });
        addChild(pDialogRemove);
    }
}

void FileDialog::deleteItem()
{
    if (QFile::exists(m_CurrentFolder->getCurrentText() + "/" + m_CurrentFile->getCurrentText()))
    {
        QFile::remove(m_CurrentFolder->getCurrentText() + "/" + m_CurrentFile->getCurrentText());
    }
    else if (QFile::exists(Settings::getInstance()->getUserPath() + m_CurrentFolder->getCurrentText() + "/" + m_CurrentFile->getCurrentText()))
    {
        QFile::remove(Settings::getInstance()->getUserPath() + m_CurrentFolder->getCurrentText() + "/" + m_CurrentFile->getCurrentText());
    }
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
                    spDialogMessageBox pDialogRemove = spDialogMessageBox::create(tr("Do you want to delete the item ") + m_CurrentFolder->getCurrentText() + "/" +
                                                                                  m_CurrentFile->getCurrentText() + "?", true);
                    connect(pDialogRemove.get(), &DialogMessageBox::sigOk, this, &FileDialog::deleteItem, Qt::QueuedConnection);
                    connect(pDialogRemove.get(), &DialogMessageBox::sigCancel, this, [this]()
                    {
                        m_focused = true;
                    });
                    addChild(pDialogRemove);
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
