#include "campaigneditor.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "game/gamemap.h"

#include "objects/filedialog.h"

#include "objects/folderdialog.h"

#include "objects/checkbox.h"

#include "qdir.h"

#include "qfile.h"

#include "qtextstream.h"

const QString CampaignEditor::campaign = "campaign";
const QString CampaignEditor::campaignName = "campaignName";
const QString CampaignEditor::campaignDescription = "campaignDescription";
const QString CampaignEditor::campaignAuthor = "campaignAuthor";
const QString CampaignEditor::campaignMaps = "campaignMaps";
const QString CampaignEditor::campaignMapsFolder = "campaignMapsFolder";
const QString CampaignEditor::campaignMapNames = "campaignMapNames";
const QString CampaignEditor::campaignMapEnabled = "campaignMapEnabled";
const QString CampaignEditor::campaignMapDisabled = "campaignMapDisabled";
const QString CampaignEditor::campainMapAdd = "campainMapAdd";
const QString CampaignEditor::campainMapFinished = "campainMapFinished";
const QString CampaignEditor::campaignFinished = "campaignFinished";

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

    qint32 y = 30;
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    oxygine::spTextField pText = new  oxygine::TextField();
    pText->setStyle(style);
    pText->setText(tr("Campaign Folder:").toStdString().c_str());
    pText->setPosition(30, y);
    pSpriteBox->addChild(pText);
    m_CampaignFolder = new Textbox(pApp->getSettings()->getWidth() - 500);
    m_CampaignFolder->setPosition(300, y);
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

    y += 40;
    pText = new  oxygine::TextField();
    pText->setStyle(style);
    pText->setText(tr("Campaign Name:").toStdString().c_str());
    pText->setPosition(30, y);
    pSpriteBox->addChild(pText);
    m_Name = new Textbox(pApp->getSettings()->getWidth() - 500);
    m_Name->setPosition(300, y);
    m_Name->setCurrentText("");
    pSpriteBox->addChild(m_Name);

    y += 40;
    pText = new  oxygine::TextField();
    pText->setStyle(style);
    pText->setText(tr("Campaign Author:").toStdString().c_str());
    pText->setPosition(30, y);
    pSpriteBox->addChild(pText);
    m_Author = new Textbox(pApp->getSettings()->getWidth() - 500);
    m_Author->setPosition(300, y);
    m_Author->setCurrentText(Settings::getUsername());
    pSpriteBox->addChild(m_Author);

    y += 40;
    pText = new  oxygine::TextField();
    pText->setStyle(style);
    pText->setText(tr("Campaign Description:").toStdString().c_str());
    pText->setPosition(30, y);
    pSpriteBox->addChild(pText);
    m_Description = new Textbox(pApp->getSettings()->getWidth() - 500);
    m_Description->setPosition(300, y);
    m_Description->setCurrentText("");
    pSpriteBox->addChild(m_Description);

    y += 40;
    QSize size(pApp->getSettings()->getWidth() - 80, pApp->getSettings()->getHeight() - 280);
    m_Panel = new Panel(true, size, size);
    m_Panel->setPosition(40, y);
    pSpriteBox->addChild(m_Panel);

    // add campaign
    oxygine::spButton pAddCampaignButton = pObjectManager->createButton(tr("add Campaign"), 150);
    pAddCampaignButton->setPosition(30, pApp->getSettings()->getHeight() - 30 - pAddCampaignButton->getHeight());
    pSpriteBox->addChild(pAddCampaignButton);
    pAddCampaignButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigShowAddCampaign();
    });

    // load campaign
    oxygine::spButton pLoadCampaignButton = pObjectManager->createButton(tr("load Campaign"), 150);
    pLoadCampaignButton->setPosition(pApp->getSettings()->getWidth() / 2 - 10 - pLoadCampaignButton->getWidth(), pApp->getSettings()->getHeight() - 30 - pLoadCampaignButton->getHeight());
    pSpriteBox->addChild(pLoadCampaignButton);
    pLoadCampaignButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigShowLoadCampaign();
    });

    // save campaign
    oxygine::spButton pSaveCampaignButton = pObjectManager->createButton(tr("save Campaign"), 150);
    pSaveCampaignButton->setPosition(pApp->getSettings()->getWidth() / 2 + 10, pApp->getSettings()->getHeight() - 30 - pSaveCampaignButton->getHeight());
    pSpriteBox->addChild(pSaveCampaignButton);
    pSaveCampaignButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigShowSaveCampaign();
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
    connect(this, &CampaignEditor::sigShowSaveCampaign, this, &CampaignEditor::showSaveCampaign, Qt::QueuedConnection);
    connect(this, &CampaignEditor::sigShowLoadCampaign, this, &CampaignEditor::showLoadCampaign, Qt::QueuedConnection);
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

void CampaignEditor::showSaveCampaign()
{
    QVector<QString> wildcards;
    wildcards.append("*.jsm");
    QString path = "maps/";
    spFileDialog fileDialog = new FileDialog(path, wildcards, m_Name->getCurrentText());
    this->addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &CampaignEditor::saveCampaign, Qt::QueuedConnection);
}

void CampaignEditor::showLoadCampaign()
{
    QVector<QString> wildcards;
    wildcards.append("*.jsm");
    QString path = "maps/";
    spFileDialog fileDialog = new FileDialog(path, wildcards, "");
    this->addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &CampaignEditor::loadCampaign, Qt::QueuedConnection);
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

void CampaignEditor::loadCampaign(QString filename)
{
    if (filename.endsWith(".jsm"))
    {
        QFile file(filename);
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        bool started = false;
        while (!file.atEnd())
        {
            QString line = stream.readLine().simplified();
            if (line.endsWith(campaign))
            {
                started = true;
                if (started)
                {
                    started = false;
                    break;
                }
            }
            else if (started)
            {
                if (line.endsWith(campaignName))
                {

                }
            }
        }
    }
}

void CampaignEditor::saveCampaign(QString filename)
{
    if (filename.endsWith(".jsm"))
    {
        QFile file(filename);
        file.open(QIODevice::WriteOnly);
        QTextStream stream(&file);
        stream << "var Constructor = function() { // " << campaign << "\n";

        stream << "    this.getCampaignName = function() { //" << campaignName << "\n";
        stream << "        return qsTr(\"" << m_Name->getCurrentText() << "\")\n";
        stream << "    }; //" << campaignName << "\n";

        stream << "    this.getAuthor = function() { // " << campaignAuthor << "\n";
        stream << "        return qsTr(\"" << m_Author->getCurrentText() << "\")\n";
        stream << "    }; // " << campaignAuthor << "\n";

        stream << "    this.getDescription = function() { // " << campaignDescription << "\n";
        stream << "        return qsTr(\"" << m_Description->getCurrentText() << "\")\n";
        stream << "    }; // " << campaignDescription << "\n";

        stream << "    this.getCurrentCampaignMaps = function(campaign) { // " << campaignMaps << "\n";
        stream << "         var ret = [\"" << m_CampaignFolder->getCurrentText() << "\"] // " << campaignMapsFolder << "\n";
        for (qint32 i = 0; i < mapDatas.size(); i++)
        {
            stream << "        var map" << QString::number(i) << "Won = variables.createVariable(\"" << mapDatas[i].mapName << "\"); // " << campaignMapNames << "\n";
        }
        for (qint32 i = 0; i < mapDatas.size(); i++)
        {
            stream << "        var map" << QString::number(i) << "EnableCount = 0; // " << campaignMapEnabled << "\n";
            for (qint32 i2 = 0; i2 < mapDatas[i].previousMaps.size(); i2++)
            {
                stream << "        if (map" << QString::number(i2) << "Won.readDataBool() === true) { map" << QString::number(i) << "EnableCount++;} /n";
            }
            stream << "// " << campaignMapEnabled << "\n";

            stream << "        var map" << QString::number(i) << "DisableCount = 0; // " << campaignMapDisabled << "\n";
            for (qint32 i2 = 0; i2 < mapDatas[i].disableMaps.size(); i2++)
            {
                stream << "        if (map" << QString::number(i2) << "Won.readDataBool() === true) { map" << QString::number(i) << "DisableCount++;} /n";
            }
            stream << "// " << campaignMapDisabled << "\n";
            stream << "        if (map" << QString::number(i) << "DisableCount < " << mapDatas[i].disableCount <<
                      " && map" << QString::number(i) << "EnableCount >= " << mapDatas[i].previousCount << ") {ret.push(\"" << mapDatas[i].map << "\");} // " << campainMapAdd << "\n";
        }

        stream << "        return ret;\n";
        stream << "    }; // " << campaignMaps << "\n";

        stream << "    this.mapFinished = function(campaign, map, result) { // " << campainMapFinished << "\n";
        stream << "        var variables = campaign.getVariables();\n";
        stream << "        var mapVar = variables.createVariable(map.getMapName());\n";
        stream << "        mapVar.writeDataBool(result);\n";
        stream << "    }; // " << campainMapFinished << "\n";

        stream << "// " << campaign << "\n"  << "};\n" <<
                  "Constructor.prototype = BASECAMPAIGN;\n" <<
                  "var campaignScript = new Constructor();";
    }
}
