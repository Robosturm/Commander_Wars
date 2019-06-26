#include "campaigneditor.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "game/gamemap.h"

#include "objects/filedialog.h"

#include "objects/folderdialog.h"

#include "objects/checkbox.h"

#include "qdir.h"

CampaignEditor::CampaignEditor()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("semidialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(pApp->getSettings()->getWidth(), pApp->getSettings()->getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Dialogs));


    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    oxygine::spTextField pText = new  oxygine::TextField();
    pText->setStyle(style);
    pText->setText(tr("Campaign Folder:").toStdString().c_str());
    pText->setPosition(30, 30);
    pSpriteBox->addChild(pText);
    m_CampaignFolder = new Textbox(pApp->getSettings()->getWidth() - 400);
    m_CampaignFolder->setPosition(200, 30);
    m_CampaignFolder->setCurrentText("maps/");
    pSpriteBox->addChild(m_CampaignFolder);
    // Campaign Button
    oxygine::spButton pCampaignButton = pObjectManager->createButton(tr("Select Folder"), 150);
    pCampaignButton->setPosition(pApp->getSettings()->getWidth() - pCampaignButton->getWidth() - 30, 30);
    pSpriteBox->addChild(pCampaignButton);
    pCampaignButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigShowSelectFolder();
    });

    QSize size(pApp->getSettings()->getWidth() - 80, pApp->getSettings()->getHeight() - 160);
    m_Panel = new Panel(true, size, size);
    m_Panel->setPosition(40, 80);
    pSpriteBox->addChild(m_Panel);

    // add campaign
    oxygine::spButton pAddCampaignButton = pObjectManager->createButton(tr("add Campaign"), 150);
    pAddCampaignButton->setPosition(30, pApp->getSettings()->getHeight() - 30 - pAddCampaignButton->getHeight());
    pSpriteBox->addChild(pAddCampaignButton);
    pAddCampaignButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigShowAddCampaign();
    });

    // ok button
    oxygine::spButton pOkButton = pObjectManager->createButton(tr("Ok"), 150);
    pOkButton->setPosition(pApp->getSettings()->getWidth() - pOkButton->getWidth() - 30, pApp->getSettings()->getHeight() - 30 - pOkButton->getHeight());
    pSpriteBox->addChild(pOkButton);
    pOkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigFinished();
        this->getParent()->removeChild(this);
    });

    connect(this, &CampaignEditor::sigShowAddCampaign, this, &CampaignEditor::showAddCampaign, Qt::QueuedConnection);
    connect(this, &CampaignEditor::sigShowSelectFolder, this, &CampaignEditor::showSelectFolder, Qt::QueuedConnection);
    connect(this, &CampaignEditor::sigUpdateCampaignData, this, &CampaignEditor::updateCampaignData, Qt::QueuedConnection);
}

void CampaignEditor::showAddCampaign()
{
    QVector<QString> wildcards;
    wildcards.append("*.map");
    QString path = QCoreApplication::applicationDirPath() + "/" + m_CampaignFolder->getCurrentText();
    spFileDialog fileDialog = new FileDialog(path, wildcards, "");
    this->addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &CampaignEditor::addCampaign, Qt::QueuedConnection);
}

void CampaignEditor::addCampaign(QString filename)
{
    QString fileData = filename.replace(QCoreApplication::applicationDirPath() + "/", "");
    if (fileData.endsWith(".map") &&
        fileData.startsWith(m_CampaignFolder->getCurrentText()))
    {
        QString map = fileData.replace(m_CampaignFolder->getCurrentText() + "/", "");
        MapData data;
        data.map = map;
        QFile file(filename);
        file.open(QIODevice::ReadOnly);
        QDataStream stream(&file);
        data.mapName = GameMap::readMapName(stream);
        file.close();
        mapDatas.append(data);
        updateCampaignData();
    }
}

void CampaignEditor::showSelectFolder()
{
    QString path = QCoreApplication::applicationDirPath() + "/maps";
    spFolderDialog folderDialog = new FolderDialog(path);
    this->addChild(folderDialog);
    connect(folderDialog.get(),  &FolderDialog::sigFolderSelected, this, &CampaignEditor::selectFolder, Qt::QueuedConnection);
}

void CampaignEditor::selectFolder(QString folder)
{
    folder = folder.replace(QCoreApplication::applicationDirPath() + "/", "");
    QDir dir(folder);
    if (dir.exists())
    {
        m_CampaignFolder->setCurrentText(folder);
        clearCampaignData();
    }
}

void CampaignEditor::clearCampaignData()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    mapDatas.clear();
    m_Panel->clearContent();
    pApp->continueThread();
}

void CampaignEditor::updateCampaignData()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_Panel->clearContent();
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    for (qint32 i = 0; i < mapDatas.size(); i++)
    {
        oxygine::TextStyle style = FontManager::getMainFont();
        style.color = oxygine::Color(255, 255, 255, 255);
        style.vAlign = oxygine::TextStyle::VALIGN_TOP;
        style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        style.multiline = false;
        oxygine::spTextField pText = new  oxygine::TextField();
        pText->setStyle(style);
        pText->setText(mapDatas[i].mapName.toStdString().c_str());
        pText->setPosition(10, 10 + i * 40);
        m_Panel->addItem(pText);

        oxygine::spButton pEnableButton = pObjectManager->createButton(tr("Enable Maps"), 150);
        pEnableButton->setPosition(200, 10 + i * 40);
        m_Panel->addItem(pEnableButton);
        pEnableButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
        });

        oxygine::spButton pDisableButton = pObjectManager->createButton(tr("Disable Maps"), 150);
        pDisableButton->setPosition(360, 10 + i * 40);
        m_Panel->addItem(pDisableButton);
        pDisableButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
        });

        oxygine::spButton pRemoveButton = pObjectManager->createButton(tr("Remove Map"), 150);
        pRemoveButton->setPosition(520, 10 + i * 40);
        m_Panel->addItem(pRemoveButton);
        pRemoveButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            mapDatas.removeAt(i);
            emit sigUpdateCampaignData();
        });

        pText = new  oxygine::TextField();
        pText->setStyle(style);
        pText->setText(tr("Last Map").toStdString().c_str());
        pText->setPosition(670, 10 + i * 40);
        m_Panel->addItem(pText);

        spCheckbox pBox = new Checkbox();
        pBox->setChecked(mapDatas[i].lastMap);
        pBox->setPosition(780, 10 + i * 40);
        m_Panel->addItem(pBox);
        connect(pBox.get(), &Checkbox::checkChanged, [=](bool value)
        {
            mapDatas[i].lastMap = value;
        });

    }
    m_Panel->setContentHeigth(mapDatas.size() * 40 + 40);
    m_Panel->setContentWidth(850);
    pApp->continueThread();
}
