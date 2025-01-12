#include <QFileInfo>
#include <QImage>

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/res/SingleResAnim.h"

#include "ingamescriptsupport/scriptdialogdialog.h"

#include "resource_management/objectmanager.h"
#include "resource_management/cospritemanager.h"

#include "coreengine/interpreter.h"
#include "coreengine/globalutils.h"
#include "coreengine/vfs.h"

#include "objects/dialogs/filedialog.h"
#include "objects/base/dropdownmenusprite.h"
#include "objects/base/dropdownmenucolor.h"


ScriptDialogDialog::ScriptDialogDialog(spScriptEventDialog scriptEventDialog)
    : m_Event(scriptEventDialog)
{
    Interpreter::setCppOwnerShip(this);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    m_pSpriteBox = MemoryManagement::create<oxygine::Box9Sprite>();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("semidialog");
    m_pSpriteBox->setResAnim(pAnim);
    m_pSpriteBox->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
    addChild(m_pSpriteBox);
    m_pSpriteBox->setPosition(0, 0);
    m_pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    QSize size(oxygine::Stage::getStage()->getWidth() - 80, oxygine::Stage::getStage()->getHeight() - 120);
    m_Panel = MemoryManagement::create<Panel>(true, size, size);
    m_Panel->setPosition(30, 30);
    m_pSpriteBox->addChild(m_Panel);

    // ok button
    oxygine::spButton pOkButton = pObjectManager->createButton(tr("Ok"), 150);
    pOkButton->setPosition(oxygine::Stage::getStage()->getWidth() - pOkButton->getScaledWidth() - 30,
                           oxygine::Stage::getStage()->getHeight() - 10 - pOkButton->getScaledHeight());
    m_pSpriteBox->addChild(pOkButton);
    pOkButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigFinished();
    });

    // add Dialog button
    oxygine::spButton pDialogButton = pObjectManager->createButton(tr("add Dialog"), 150);
    pDialogButton->setPosition(30, oxygine::Stage::getStage()->getHeight() - 10 - pDialogButton->getScaledHeight());
    m_pSpriteBox->addChild(pDialogButton);
    pDialogButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigAddItem();
    });
    // remove Dialog button
    oxygine::spButton pRemoveButton = pObjectManager->createButton(tr("remove Last"), 150);
    pRemoveButton->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - pRemoveButton->getScaledWidth() / 2,
                               oxygine::Stage::getStage()->getHeight() - 10 - pRemoveButton->getScaledHeight());
    m_pSpriteBox->addChild(pRemoveButton);
    pRemoveButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigRemoveLast();
    });
    connect(this, &ScriptDialogDialog::sigRemoveLast, this, &ScriptDialogDialog::removeLast, Qt::QueuedConnection);
    connect(this, &ScriptDialogDialog::sigAddItem, this, &ScriptDialogDialog::addItem, Qt::QueuedConnection);
    connect(this, &ScriptDialogDialog::sigShowChangeBackground, this, &ScriptDialogDialog::showChangeBackground, Qt::QueuedConnection);
    connect(this, &ScriptDialogDialog::sigFinished, this, &ScriptDialogDialog::remove, Qt::QueuedConnection);
    updateDialog();
}

void ScriptDialogDialog::remove()
{
    detach();
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
    qint32 panelWidth = oxygine::Stage::getStage()->getWidth();
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
    QVector<oxygine::spActor> items;
    spDialogEntry pDialog = m_Event->getDialog(i);
    qint32 y = i * 40;
    qint32 boxWidth = panelWidth - 800;
    qint32 posX = panelWidth - 800;
    spTextbox pTextbox = MemoryManagement::create<Textbox>(boxWidth);
    pTextbox->setTooltipText(tr("The text the CO should talk."));
    pTextbox->setPosition(0, y);
    pTextbox->setCurrentText(pDialog->text);
    items.append(pTextbox);
    DialogEntry* pPtrDialog = pDialog.get();
    connect(pTextbox.get(), &Textbox::sigTextChanged, this, [=](QString text)
    {
        pPtrDialog->text = text;
    });
    QStringList moods = {tr("Normal"), tr("Happy"), tr("Sad")};
    spDropDownmenu moodMenu = MemoryManagement::create<DropDownmenu>(150, moods);
    moodMenu->setTooltipText(tr("The CO Mood/Icon that will be used for the dialog."));
    moodMenu->setPosition(posX, y);
    moodMenu->setCurrentItem(static_cast<qint32>(pDialog->mood));
    items.append(moodMenu);
    connect(moodMenu.get(), &DropDownmenu::sigItemChanged, this, [=](qint32 item)
    {
        pPtrDialog->mood = static_cast<GameEnums::COMood>(item);
    });

    QStringList ids;
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    pCOSpriteManager->getCoGroups(ids);
    ids.push_front(ScriptEventDialog::m_CurrentPlayerCO0);
    ids.push_front(ScriptEventDialog::m_CurrentPlayerCO1);
    Interpreter* pInterpreter = Interpreter::getInstance();

    QString function = "getAddtionalCoFaces";
    QJSValue ret = pInterpreter->doFunction("CO", function);
    auto additionalFaces = ret.toVariant().toStringList();
    QStringList coIds = ret.toVariant().toStringList();
    coIds.append(additionalFaces);
    for (const auto & face : std::as_const(coIds))
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
        oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
        pSprite->setResAnim(pAnim);
        pSprite->setScale(static_cast<float>(pAnim->getWidth()) / 32.0f);
        pSprite->setSize(pAnim->getSize());
        return pSprite;
    };

    spDropDownmenuSprite coidsMenu = MemoryManagement::create<DropDownmenuSprite>(150, ids, creator);
    coidsMenu->setTooltipText(tr("The ID of the CO that should talk.\nNote: CO 1 and CO 2 represent the CO of the current Player."));
    coidsMenu->setPosition(posX + 150, y);
    coidsMenu->setCurrentItem(pDialog->coid);
    items.append(coidsMenu);
    connect(coidsMenu.get(), &DropDownmenuSprite::sigItemChanged, this, [=](qint32)
    {
        pPtrDialog->coid = coidsMenu->getCurrentItemText();
    });

    function = "getDefaultPlayerColors";
    QJSValueList args;
    ret = pInterpreter->doFunction("PLAYER", function, args);
    qint32 colorCount = ret.toInt();
    QVector<QColor> playerColors;
    for (qint32 i = 0; i < colorCount; i++)
    {
        QString function = "getDefaultColor";
        QJSValueList args({QJSValue(i)});
        ret = pInterpreter->doFunction("PLAYER", function, args);
        playerColors.append(QColor(ret.toString()));
    }
    spDropDownmenuColor colors = MemoryManagement::create<DropDownmenuColor>(110, playerColors);
    colors->setTooltipText(tr("The background color of the dialog."));
    colors->setPosition(posX + 300, y);
    colors->setCurrentItem(pDialog->color);
    items.append(colors);
    connect(colors.get(), &DropDownmenuColor::sigItemChanged, this, [this, pPtrDialog](QColor color)
    {
        pPtrDialog->color = color;
    });

    oxygine::spButton pBackgroundTextbox = ObjectManager::createButton(tr("Background"), 200);
    pBackgroundTextbox->setPosition(posX + 420, y);
    items.append(pBackgroundTextbox);
    pBackgroundTextbox->addClickListener([this, i](oxygine::Event*)
    {
        dialogIndex = i;
        emit sigShowChangeBackground();
    });
    oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
    pSprite->setPosition(posX + 630, y + 5);
    items.append(pSprite);
    m_backgrounds.append(pSprite);
    m_backgroundAnims.append(oxygine::spResAnim());
    for (auto & item : items)
    {
        m_Panel->addItem(item);
    }
    m_dialogItems.append(items);
    loadBackground(pDialog->background, i);
}

void ScriptDialogDialog::removeLast()
{
    if (m_Event->getDialogSize() > 0)
    {
        m_Event->removeDialog(m_Event->getDialogSize() - 1);
        auto & items = m_dialogItems[m_dialogItems.size() - 1];
        for (auto & item : items)
        {
            item->detach();
        }
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
    QString folder = Settings::userPath() + "maps/";
    QString fileName = "";
    if (file.exists() && file.isFile())
    {
        folder = file.path();
        fileName = file.fileName();
    }
    spFileDialog pFileDialog = MemoryManagement::create<FileDialog>(folder, QStringList(1, "*.png"), false, fileName, true, tr("Load"));
    addChild(pFileDialog);
    connect(pFileDialog.get(), &FileDialog::sigFileSelected, this, &ScriptDialogDialog::setCurrentDialogBackground, Qt::QueuedConnection);
    
}

void ScriptDialogDialog::setCurrentDialogBackground(QString file)
{
    QFileInfo fileInfo(file);
    spDialogEntry pDialog = m_Event->getDialog(dialogIndex);
    if (fileInfo.exists() && fileInfo.isFile())
    {
        QString filename = GlobalUtils::makePathRelative(file);
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
        QImage image;
        QString imgPath = Vfs::find(filename);
        if (QFile::exists(imgPath))
        {
            image = QImage(imgPath);
        }
        oxygine::spResAnim pAnim = MemoryManagement::create<oxygine::SingleResAnim>();
        m_backgroundAnims[index] = pAnim;
        Mainapp::getInstance()->loadResAnim(pAnim, image, 1, 1, 1);
        m_backgrounds[index]->setResAnim(pAnim.get());
        m_backgrounds[index]->setScaleX(60.0f / static_cast<float>(pAnim->getWidth()));
        m_backgrounds[index]->setScaleY(30.0f / static_cast<float>(pAnim->getHeight()));
    }
    else
    {
        m_backgrounds[index]->setResAnim(nullptr);
        m_backgroundAnims[index].reset();
    }
}
