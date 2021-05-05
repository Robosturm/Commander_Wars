#include "qdir.h"
#include "qfile.h"
#include "objects/base/label.h"

#include "ingamescriptsupport/campaigneditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "game/gamemap.h"

#include "objects/dialogs/filedialog.h"
#include "objects/dialogs/folderdialog.h"
#include "objects/base/checkbox.h"
#include "objects/base/spinbox.h"
#include "objects/dialogs/dialogmessagebox.h"

const QString CampaignEditor::campaign = "campaign";
const QString CampaignEditor::campaignName = "campaignName";
const QString CampaignEditor::campaignDescription = "campaignDescription";
const QString CampaignEditor::campaignAuthor = "campaignAuthor";
const QString CampaignEditor::campaignMaps = "campaignMaps";
const QString CampaignEditor::campaignMapsFolder = "campaignMapsFolder";
const QString CampaignEditor::campaignMapNames = "campaignMapNames";
const QString CampaignEditor::campaignMapEnabled = "campaignMapEnabled";
const QString CampaignEditor::campaignMapDisabled = "campaignMapDisabled";
const QString CampaignEditor::campaignMapAdd = "campaignMapAdd";
const QString CampaignEditor::campaignMapFinished = "campaignMapFinished";
const QString CampaignEditor::campaignFinished = "campaignFinished";

CampaignEditor::CampaignEditor()
    : QObject()
{
    setObjectName("CampaignEditor");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("semidialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    qint32 y = 30;
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    oxygine::spTextField pText =  oxygine::spTextField::create();
    pText->setStyle(style);
    pText->setHtmlText(tr("Folder:"));
    pText->setPosition(30, y);
    pSpriteBox->addChild(pText);
    m_CampaignFolder = spTextbox::create(Settings::getWidth() - 500);
    m_CampaignFolder->setTooltipText(tr("Folder containing the campaign maps. All maps for this campaign should be directly below this folder. The folder name must end with .camp"));
    m_CampaignFolder->setPosition(300, y);
    m_CampaignFolder->setCurrentText("maps/");
    pSpriteBox->addChild(m_CampaignFolder);
    // Campaign Button
    oxygine::spButton pCampaignButton = pObjectManager->createButton(tr("Folder"), 150);
    pCampaignButton->setPosition(Settings::getWidth() - pCampaignButton->getWidth() - 30, 30);
    pSpriteBox->addChild(pCampaignButton);
    pCampaignButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigShowSelectFolder();
    });

    y += 40;
    pText =  oxygine::spTextField::create();
    pText->setStyle(style);
    pText->setHtmlText(tr("Name:"));
    pText->setPosition(30, y);
    pSpriteBox->addChild(pText);
    m_Name = spTextbox::create(Settings::getWidth() - 500);
    m_Name->setTooltipText(tr("Name of the campaign shown in the map selection screen."));
    m_Name->setPosition(300, y);
    m_Name->setCurrentText("");
    pSpriteBox->addChild(m_Name);

    y += 40;
    pText =  oxygine::spTextField::create();
    pText->setStyle(style);
    pText->setHtmlText(tr("Author:"));
    pText->setPosition(30, y);
    pSpriteBox->addChild(pText);
    m_Author = spTextbox::create(Settings::getWidth() - 500);
    m_Author->setTooltipText(tr("Name of the author shown in the map selection screen."));
    m_Author->setPosition(300, y);
    m_Author->setCurrentText(Settings::getUsername());
    pSpriteBox->addChild(m_Author);

    y += 40;
    pText =  oxygine::spTextField::create();
    pText->setStyle(style);
    pText->setHtmlText(tr("Description:"));
    pText->setPosition(30, y);
    pSpriteBox->addChild(pText);
    m_Description = spTextbox::create(Settings::getWidth() - 500);
    m_Description->setTooltipText(tr("Description of the campaign shown in the map selection screen."));
    m_Description->setPosition(300, y);
    m_Description->setCurrentText("");
    pSpriteBox->addChild(m_Description);

    y += 40;
    QSize size(Settings::getWidth() - 80, Settings::getHeight() - 280);
    m_Panel = spPanel::create(true, size, size);
    m_Panel->setPosition(40, y);
    pSpriteBox->addChild(m_Panel);

    // add campaign
    oxygine::spButton pAddCampaignButton = pObjectManager->createButton(tr("Add Map"), 200);
    pAddCampaignButton->setPosition(30, Settings::getHeight() - 30 - pAddCampaignButton->getHeight());
    pSpriteBox->addChild(pAddCampaignButton);
    pAddCampaignButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigShowAddCampaign();
    });

    // load campaign
    oxygine::spButton pLoadCampaignButton = pObjectManager->createButton(tr("Load"), 150);
    pLoadCampaignButton->setPosition(Settings::getWidth() / 2 - 10 - pLoadCampaignButton->getWidth(), Settings::getHeight() - 30 - pLoadCampaignButton->getHeight());
    pSpriteBox->addChild(pLoadCampaignButton);
    pLoadCampaignButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigShowLoadCampaign();
    });

    // save campaign
    oxygine::spButton pSaveCampaignButton = pObjectManager->createButton(tr("Save"), 150);
    pSaveCampaignButton->setPosition(Settings::getWidth() / 2 + 10, Settings::getHeight() - 30 - pSaveCampaignButton->getHeight());
    pSpriteBox->addChild(pSaveCampaignButton);
    pSaveCampaignButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigShowSaveCampaign();
    });

    // ok button
    oxygine::spButton pOkButton = pObjectManager->createButton(tr("Ok"), 150);
    pOkButton->setPosition(Settings::getWidth() - pOkButton->getWidth() - 30, Settings::getHeight() - 30 - pOkButton->getHeight());
    pSpriteBox->addChild(pOkButton);
    pOkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigShowExitBox();
    });

    connect(this, &CampaignEditor::sigShowAddCampaign, this, &CampaignEditor::showAddCampaign, Qt::QueuedConnection);
    connect(this, &CampaignEditor::sigShowSelectFolder, this, &CampaignEditor::showSelectFolder, Qt::QueuedConnection);
    connect(this, &CampaignEditor::sigShowSaveCampaign, this, &CampaignEditor::showSaveCampaign, Qt::QueuedConnection);
    connect(this, &CampaignEditor::sigShowLoadCampaign, this, &CampaignEditor::showLoadCampaign, Qt::QueuedConnection);
    connect(this, &CampaignEditor::sigUpdateCampaignData, this, &CampaignEditor::updateCampaignData, Qt::QueuedConnection);
    connect(this, &CampaignEditor::sigShowEditEnableMaps, this, &CampaignEditor::showEditEnableMaps, Qt::QueuedConnection);
    connect(this, &CampaignEditor::sigShowEditDisableMaps, this, &CampaignEditor::showEditDisableMaps, Qt::QueuedConnection);
    connect(this, &CampaignEditor::sigShowExitBox, this, &CampaignEditor::showExitBox, Qt::QueuedConnection);
    connect(this, &CampaignEditor::sigShowEditScriptVariables, this, &CampaignEditor::showEditScriptVariables, Qt::QueuedConnection);
}

void CampaignEditor::showExitBox()
{
    
    spDialogMessageBox pExit = spDialogMessageBox::create(tr("Do you want to exit the campaign editor?"), true);
    connect(pExit.get(), &DialogMessageBox::sigOk, this, &CampaignEditor::exitEditor, Qt::QueuedConnection);
    addChild(pExit);
    
}

void CampaignEditor::exitEditor()
{
    
    emit sigFinished();
    detach();
    
}

void CampaignEditor::showAddCampaign()
{
    
    QVector<QString> wildcards;
    wildcards.append("*.map");
    QString path = QCoreApplication::applicationDirPath() + "/" + m_CampaignFolder->getCurrentText();
    spFileDialog fileDialog = spFileDialog::create(path, wildcards, "");
    this->addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &CampaignEditor::addCampaign, Qt::QueuedConnection);
    
}

void CampaignEditor::showSaveCampaign()
{
    
    QVector<QString> wildcards;
    wildcards.append("*.jsm");
    QString path = "maps/";
    spFileDialog fileDialog = spFileDialog::create(path, wildcards, m_Name->getCurrentText());
    this->addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &CampaignEditor::saveCampaign, Qt::QueuedConnection);
    
}

void CampaignEditor::showLoadCampaign()
{
    
    QVector<QString> wildcards;
    wildcards.append("*.jsm");
    QString path = "maps/";
    spFileDialog fileDialog = spFileDialog::create(path, wildcards, "");
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
        data.mapName = getMapName(filename);
        mapDatas.append(data);
        updateCampaignData();
    }
}

QString CampaignEditor::getMapName(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QDataStream stream(&file);
    QString ret = GameMap::readMapName(stream);
    file.close();
    return ret;
}

void CampaignEditor::showSelectFolder()
{
    
    QString path = QCoreApplication::applicationDirPath() + "/maps";
    spFolderDialog folderDialog = spFolderDialog::create(path);
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
    
    mapDatas.clear();
    m_Panel->clearContent();
    
}

void CampaignEditor::updateCampaignData()
{
    
    m_Panel->clearContent();
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    for (qint32 i = 0; i < mapDatas.size(); i++)
    {
        oxygine::TextStyle style = FontManager::getMainFont24();
        style.color = FontManager::getFontColor();
        style.vAlign = oxygine::TextStyle::VALIGN_TOP;
        style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        style.multiline = false;
        oxygine::spTextField pText =  oxygine::spTextField::create();
        pText->setStyle(style);
        pText->setHtmlText(mapDatas[i].mapName);
        pText->setPosition(10, 10 + i * 40);
        m_Panel->addItem(pText);

        oxygine::spButton pEnableButton = pObjectManager->createButton(tr("Enable Maps"), 150);
        pEnableButton->setPosition(200, 10 + i * 40);
        m_Panel->addItem(pEnableButton);
        pEnableButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            emit sigShowEditEnableMaps(i);
        });

        oxygine::spButton pDisableButton = pObjectManager->createButton(tr("Disable Maps"), 150);
        pDisableButton->setPosition(360, 10 + i * 40);
        m_Panel->addItem(pDisableButton);
        pDisableButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            emit sigShowEditDisableMaps(i);
        });

        oxygine::spButton pVariableButton = pObjectManager->createButton(tr("Variables"), 150);
        pVariableButton->setPosition(520, 10 + i * 40);
        m_Panel->addItem(pVariableButton);
        pVariableButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            emit sigShowEditScriptVariables(i);
        });

        oxygine::spButton pRemoveButton = pObjectManager->createButton(tr("Remove Map"), 150);
        pRemoveButton->setPosition(680, 10 + i * 40);
        m_Panel->addItem(pRemoveButton);
        pRemoveButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            mapDatas.removeAt(i);
            emit sigUpdateCampaignData();
        });

        pText =  oxygine::spTextField::create();
        pText->setStyle(style);
        pText->setHtmlText(tr("Last Map"));
        pText->setPosition(835, 10 + i * 40);
        m_Panel->addItem(pText);

        spCheckbox pBox = spCheckbox::create();
        pBox->setChecked(mapDatas[i].lastMap);
        pBox->setTooltipText(tr("All maps marked as last map need to be won in order to finish the campaign"));
        pBox->setPosition(940, 10 + i * 40);
        m_Panel->addItem(pBox);
        connect(pBox.get(), &Checkbox::checkChanged, [=](bool value)
        {
            mapDatas[i].lastMap = value;
        });
    }
    m_Panel->setContentHeigth(mapDatas.size() * 40 + 40);
    m_Panel->setContentWidth(1010);
    
}

void CampaignEditor::loadCampaign(QString filename)
{
    if (filename.endsWith(".jsm"))
    {
        QFile file(filename);
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        bool started = false;
        while (!stream.atEnd())
        {
            QString line = stream.readLine().simplified();
            if (line.endsWith(campaign))
            {
                if (started == true)
                {
                    started = false;
                    break;
                }
                else
                {
                    started = true;
                }
            }
            else if (started)
            {
                if (line.endsWith(campaignName))
                {
                    line = stream.readLine().simplified();
                    m_Name->setCurrentText(line.replace("return qsTr(\"", "").replace("\");", ""));
                    line = stream.readLine();
                }
                else if (line.endsWith(campaignAuthor))
                {
                    line = stream.readLine().simplified();
                    m_Author->setCurrentText(line.replace("return qsTr(\"", "").replace("\");", ""));
                    line = stream.readLine();
                }
                else if (line.endsWith(campaignDescription))
                {
                    line = stream.readLine().simplified();
                    m_Description->setCurrentText(line.replace("return qsTr(\"", "").replace("\");", ""));
                    line = stream.readLine();
                }
                else if (line.endsWith(campaignMaps))
                {
                    loadCampaignMaps(stream);
                }
                else if (line.endsWith(campaignFinished))
                {
                    while (!stream.atEnd())
                    {
                        line = stream.readLine().simplified();
                        if (line.endsWith(campaignFinished))
                        {
                            break;
                        }
                        else
                        {
                            if (line.endsWith(campaignMapNames))
                            {
                                QStringList items = line.replace("var map", "")
                                                        .replace("Won = variables.createVariable(\"", ",")
                                                        .replace("\"); // " + campaignMapNames, "")
                                                        .split(",");
                                if (items.size() >= 2)
                                {
                                    mapDatas[items[0].toInt()].lastMap = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    updateMapNames();
    updateCampaignData();
}

void CampaignEditor::loadCampaignMaps(QTextStream& stream)
{
    mapDatas.clear();
    while (!stream.atEnd())
    {
        QString line = stream.readLine().simplified();
        if (line.endsWith(campaignMaps))
        {

            break;
        }
        if (line.endsWith(campaignMapsFolder))
        {
            m_CampaignFolder->setCurrentText(line.replace("var ret = [\"", "").replace("\"]; // " + campaignMapsFolder, ""));
        }
        if (line.endsWith(campaignMapNames))
        {
            mapDatas.append(MapData());
            QStringList items = line.replace("var map", "")
                                    .replace("Won = variables.createVariable(\"", ",")
                                    .replace("\"); // " + campaignMapNames, "")
                                    .split(",");
            if (items.size() >= 2)
            {
                mapDatas[items[0].toInt()].mapName = items[1];
            }
        }
        if (line.endsWith(campaignMapEnabled))
        {
            qint32 mapDataIndex = line.replace("var map", "").replace("EnableCount = 0; // " + campaignMapEnabled, "").toInt();
            while (!stream.atEnd())
            {
                line = stream.readLine().simplified();
                if (line.endsWith(campaignMapEnabled))
                {
                    break;
                }
                else
                {
                    QString index = line.replace("if (map", "");
                    qint32 pos = index.indexOf("Won");
                    index = index.remove(pos, index.size());
                    mapDatas[mapDataIndex].previousMaps.append(mapDatas[index.toInt()].mapName);
                }
            }
        }
        if (line.endsWith(campaignMapDisabled))
        {
            qint32 mapDataIndex = line.replace("var map", "").replace("DisableCount = 0; // " + campaignMapDisabled, "").toInt();
            while (!stream.atEnd())
            {
                line = stream.readLine().simplified();
                if (line.endsWith(campaignMapDisabled))
                {
                    break;
                }
                else
                {
                    QString index = line.replace("if (map", "");
                    qint32 pos = index.indexOf("Won");
                    index = index.remove(pos, index.size());
                    mapDatas[mapDataIndex].disableMaps.append(mapDatas[index.toInt()].mapName);
                }
            }
        }
        if (line.endsWith(campaignMapAdd))
        {
            QStringList items = line.replace("if (map", "")
                                    .replace("DisableCount < ", ",")
                                    .replace(" && map", ",")
                                    .replace(" && ", ",")
                                    .replace("EnableCount >= ", ",")
                                    .replace(") {ret.push(\"", ",")
                                    .replace("\");} // " + campaignMapAdd, "")
                                    .split(",");
            if (items.size() >= 5)
            {
                qint32 index = items[0].toInt();
                mapDatas[index].disableCount = items[1].toInt();
                mapDatas[index].previousCount = items[3].toInt();
                for (qint32 i = 4; i < items.size() - 1; i++)
                {
                    if (items[i].startsWith("!("))
                    {
                        QStringList subList = items[i].replace("!(variables.createVariable(\"", "")
                                              .replace("\").readDataInt32() ", "@")
                                              .replace(" ", "@").replace(")", "").split("@");
                        if (subList.size() >= 3)
                        {
                            subList[1] = subList[1].replace(">", "&gt;").replace("<", "&lt;");
                            mapDatas[index].scriptVariableDisableActive = true;
                            mapDatas[index].scriptVariableDisableName = subList[0];
                            mapDatas[index].scriptVariableDisableCompare = subList[1];
                            mapDatas[index].scriptVariableDisableValue = subList[2].toInt();
                        }

                    }
                    else
                    {
                        QStringList subList = items[i].replace("variables.createVariable(\"", "")
                                              .replace("\").readDataInt32() ", "@")
                                              .replace(" ", "@").replace(")", "").split("@");
                        if (subList.size() >= 3)
                        {
                            subList[1] = subList[1].replace(">", "&gt;").replace("<", "&lt;");
                            mapDatas[index].scriptVariableEnableActive = true;
                            mapDatas[index].scriptVariableEnableName = subList[0];
                            mapDatas[index].scriptVariableEnableCompare = subList[1];
                            mapDatas[index].scriptVariableEnableValue = subList[2].toInt();
                        }
                    }
                }
                mapDatas[index].map = items[items.size() - 1];
            }
        }
    }   
}

void CampaignEditor::updateMapNames()
{
    QStringList orgName;
    QStringList newName;
    for (qint32 i = 0; i < mapDatas.size(); i++)
    {
        QString name = getMapName(m_CampaignFolder->getCurrentText() + mapDatas[i].map);
        if (name != mapDatas[i].mapName && !name.isEmpty())
        {
            orgName.append(mapDatas[i].mapName);
            newName.append(name);
            mapDatas[i].mapName = name;
        }
    }
    for (qint32 i = 0; i < mapDatas.size(); i++)
    {
        for (qint32 i2 = 0; i2 < orgName.size(); i2++)
        {
            if (mapDatas[i].previousMaps.contains(orgName[i2]))
            {
                mapDatas[i].previousMaps.removeAll(orgName[i2]);
                mapDatas[i].previousMaps.append(newName[i2]);
            }
            if (mapDatas[i].disableMaps.contains(orgName[i2]))
            {
                mapDatas[i].disableMaps.removeAll(orgName[i2]);
                mapDatas[i].disableMaps.append(newName[i2]);
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
        stream << "        return qsTr(\"" << m_Name->getCurrentText() << "\");\n";
        stream << "    }; //" << campaignName << "\n";

        stream << "    this.getAuthor = function() { // " << campaignAuthor << "\n";
        stream << "        return qsTr(\"" << m_Author->getCurrentText() << "\");\n";
        stream << "    }; // " << campaignAuthor << "\n";

        stream << "    this.getDescription = function() { // " << campaignDescription << "\n";
        stream << "        return qsTr(\"" << m_Description->getCurrentText() << "\");\n";
        stream << "    }; // " << campaignDescription << "\n";

        stream << "    this.getCurrentCampaignMaps = function(campaign) { // " << campaignMaps << "\n";
        stream << "        var variables = campaign.getVariables();" << "\n";
        QString folder = m_CampaignFolder->getCurrentText();
        if (!folder.endsWith("/"))
        {
            folder += "/";
        }
        stream << "        var ret = [\"" << folder << "\"]; // " << campaignMapsFolder << "\n";
        for (qint32 i = 0; i < mapDatas.size(); i++)
        {
            stream << "        var map" << QString::number(i) << "Won = variables.createVariable(\"" << mapDatas[i].mapName << "\"); // " << campaignMapNames << "\n";
        }
        for (qint32 i = 0; i < mapDatas.size(); i++)
        {
            stream << "        var map" << QString::number(i) << "EnableCount = 0; // " << campaignMapEnabled << "\n";
            for (qint32 i2 = 0; i2 < mapDatas[i].previousMaps.size(); i2++)
            {
                for (qint32 i3 = 0; i3 < mapDatas.size(); i3++)
                {
                    if (mapDatas[i3].mapName == mapDatas[i].previousMaps[i2])
                    {
                        stream << "        if (map" << QString::number(i3) << "Won.readDataBool() === true) { map" << QString::number(i) << "EnableCount++;} \n";
                        break;
                    }
                }
            }
            stream << "        // " << campaignMapEnabled << "\n";

            stream << "        var map" << QString::number(i) << "DisableCount = 0; // " << campaignMapDisabled << "\n";
            for (qint32 i2 = 0; i2 < mapDatas[i].disableMaps.size(); i2++)
            {
                for (qint32 i3 = 0; i3 < mapDatas.size(); i3++)
                {
                    if (mapDatas[i3].mapName == mapDatas[i].disableMaps[i2])
                    {
                        stream << "        if (map" << QString::number(i3) << "Won.readDataBool() === true) { map" << QString::number(i) << "DisableCount++;} \n";
                    }
                }
            }
            stream << "        // " << campaignMapDisabled << "\n";
            stream << "        if (map" << QString::number(i) << "DisableCount < " << mapDatas[i].disableCount <<
                      " && map" << QString::number(i) << "EnableCount >= " << mapDatas[i].previousCount;
            if (mapDatas[i].scriptVariableEnableActive)
            {
                QString compare = mapDatas[i].scriptVariableEnableCompare;
                compare = compare.replace("&gt;", ">").replace("&lt;", "<");
                stream << " && variables.createVariable(\"" << mapDatas[i].scriptVariableEnableName << "\").readDataInt32() "
                       << compare << " " << mapDatas[i].scriptVariableEnableValue;
            }
            if (mapDatas[i].scriptVariableDisableActive)
            {
                QString compare = mapDatas[i].scriptVariableDisableCompare;
                compare = compare.replace("&gt;", ">").replace("&lt;", "<");
                stream << " && !(variables.createVariable(\"" << mapDatas[i].scriptVariableDisableName << "\").readDataInt32() "
                       << compare << " " << mapDatas[i].scriptVariableDisableValue << ")";
            }

            stream << ") {ret.push(\"" << mapDatas[i].map << "\");} // " << campaignMapAdd << "\n";
        }

        stream << "        return ret;\n";
        stream << "    }; // " << campaignMaps << "\n";

        stream << "    this.mapFinished = function(campaign, map, result) { // " << campaignMapFinished << "\n";
        stream << "        var variables = campaign.getVariables();\n";
        stream << "        var mapVar = variables.createVariable(map.getMapName());\n";
        stream << "        mapVar.writeDataBool(result);\n";
        stream << "    }; // " << campaignMapFinished << "\n";

        stream << "    this.getCampaignFinished = function(campaign){ // " << campaignFinished << "\n";
        stream << "        var variables = campaign.getVariables();\n";
        stream << "        var wonCounter = 0;\n";
        qint32 count = 0;
        for (qint32 i = 0; i < mapDatas.size(); i++)
        {
            if (mapDatas[i].lastMap)
            {
                count++;
                stream << "        var map" << QString::number(i) << "Won = variables.createVariable(\"" << mapDatas[i].mapName << "\"); // " << campaignMapNames << "\n";
                stream << "        if (map" << QString::number(i) << "Won.readDataBool() === true) { wonCounter++;} \n";
            }
        }
        if (count > 0)
        {
            stream << "        if (wonCounter >= " << QString::number(count) << "){return true;} \n";
        }
        stream << "        return false;\n";
        stream << "    } // " << campaignFinished << "\n";
        stream << "// " << campaign << "\n"  << "};\n" <<
                  "Constructor.prototype = BASECAMPAIGN;\n" <<
                  "var campaignScript = new Constructor();";
    }
}

void CampaignEditor::showEditEnableMaps(qint32 index)
{
    
    spGenericBox pBox = spGenericBox::create();
    QSize size(Settings::getWidth() - 40, Settings::getHeight() - 100);
    spPanel pPanel = spPanel::create(true, size, size);
    pPanel->setPosition(20, 20);
    pBox->addChild(pPanel);
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    oxygine::spTextField pText =  oxygine::spTextField::create();
    pText->setStyle(style);
    pText->setHtmlText(tr("Enable Map Count:"));
    pText->setPosition(10, 10);
    pPanel->addItem(pText);
    spSpinBox spinBox = spSpinBox::create(150, 0, mapDatas.size() - 1);
    spinBox->setTooltipText(tr("Number of maps that leads to this map and that need to be won in order to play this map. Can be smaller so multiple campaign paths lead to this map."));
    spinBox->setPosition(300, 10);
    spinBox->setCurrentValue(mapDatas[index].previousCount);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        mapDatas[index].previousCount = static_cast<qint32>(value);
    });
    pPanel->addItem(spinBox);

    qint32 counter = 0;
    for (qint32 i = 0; i < mapDatas.size(); i++)
    {
        if (i != index)
        {
            pText =  oxygine::spTextField::create();
            pText->setStyle(style);
            pText->setHtmlText(mapDatas[i].mapName);
            pText->setPosition(10, 50 + counter * 40);
            pPanel->addItem(pText);

            spCheckbox pCheckbox = spCheckbox::create();
            pCheckbox->setTooltipText(tr("If checked this map leads to the selected map. Also see \"Enable Map Count\""));
            pCheckbox->setPosition(300, 50 + counter * 40);
            if (mapDatas[index].previousMaps.contains(mapDatas[i].mapName))
            {
                pCheckbox->setChecked(true);
            }
            else
            {
                pCheckbox->setChecked(false);
            }
            connect(pCheckbox.get(), &Checkbox::checkChanged, [=](bool value)
            {
                if (value)
                {
                    mapDatas[index].previousMaps.append(mapDatas[i].mapName);
                }
                else
                {
                    mapDatas[index].previousMaps.removeAll(mapDatas[i].mapName);
                }
            });
            pPanel->addItem(pCheckbox);
            counter++;
        }
    }
    addChild(pBox);
    
}

void CampaignEditor::showEditDisableMaps(qint32 index)
{
    
    spGenericBox pBox = spGenericBox::create();
    QSize size(Settings::getWidth() - 40, Settings::getHeight() - 100);
    spPanel pPanel = spPanel::create(true, size, size);
    pPanel->setPosition(20, 20);
    pBox->addChild(pPanel);
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    oxygine::spTextField pText =  oxygine::spTextField::create();
    pText->setStyle(style);
    pText->setHtmlText(tr("Disable Map Count:"));
    pText->setPosition(10, 10);
    pPanel->addItem(pText);
    spSpinBox spinBox = spSpinBox::create(150, 1, mapDatas.size() - 1);
    spinBox->setTooltipText(tr("Number of maps that disable this map again. They need to be won in order to make this map unplayable again. E.g. you won the selected map and you want to stop make it repeatedly playable."));
    spinBox->setPosition(300, 10);
    spinBox->setCurrentValue(mapDatas[index].disableCount);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        mapDatas[index].disableCount = static_cast<qint32>(value);
    });
    pPanel->addItem(spinBox);

    qint32 counter = 0;
    for (qint32 i = 0; i < mapDatas.size(); i++)
    {
        pText =  oxygine::spTextField::create();
        pText->setStyle(style);
        pText->setHtmlText(mapDatas[i].mapName);
        pText->setPosition(10, 50 + counter * 40);
        pPanel->addItem(pText);

        spCheckbox pCheckbox = spCheckbox::create();
        pCheckbox->setTooltipText(tr("If checked this map disables the selected map. Also see \"Disable Map Count\""));
        pCheckbox->setPosition(300, 50 + counter * 40);
        if (mapDatas[index].disableMaps.contains(mapDatas[i].mapName))
        {
            pCheckbox->setChecked(true);
        }
        else
        {
            pCheckbox->setChecked(false);
        }
        connect(pCheckbox.get(), &Checkbox::checkChanged, [=](bool value)
        {
            if (value)
            {
                mapDatas[index].disableMaps.append(mapDatas[i].mapName);
            }
            else
            {
                mapDatas[index].disableMaps.removeAll(mapDatas[i].mapName);
            }
        });
        pPanel->addItem(pCheckbox);
        counter++;
    }
    CampaignEditor::addChild(pBox);
    
}

void CampaignEditor::showEditScriptVariables(qint32 index)
{
    
    spGenericBox pBox = spGenericBox::create();
    QSize size(Settings::getWidth() - 40, Settings::getHeight() - 100);
    spPanel pPanel = spPanel::create(true, size, size);
    pPanel->setPosition(20, 20);
    pBox->addChild(pPanel);
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    oxygine::TextStyle headerStyle = FontManager::getMainFont48();
    headerStyle.color = FontManager::getFontColor();
    headerStyle.vAlign = oxygine::TextStyle::VALIGN_TOP;
    headerStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headerStyle.multiline = false;

    qint32 y = 10;
    qint32 width = 300;
    spLabel pText = spLabel::create(width - 10);
    pText->setStyle(headerStyle);
    pText->setHtmlText(tr("Enable Variable"));
    pText->setPosition(10, y);
    pPanel->addItem(pText);
    y += 60;



    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Variable: "));
    pText->setPosition(30, y);
    pPanel->addItem(pText);
    spTextbox textBox = spTextbox::create(300);
    textBox->setTooltipText(tr("Name of the Variable that should be checked. Try not to use names starting with \"variable\". This name is used by the system."));
    textBox->setPosition(width, y);
    textBox->setCurrentText(mapDatas[index].scriptVariableEnableName);
    connect(textBox.get(), &Textbox::sigTextChanged,
            [=](QString value)
    {
        mapDatas[index].scriptVariableEnableName = value;
    });
    pPanel->addItem(textBox);
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Compare: "));
    pText->setPosition(30, y);
    pPanel->addItem(pText);

    QVector<QString> items = {"===", "!==", "&gt;=", "&lt;="};
    spDropDownmenu dropDown = spDropDownmenu::create(150, items);
    dropDown->setTooltipText(tr("The way how the variable gets compared with the constant. variable compare value "));
    dropDown->setPosition(width, y);
    dropDown->setCurrentItemText(mapDatas[index].scriptVariableEnableCompare);
    connect(dropDown.get(), &DropDownmenu::sigItemChanged, this, [=](qint32)
    {
        mapDatas[index].scriptVariableEnableCompare = dropDown->getCurrentItemText();
    });
    pPanel->addItem(dropDown);
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Value: "));
    pText->setPosition(30, y);
    pPanel->addItem(pText);
    spSpinBox spinBox = spSpinBox::create(150, -999999, 999999);
    spinBox->setTooltipText(tr("The value that the variable gets checked against."));
    spinBox->setPosition(width, y);
    spinBox->setCurrentValue(mapDatas[index].scriptVariableEnableValue);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        mapDatas[index].scriptVariableEnableValue = value;
    });
    pPanel->addItem(spinBox);
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Use Variable: "));
    pText->setPosition(30, y);
    pPanel->addItem(pText);
    spCheckbox checkBox = spCheckbox::create();
    checkBox->setTooltipText(tr("If checked the enable variable needs to fullfil the condition to allow this map to be playable."));
    checkBox->setPosition(width, y);
    checkBox->setChecked(mapDatas[index].scriptVariableEnableActive);
    connect(checkBox.get(), &Checkbox::checkChanged,
            [=](bool value)
    {
        mapDatas[index].scriptVariableEnableActive = value;
    });
    pPanel->addItem(checkBox);
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(headerStyle);
    pText->setHtmlText(tr("Disable Variable"));
    pText->setPosition(10, y);
    pPanel->addItem(pText);
    y += 60;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Variable: "));
    pText->setPosition(30, y);
    pPanel->addItem(pText);
    textBox = spTextbox::create(300);
    textBox->setTooltipText(tr("Name of the Variable that should be checked. Try not to use names starting with \"variable\". This name is used by the system."));
    textBox->setPosition(width, y);
    textBox->setCurrentText(mapDatas[index].scriptVariableDisableName);
    connect(textBox.get(), &Textbox::sigTextChanged,
            [=](QString value)
    {
        mapDatas[index].scriptVariableDisableName = value;
    });
    pPanel->addItem(textBox);
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Compare: "));
    pText->setPosition(30, y);
    pPanel->addItem(pText);
    dropDown = spDropDownmenu::create(150, items);
    dropDown->setTooltipText(tr("The way how the variable gets compared with the constant. variable compare value "));
    dropDown->setPosition(width, y);
    dropDown->setCurrentItemText(mapDatas[index].scriptVariableDisableCompare);
    connect(dropDown.get(), &DropDownmenu::sigItemChanged, this, [=](qint32)
    {
        mapDatas[index].scriptVariableDisableCompare = dropDown->getCurrentItemText();
    });
    pPanel->addItem(dropDown);
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Value: "));
    pText->setPosition(30, y);
    pPanel->addItem(pText);
    spinBox = spSpinBox::create(150, -999999, 999999);
    spinBox->setTooltipText(tr("The value that the variable gets checked against."));
    spinBox->setPosition(width, y);
    spinBox->setCurrentValue(mapDatas[index].scriptVariableDisableValue);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        mapDatas[index].scriptVariableDisableValue = value;
    });
    pPanel->addItem(spinBox);
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Use Variable: "));
    pText->setPosition(30, y);
    pPanel->addItem(pText);
    checkBox = spCheckbox::create();
    checkBox->setTooltipText(tr("If checked and if the disable variable fullfil the condition this map can't be played."));
    checkBox->setPosition(width, y);
    checkBox->setChecked(mapDatas[index].scriptVariableDisableActive);
    connect(checkBox.get(), &Checkbox::checkChanged,
            [=](bool value)
    {
        mapDatas[index].scriptVariableDisableActive = value;
    });
    pPanel->addItem(checkBox);
    y += 40;
    pPanel->setContentHeigth(y);
    CampaignEditor::addChild(pBox);
    
}
