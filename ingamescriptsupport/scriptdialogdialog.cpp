#include "qfileinfo.h"
#include "qimage.h"

#include "ingamescriptsupport/scriptdialogdialog.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/cospritemanager.h"

#include "coreengine/mainapp.h"

#include "objects/dialogs/filedialog.h"

#include "objects/base/dropdownmenusprite.h"

ScriptDialogDialog::ScriptDialogDialog(spScriptEventDialog scriptEventDialog)
    : QObject(),
      m_Event(scriptEventDialog)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    m_pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("semidialog");
    m_pSpriteBox->setResAnim(pAnim);
    m_pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    m_pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    m_pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(m_pSpriteBox);
    m_pSpriteBox->setPosition(0, 0);
    m_pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    QSize size(Settings::getWidth() - 80, Settings::getHeight() - 120);
    m_Panel = spPanel::create(true, size, size);
    m_Panel->setPosition(30, 30);
    m_pSpriteBox->addChild(m_Panel);

    // ok button
    oxygine::spButton pOkButton = pObjectManager->createButton(tr("Ok"), 150);
    pOkButton->setPosition(Settings::getWidth() - pOkButton->getWidth() - 30, Settings::getHeight() - 30 - pOkButton->getHeight());
    m_pSpriteBox->addChild(pOkButton);
    pOkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigFinished();
        detach();
    });

    // add Dialog button
    oxygine::spButton pDialogButton = pObjectManager->createButton(tr("add Dialog"), 150);
    pDialogButton->setPosition(30, Settings::getHeight() - 30 - pDialogButton->getHeight());
    m_pSpriteBox->addChild(pDialogButton);
    pDialogButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigAddItem();
    });
    // remove Dialog button
    oxygine::spButton pRemoveButton = pObjectManager->createButton(tr("remove Last"), 150);
    pRemoveButton->setPosition(Settings::getWidth() / 2 - pRemoveButton->getWidth() / 2, Settings::getHeight() - 30 - pRemoveButton->getHeight());
    m_pSpriteBox->addChild(pRemoveButton);
    pRemoveButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigRemoveLast();
    });
    connect(this, &ScriptDialogDialog::sigRemoveLast, this, &ScriptDialogDialog::removeLast, Qt::QueuedConnection);
    connect(this, &ScriptDialogDialog::sigAddItem, this, &ScriptDialogDialog::addItem, Qt::QueuedConnection);
    connect(this, &ScriptDialogDialog::sigShowChangeBackground, this, &ScriptDialogDialog::showChangeBackground, Qt::QueuedConnection);
    updateDialog();
}

void ScriptDialogDialog::addItem()
{
    m_Event->addDialog("", "CO_ANDY", GameEnums::COMood_Normal, Qt::red);
    qint32 panelWidth = getPanelWidth();
    addActorItem(m_Event->getDialogSize() - 1, panelWidth);
    m_Panel->setContentHeigth(m_Event->getDialogSize() * 40 + 80);
}

qint32 ScriptDialogDialog::getPanelWidth()
{
    qint32 panelWidth = Settings::getWidth();
    if (panelWidth < 1000)
    {
        panelWidth = 1000;
    }
    return panelWidth;
}

void ScriptDialogDialog::updateDialog()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    m_Panel->clearContent();
    qint32 count = m_Event->getDialogSize();
    qint32 panelWidth = getPanelWidth();
    for (qint32 i = 0; i < count; i++)
    {
        addActorItem(i, panelWidth);
    }
    m_Panel->setContentHeigth(count * 40 + 80);
    m_Panel->setContentWidth(panelWidth);
    pApp->continueRendering();
}

void ScriptDialogDialog::addActorItem(qint32 i, qint32 panelWidth)
{
    oxygine::spActor pActor = oxygine::spActor::create();
    spDialogEntry pDialog = m_Event->getDialog(i);
    qint32 y = i * 40;
    qint32 boxWidth = panelWidth - 800;
    qint32 posX = panelWidth - 800;
    spTextbox pTextbox = spTextbox::create(boxWidth);
    pTextbox->setTooltipText(tr("The text the CO should talk."));
    pTextbox->setPosition(0, y);
    pTextbox->setCurrentText(pDialog->text);
    pActor->addChild(pTextbox);
    connect(pTextbox.get(), &Textbox::sigTextChanged, [=](QString text)
    {
        pDialog->text = text;
    });
    QVector<QString> moods = {tr("Normal"), tr("Happy"), tr("Sad")};
    spDropDownmenu moodMenu = spDropDownmenu::create(150, moods);
    moodMenu->setTooltipText(tr("The CO Mood/Icon that will be used for the dialog."));
    moodMenu->setPosition(posX, y);
    moodMenu->setCurrentItem(static_cast<qint32>(pDialog->mood));
    pActor->addChild(moodMenu);
    connect(moodMenu.get(), &DropDownmenu::sigItemChanged, [=](qint32 item)
    {
        pDialog->mood = static_cast<GameEnums::COMood>(item);
    });

    QVector<QString> ids;
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    ids.append(ScriptEventDialog::m_CurrentPlayerCO0);
    ids.append(ScriptEventDialog::m_CurrentPlayerCO1);
    for (qint32 i = 0; i < pCOSpriteManager->getCount(); i++)
    {
        ids.append(pCOSpriteManager->getID(i));
    }
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function = "getAddtionalCoFaces";
    QJSValue ret = pInterpreter->doFunction("CO", function);
    auto additionalFaces = ret.toVariant().toStringList();
    QStringList coIds = ret.toVariant().toStringList();
    coIds.append(additionalFaces);
    for (const auto & face : qAsConst(coIds))
    {
        oxygine::ResAnim* pAnim = nullptr;
        pAnim = COSpriteManager::getInstance()->getResAnim(face + "+info");
        if (pAnim != nullptr)
        {
            ids.append(face);
        }
    }

    auto creator = [=](QString id)
    {
        oxygine::ResAnim* pAnim = nullptr;
        if (id == ScriptEventDialog::m_CurrentPlayerCO0)
        {
            pAnim = COSpriteManager::getInstance()->getResAnim("co_0+info");
        }
        else if (id == ScriptEventDialog::m_CurrentPlayerCO1)
        {
            pAnim = COSpriteManager::getInstance()->getResAnim("co_1+info");
        }
        else
        {
            pAnim = COSpriteManager::getInstance()->getResAnim(id + "+info");
        }
        oxygine::spSprite pSprite = oxygine::spSprite::create();
        pSprite->setResAnim(pAnim);
        pSprite->setScale(pAnim->getWidth() / 32.0f);
        pSprite->setSize(pAnim->getSize());
        return pSprite;
    };

    spDropDownmenuSprite coidsMenu = spDropDownmenuSprite::create(150, ids, creator);
    coidsMenu->setTooltipText(tr("The ID of the CO that should talk.\nNote: CO 1 and CO 2 represent the CO of the current Player."));
    coidsMenu->setPosition(posX + 150, y);
    coidsMenu->setCurrentItem(pDialog->coid);
    pActor->addChild(coidsMenu);
    connect(coidsMenu.get(), &DropDownmenuSprite::sigItemChanged, [=](qint32)
    {
        pDialog->coid = coidsMenu->getCurrentItemText();
    });

    function = "getDefaultPlayerColors";
    QJSValueList args;
    ret = pInterpreter->doFunction("PLAYER", function, args);
    qint32 colorCount = ret.toInt();
    QVector<QColor> playerColors;
    for (qint32 i = 0; i < colorCount; i++)
    {
        QString function = "getDefaultColor";
        QJSValueList args;
        args << i;
        ret = pInterpreter->doFunction("PLAYER", function, args);
        playerColors.append(QColor(ret.toString()));
    }
    spDropDownmenuColor colors = spDropDownmenuColor::create(110, playerColors);
    colors->setTooltipText(tr("The background color of the dialog."));
    colors->setPosition(posX + 300, y);
    colors->setCurrentItem(pDialog->color);
    pActor->addChild(colors);
    connect(colors.get(), &DropDownmenuColor::sigItemChanged, [=](QColor color)
    {
        pDialog->color = color;
    });

    oxygine::spButton pBackgroundTextbox = ObjectManager::createButton(tr("Background"), 200);
    pBackgroundTextbox->setPosition(posX + 420, y);
    pActor->addChild(pBackgroundTextbox);
    pBackgroundTextbox->addClickListener([=](oxygine::Event*)
    {
        dialogIndex = i;
        emit sigShowChangeBackground();
    });
    oxygine::spSprite pSprite = oxygine::spSprite::create();
    pSprite->setPosition(posX + 630, y + 5);
    pActor->addChild(pSprite);
    m_backgrounds.append(pSprite);
    m_backgroundAnims.append(oxygine::spResAnim());
    m_Panel->addItem(pActor);
    m_dialogItems.append(pActor);
    loadBackground(pDialog->background, i);
}

void ScriptDialogDialog::removeLast()
{
    if (m_Event->getDialogSize() > 0)
    {
        m_Event->removeDialog(m_Event->getDialogSize() - 1);
        m_dialogItems[m_dialogItems.size() - 1]->detach();
        m_dialogItems.removeLast();
        m_backgrounds.removeLast();
        m_backgroundAnims.removeLast();
        m_Panel->setContentHeigth(m_Event->getDialogSize() * 40 + 80);
    }
}

void ScriptDialogDialog::showChangeBackground()
{
    
    spDialogEntry pDialog = m_Event->getDialog(dialogIndex);
    QFileInfo file(pDialog->background);
    QString folder = "maps/";
    QString fileName = "";
    if (file.exists() && file.isFile())
    {
        folder = file.path();
        fileName = file.fileName();
    }
    spFileDialog pFileDialog = spFileDialog::create(folder, QVector<QString>(1, "*.png"), fileName, true);
    addChild(pFileDialog);
    connect(pFileDialog.get(), &FileDialog::sigFileSelected, this, &ScriptDialogDialog::setCurrentDialogBackground, Qt::QueuedConnection);
    
}

void ScriptDialogDialog::setCurrentDialogBackground(QString file)
{
    QFileInfo fileInfo(file);
    spDialogEntry pDialog = m_Event->getDialog(dialogIndex);
    if (fileInfo.exists() && fileInfo.isFile())
    {
        QString filename = file.replace(QCoreApplication::applicationDirPath() + "/", "");
        pDialog->background = filename;
        loadBackground(filename, dialogIndex);
    }
    else
    {
        pDialog->background = "";
        loadBackground("", dialogIndex);
    }
    dialogIndex = -1;
}

void ScriptDialogDialog::loadBackground(QString filename, qint32 index)
{
    if (!filename.isEmpty())
    {
        QImage image(filename);
        oxygine::spResAnim pAnim = oxygine::spSingleResAnim::create();
        m_backgroundAnims[index] = pAnim;
        Mainapp::getInstance()->loadResAnim(pAnim.get(), image);
        m_backgrounds[index]->setResAnim(pAnim.get());
        m_backgrounds[index]->setScaleX(60.0f / pAnim->getWidth());
        m_backgrounds[index]->setScaleY(30.0f / pAnim->getHeight());
    }
    else
    {
        m_backgrounds[index]->setResAnim(nullptr);
        m_backgroundAnims[index] = nullptr;
    }
}
