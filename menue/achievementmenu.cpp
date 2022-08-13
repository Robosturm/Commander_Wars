#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "menue/achievementmenu.h"
#include "menue/mainwindow.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/audiothread.h"
#include "coreengine/userdata.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "game/gamemap.h"

#include "wiki/wikidatabase.h"

#include "objects/base/label.h"
#include "objects/base/progressinfobar.h"

Achievementmenu::Achievementmenu()
    : Basemenu()
{
    setObjectName("Achievementmenu");
    Interpreter::setCppOwnerShip(this);
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    moveToThread(pApp->getWorkerthread());
    CONSOLE_PRINT("Entering Achievement Menue", Console::eDEBUG);

    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = oxygine::spSprite::create();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("achievementmenu");
    if (pBackground != nullptr)
    {
        sprite->setResAnim(pBackground);
        // background should be last to draw
        sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Background));
        sprite->setScaleX(Settings::getWidth() / pBackground->getWidth());
        sprite->setScaleY(Settings::getHeight() / pBackground->getHeight());
    }
    pApp->getAudioThread()->clearPlayList();
    pApp->getAudioThread()->loadFolder("resources/music/credits_options");
    pApp->getAudioThread()->playRandom();


    oxygine::spButton pButtonExit = ObjectManager::createButton(tr("Exit"));
    addChild(pButtonExit);
    pButtonExit->setPosition(Settings::getWidth()  / 2.0f - pButtonExit->getWidth() / 2.0f,
                             Settings::getHeight() - pButtonExit->getHeight() - 10);
    pButtonExit->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigExitMenue();
    });
    connect(this, &Achievementmenu::sigExitMenue, this, &Achievementmenu::exitMenue, Qt::QueuedConnection);

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 y = 10;
    qint32 width = 150;
    spLabel pTextfield = spLabel::create(width - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Search: "));
    pTextfield->setPosition(10, y);
    addChild(pTextfield);
    m_SearchString = spTextbox::create(Settings::getWidth() - 380);
    m_SearchString->setTooltipText(tr("Search for an Achievement by title or description."));
    m_SearchString->setPosition(150, y);
    connect(m_SearchString.get(), &Textbox::sigTextChanged, this, [this](QString)
    {
        search();
    }, Qt::QueuedConnection);
    addChild(m_SearchString);
    oxygine::spButton pButton = ObjectManager::createButton(tr("Search"));
    addChild(pButton);
    pButton->setPosition(m_SearchString->getWidth() + m_SearchString->getX() + 10, y);
    pButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigSearch();
    });
    connect(this, &Achievementmenu::sigSearch, this, &Achievementmenu::search, Qt::QueuedConnection);
    y += 50;

    qint32 x = 10;
    pTextfield = spLabel::create(100);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Group: "));
    pTextfield->setPosition(x, y);
    addChild(pTextfield);
    x += 10 + pTextfield->getWidth();

    QStringList groups{tr("All")};
    Userdata* pUserdata = Userdata::getInstance();
    const auto* achievements = pUserdata->getAchievements();
    for (const auto & achievement : qAsConst(*achievements))
    {
        if (achievement.loaded && !groups.contains(achievement.group))
        {
            groups.append(achievement.group);
        }
    }
    m_group = spDropDownmenu::create(200, groups);
    m_group->setPosition(x, y);
    addChild(m_group);
    connect(m_group.get(), &DropDownmenu::sigItemChanged, this, [this](qint32)
    {
        search();
    }, Qt::QueuedConnection);
    x += 10 + m_group->getWidth();

    pTextfield = spLabel::create(100);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Sort: "));
    pTextfield->setPosition(x, y);
    addChild(pTextfield);
    x += 10 + pTextfield->getWidth();
    m_sort = spDropDownmenu::create(200, QStringList{tr("None"), tr("Ascending"), tr("Descending")});
    m_sort->setPosition(x, y);
    addChild(m_sort);
    connect(m_sort.get(), &DropDownmenu::sigItemChanged, this, [this](qint32)
    {
        search();
    }, Qt::QueuedConnection);
    x += 10 + m_sort->getWidth();
    y += 50;

    QSize size(Settings::getWidth() - 20, Settings::getHeight() - 210);
    m_MainPanel = spPanel::create(true, size, size);
    m_MainPanel->setPosition(10, 150);
    addChild(m_MainPanel);

    qint32 singleWidth = Settings::getWidth() - 80;
    qint32 achieveCount = 0;
    for (const auto & achievement : qAsConst(*achievements))
    {
        if (achievement.loaded)
        {
            if (achievement.progress >= achievement.targetValue)
            {
                achieveCount += 1;
            }
        }
    }
    pTextfield = spLabel::create(singleWidth);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Achievement Progress: ") + QString::number(achieveCount) + " / " + QString::number(achievements->length()));
    pTextfield->setPosition(10, 100);
    addChild(pTextfield);

    search();
    pApp->continueRendering();
}

void Achievementmenu::onEnter()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString object = "Init";
    QString func = "achievementMenu";
    if (pInterpreter->exists(object, func))
    {
        CONSOLE_PRINT("Executing:" + object + "." + func, Console::eDEBUG);
        QJSValueList args({pInterpreter->newQObject(this)});
        pInterpreter->doFunction(object, func, args);
    }
}

void Achievementmenu::exitMenue()
{    
    CONSOLE_PRINT("Leaving Achievement Menue", Console::eDEBUG);
    auto window = spMainwindow::create("ui/menu/playermenu.xml");
    oxygine::Stage::getStage()->addChild(window);
    oxygine::Actor::detach();    
}

void Achievementmenu::search()
{
    QString group;
    if (m_group->getCurrentItem()  > 0)
    {
        group = m_group->getCurrentItemText();
    }

    searchChanged(m_SearchString->getCurrentText(), group, static_cast<SortDirection>(m_sort->getCurrentItem()));
}

void Achievementmenu::searchChanged(QString searchText, QString group, SortDirection sortDirection)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    CONSOLE_PRINT("Achievementmenu::searchChanged " + searchText + " group " + group + " sorting direction " + QString::number(static_cast<qint32>(sortDirection)), Console::eDEBUG);
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    oxygine::TextStyle styleLarge = oxygine::TextStyle(FontManager::getMainFont48());
    styleLarge.color = FontManager::getFontColor();
    styleLarge.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    styleLarge.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    styleLarge.multiline = false;

    m_MainPanel->clearContent();
    searchText = searchText.toLower();

    Userdata* pUserdata = Userdata::getInstance();
    auto achievements = *pUserdata->getAchievements();
    // remove unrelated achievements
    if (!group.isEmpty())
    {
        qint32 i = 0;
        while (i < achievements.size())
        {
            if (achievements[i].group == group)
            {
                ++i;
            }
            else
            {
                achievements.removeAt(i);
            }
        }
    }
    // sort achievements
    switch (sortDirection)
    {
        case SortDirection::Ascending:
        {
            std::sort(achievements.begin(), achievements.end(), [](const Userdata::Achievement& lhs, const Userdata::Achievement& rhs)
            {
                return lhs.progress < rhs.progress;
            });
            break;
        }
        case SortDirection::Descending:
        {
            std::sort(achievements.begin(), achievements.end(), [](const Userdata::Achievement& lhs, const Userdata::Achievement& rhs)
            {
                return lhs.progress > rhs.progress;
            });
            break;
        }
        case SortDirection::None:
        {
            break;
        }
    }
    qint32 x = 10;
    qint32 y = 10;
    qint32 singleWidth = Settings::getWidth() - 80;
    for (const auto & achievement : qAsConst(achievements))
    {
        if (achievement.loaded)
        {
            bool achieved = achievement.progress >= achievement.targetValue;
            QString lowerName = achievement.name.toLower();
            QString lowerDescription = achievement.description.toLower();
            if (searchText.isEmpty() ||
                ((lowerName.contains(searchText) ||
                  lowerDescription.contains(searchText)) &&
                 (!achievement.hide || achieved)))
            {
                oxygine::spActor pParent = oxygine::spActor::create();
                if (achieved)
                {
                    WikiDatabase* pWikiDatabase = WikiDatabase::getInstance();
                    oxygine::spSprite pIcon = pWikiDatabase->getIcon(nullptr, achievement.icon, GameMap::defaultImageSize * 2);
                    pIcon->setPosition(x, y + 16);
                    pParent->addChild(pIcon);
                }
                else
                {
                    spLabel pTextfield = spLabel::create(50);
                    pTextfield->setStyle(styleLarge);
                    pTextfield->setHtmlText("?");
                    pTextfield->setPosition(x, y + 8);
                    pParent->addChild(pTextfield);
                }

                spLabel pTextfield = spLabel::create(singleWidth - 60);
                pTextfield->setStyle(style);
                if (achievement.hide && !achieved)
                {
                    pTextfield->setHtmlText("?");
                }
                else
                {
                    pTextfield->setHtmlText(achievement.name);
                }
                pTextfield->setPosition(x + 60, y);
                pParent->addChild(pTextfield);

                pTextfield = spLabel::create(singleWidth - 60);
                pTextfield->setStyle(style);
                if (achievement.hide && !achieved)
                {
                    pTextfield->setHtmlText("?");
                }
                else
                {
                    pTextfield->setHtmlText(achievement.description);
                }
                pTextfield->setPosition(x + 60, y + 40);
                pParent->addChild(pTextfield);

                QString info = QString::number(achievement.progress) + " / " + QString::number(achievement.targetValue);
                spProgressInfoBar pProgressInfoBar = spProgressInfoBar::create(singleWidth, 32, info, static_cast<float>(achievement.progress) / static_cast<float>(achievement.targetValue));
                pProgressInfoBar->setPosition(x, y + 80);
                pParent->addChild(pProgressInfoBar);
                pParent->setSize(Settings::getWidth(), 120);
                m_MainPanel->addItem(pParent);
                y += 120;
            }
        }
    }
    m_MainPanel->setContentHeigth(y + 50);
    pApp->continueRendering();
}
