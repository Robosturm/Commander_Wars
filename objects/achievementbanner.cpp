#include "achievementbanner.h"

#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"
#include "coreengine/settings.h"
#include "wiki/wikidatabase.h"
#include "game/gamemap.h"
#include "objects/base/label.h"

quint32 AchievementBanner::m_activeBanners = 0;

AchievementBanner::AchievementBanner(const Userdata::Achievement& achievement)
    : QObject()
{
    setObjectName("AchievementBanner");
    qint32 bannerWidth = 350;
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    oxygine::spBox9Sprite pButtonBox = oxygine::spBox9Sprite::create();
    pButtonBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pButtonBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pButtonBox->setResAnim(pAnim);
    pButtonBox->setSize(bannerWidth, 60);
    setSize(bannerWidth, 60);
    addChild(pButtonBox);
    WikiDatabase* pWikiDatabase = WikiDatabase::getInstance();
    oxygine::spSprite pIcon = pWikiDatabase->getIcon(achievement.icon, GameMap::defaultImageSize * 2);
    pIcon->setPosition(5, 5);
    pButtonBox->addChild(pIcon);

    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    spLabel pTextfield = spLabel::create(bannerWidth - 20 - GameMap::defaultImageSize * 2);
    pTextfield->setStyle(style);
    pTextfield->setText(achievement.name);
    pTextfield->setPosition(bannerWidth - 5 - pTextfield->getWidth(), 5);
    pButtonBox->addChild(pTextfield);

    setPriority(static_cast<qint32>(Mainapp::ZOrder::Achievement));
    setPosition(0, Settings::getHeight() + 60);
    oxygine::spTween tween = oxygine::createTween(oxygine::Actor::TweenY(Settings::getHeight() - 60 * (m_activeBanners + 1)), oxygine::timeMS(200 * (m_activeBanners + 1)));
    addTween(tween);

    m_activeBanners++;
    m_showTimer.setSingleShot(true);
    m_showTimer.start(std::chrono::seconds(5));
    connect(&m_showTimer, &QTimer::timeout, this, &AchievementBanner::removeBanner, Qt::QueuedConnection);
}

void AchievementBanner::removeBanner()
{
    m_activeBanners--;
    detach();
}
