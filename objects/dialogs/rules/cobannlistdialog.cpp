#include "cobannlistdialog.h"

#include "qdiriterator.h"

#include "coreengine/filesupport.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

#include "resource_management/cospritemanager.h"

#include "game/gamemap.h"

#include "objects/base/panel.h"
#include "objects/base/label.h"

#include "objects/dialogs/dialogtextinput.h"

COBannListDialog::COBannListDialog(QStringList cobannlist)
    : QObject(),
      m_CurrentCOBannList(cobannlist)
{
    setObjectName("COBannListDialog");
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
    m_OkButton->setPosition(Settings::getWidth() - m_OkButton->getWidth() - 30, Settings::getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit editFinished(m_CurrentCOBannList);
        detach();
    });

    // cancel button
    m_ExitButton = pObjectManager->createButton(tr("Cancel"), 150);
    m_ExitButton->setPosition(30, Settings::getHeight() - 30 - m_ExitButton->getHeight());
    pSpriteBox->addChild(m_ExitButton);
    m_ExitButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit canceled();
        detach();
    });

    m_ToggleAll = pObjectManager->createButton(tr("Un/Select All"), 180);
    m_ToggleAll->setPosition(Settings::getWidth() / 2 + 60 , Settings::getHeight() - 30 - m_ToggleAll->getHeight());
    pSpriteBox->addChild(m_ToggleAll);
    m_ToggleAll->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        m_toggle = !m_toggle;
        for (qint32 i = 0; i < m_Checkboxes.size(); i++)
        {
            m_Checkboxes[i]->setChecked(m_toggle);
            emit m_Checkboxes[i]->checkChanged(m_toggle);
        }
    });
    auto items = getNameList();
    m_PredefinedLists = spDropDownmenu::create(260, items);

    m_PredefinedLists->setPosition(Settings::getWidth() / 2 + 40 - m_PredefinedLists->getWidth(), Settings::getHeight() - 30 - m_ToggleAll->getHeight());
    pSpriteBox->addChild(m_PredefinedLists);
    connect(m_PredefinedLists.get(), &DropDownmenu::sigItemChanged, this, &COBannListDialog::setCOBannlist, Qt::QueuedConnection);

    oxygine::spButton pSave = pObjectManager->createButton(tr("Save"), 150);
    pSave->setPosition(Settings::getWidth() / 2 - pSave->getWidth() / 2, Settings::getHeight() - 75 - m_ToggleAll->getHeight());
    pSave->addClickListener([=](oxygine::Event*)
    {
        emit sigShowSaveBannlist();
    });
    pSpriteBox->addChild(pSave);
    connect(this, &COBannListDialog::sigShowSaveBannlist, this, &COBannListDialog::showSaveBannlist, Qt::QueuedConnection);

    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;


    // no the fun begins create checkboxes and stuff and a panel down here
    spPanel pPanel = spPanel::create(true, QSize(Settings::getWidth() - 60, Settings::getHeight() - 150),
                                     QSize(Settings::getWidth() - 60, Settings::getHeight() - 150));
    pPanel->setPosition(30, 30);
    pSpriteBox->addChild(pPanel);

    oxygine::TextStyle headerStyle = FontManager::getMainFont48();
    headerStyle.color = FontManager::getFontColor();
    headerStyle.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    headerStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headerStyle.multiline = false;
    spLabel pLabel = spLabel::create(pPanel->getWidth() - 40);
    pLabel->setStyle(headerStyle);
    pLabel->setHtmlText(tr("CO Bann List"));
    pLabel->setPosition(pPanel->getWidth() / 2 - pLabel->getTextRect().getWidth() / 2, 10);
    if (pLabel->getX() < 20)
    {
        pLabel->setX(20);
    }
    pPanel->addItem(pLabel);

    // load default army and co sets
    Interpreter* pInterpreter = Interpreter::getInstance();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    QString function1 = "getArmies";
    QJSValueList args1;
    QJSValue ret = pInterpreter->doFunction("PLAYER", function1, args1);
    QStringList m_Armies = ret.toVariant().toStringList();
    QStringList coids;
    // go through armies
    for (qint32 i = 0; i < m_Armies.size(); i++)
    {
        // add default co order
        ret = pInterpreter->doFunction("PLAYER", "getArmyCOs" + m_Armies[i]);
        coids.append(ret.toVariant().toStringList());
        // add unadded co's of this army
        for (qint32 i2 = 0; i2 < pCOSpriteManager->getCount(); i2++)
        {
            QString coID = pCOSpriteManager->getID(i2);
            QString function1 = "getCOArmy";
            QJSValue ret = pInterpreter->doFunction(coID, function1);
            if (ret.isString())
            {
                QString COArmy = ret.toString();
                if (COArmy == m_Armies[i] && !coids.contains(coID))
                {
                    coids.append(coID);
                    break;
                }
            }
        }
    }
    // add unadded co's
    for (qint32 i = 0; i < pCOSpriteManager->getCount(); i++)
    {
        QString coID = pCOSpriteManager->getID(i);
        if (!coids.contains(coID))
        {
            coids.append(coID);
        }
    }
    // remove multiple appearance of random co from selection and add him last
    coids.removeAll("CO_RANDOM");
    coids.append("CO_RANDOM");
    m_COIDs = coids;

    qint32 y = 30 + pLabel->getTextRect().getHeight() * 2;
    qint32 x = 10;
    for (qint32 i = 0; i < coids.size(); i++)
    {
        QString coID = coids[i];

        oxygine::ResAnim* pAnim = pCOSpriteManager->getResAnim((coID.toLower() + "+face"));
        oxygine::spSprite pCo = oxygine::spSprite::create();
        pCo->setResAnim(pAnim, 0, 0);

        pLabel = spLabel::create(250);
        pLabel->setStyle(style);

        pLabel->setHtmlText(pCOSpriteManager->getName(coID));

        pLabel->setPosition(x + 80, y);
        pCo->setPosition(x + 45, y);
        pCo->setScale(0.75f);
        spCheckbox pCheckbox = spCheckbox::create();
        pCheckbox->setPosition(x, y);
        m_Checkboxes.append(pCheckbox);

        if (m_CurrentCOBannList.contains(coID))
        {
            pCheckbox->setChecked(true);
        }
        else
        {
            pCheckbox->setChecked(false);
        }
        connect(pCheckbox.get(), &Checkbox::checkChanged, this, [=](bool checked)
        {
            if (checked)
            {
                m_CurrentCOBannList.append(coID);
            }
            else
            {
                m_CurrentCOBannList.removeAll(coID);
            }
        });

        pPanel->addItem(pCheckbox);
        pPanel->addItem(pLabel);
        pPanel->addItem(pCo);

        x += 340;
        if (x + 340 > pPanel->getContentWidth())
        {
            y += 40;
            x = 10;
        }
    }
    pPanel->setContentHeigth(y + 50);
}

void COBannListDialog::setCOBannlist(qint32 item)
{
    QStringList data;
    if (item == 0) // Commander Wars
    {
        data = QStringList({"CO_ADAM", "CO_ADDER", "CO_AIRA", "CO_ALEXANDER", "CO_ALEXIS",
                           "CO_AMY", "CO_ANDY", "CO_BRENNER", "CO_CASSIDY", "CO_CAULDER",
                           "CO_COLIN", "CO_CONRAD", "CO_DRAKE", "CO_EAGLE", "CO_EPOCH",
                           "CO_FLAK", "CO_FORSYTHE", "CO_GAGE", "CO_GRAVES", "CO_GREYFIELD",
                           "CO_GRIMM", "CO_GRIT", "CO_HACHI", "CO_HAWKE", "CO_IK_486_B7",
                           "CO_ISABELLA", "CO_JAKE", "CO_JAVIER", "CO_JESS", "CO_JOEY",
                           "CO_JUGGER", "CO_JULIA", "CO_KANBEI", "CO_KINDLE", "CO_KOAL",
                           "CO_LASH", "CO_LIN", "CO_MARY", "CO_MAX", "CO_MEIYO", "CO_MELANTHE",
                           "CO_MINA", "CO_MINAMOTO", "CO_NANA", "CO_NAPOLEON", "CO_NELL",
                           "CO_OLAF", "CO_OZZY", "CO_PENNY", "CO_PETER", "CO_RACHEL", "CO_RANDOM",
                           "CO_RATTIGAN", "CO_ROBOANDY", "CO_ROBOSTURM", "CO_SABAKI", "CO_SAMI",
                           "CO_SANJURO", "CO_SASHA", "CO_SENSEI", "CO_SMITAN", "CO_SONJA", "CO_SOPHIE",
                           "CO_STURM", "CO_TABITHA", "CO_TASHA", "CO_VARLOT", "CO_VON_BOLT",
                           "CO_WAYLON", "CO_WILL", "CO_XAVIER", "CO_ZANDRA"});
    }
    else if (item == 1) // advance wars dc
    {
        data = QStringList({"CO_BRENNER", "CO_CAULDER", "CO_FORSYTHE", "CO_GAGE", "CO_GREYFIELD",
                            "CO_ISABELLA", "CO_LIN", "CO_PENNY", "CO_RANDOM","CO_TABITHA", "CO_TASHA",
                            "CO_WAYLON", "CO_WILL"});
    }
    else if (item == 2) // advance wars ds
    {
        data = QStringList({"CO_RANDOM", "CO_ADDER", "CO_ANDY", "CO_COLIN", "CO_DRAKE", "CO_EAGLE",
                           "CO_FLAK", "CO_GRIMM", "CO_GRIT", "CO_HACHI", "CO_HAWKE",
                           "CO_JAKE", "CO_JAVIER", "CO_JESS", "CO_JUGGER", "CO_KANBEI", "CO_KINDLE",
                           "CO_KOAL", "CO_LASH", "CO_MAX", "CO_NELL", "CO_OLAF", "CO_RACHEL",
                           "CO_SAMI", "CO_SASHA", "CO_SENSEI", "CO_SONJA",  "CO_VON_BOLT", "CO_XAVIER"});
    }
    else if (item == 3) // advance wars 2
    {
        data = QStringList({"CO_RANDOM", "CO_ADDER", "CO_ANDY", "CO_COLIN", "CO_DRAKE", "CO_EAGLE",
                           "CO_FLAK", "CO_GRIT", "CO_HACHI", "CO_HAWKE",
                           "CO_JESS", "CO_KANBEI", "CO_LASH", "CO_MAX", "CO_NELL",
                           "CO_OLAF", "CO_SAMI", "CO_SENSEI", "CO_SONJA",
                           "CO_STURM"});
    }
    else if (item == 4)  // advance wars
    {
        data = QStringList({"CO_RANDOM", "CO_ANDY", "CO_DRAKE", "CO_EAGLE", "CO_GRIT",
                            "CO_KANBEI", "CO_MAX", "CO_OLAF",
                            "CO_SAMI", "CO_SONJA", "CO_STURM"});
    }
    else
    {
        QString file = m_PredefinedLists->getCurrentItemText();
        auto fileData = Filesupport::readList(file + ".bl", "data/cobannlist/");
        data = std::get<1>(fileData);
    }

    for (qint32 i = 0; i < m_COIDs.size(); i++)
    {
        if (data.contains(m_COIDs[i]))
        {
            m_Checkboxes[i]->setChecked(true);
        }
        else
        {
            m_Checkboxes[i]->setChecked(false);
        }
    }
    m_CurrentCOBannList = data;
}

void COBannListDialog::showSaveBannlist()
{
    
    spDialogTextInput pSaveInput = spDialogTextInput::create(tr("Banlist Name"), true, "");
    connect(pSaveInput.get(), &DialogTextInput::sigTextChanged, this, &COBannListDialog::saveBannlist, Qt::QueuedConnection);
    addChild(pSaveInput);
    
}

QVector<QString> COBannListDialog::getNameList()
{
    QVector<QString> items = {tr("Commander Wars"),
                              tr("Advance Wars DoR"),
                              tr("Advance Wars DS"),
                              tr("Advance Wars 2"),
                              tr("Advance Wars")};
    QStringList filters;
    filters << "*.bl";
    QDirIterator dirIter("data/cobannlist/", filters, QDir::Files, QDirIterator::IteratorFlag::NoIteratorFlags);
    while (dirIter.hasNext())
    {
        dirIter.next();
        QString file = dirIter.fileInfo().absoluteFilePath();
        std::tuple<QString, QStringList> data = Filesupport::readList(file);
        items.append(std::get<0>(data));
    }
    return items;
}

void COBannListDialog::saveBannlist(QString filename)
{    
    Filesupport::storeList(filename, m_CurrentCOBannList, "data/cobannlist/");
    auto items = getNameList();
    m_PredefinedLists->changeList(items);
}
