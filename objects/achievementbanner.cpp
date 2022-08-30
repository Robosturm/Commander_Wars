#include "objects/achievementbanner.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"

#include "coreengine/mainapp.h"
#include "coreengine/settings.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "wiki/wikidatabase.h"
#include "game/gamemap.h"

#include "objects/base/label.h"

quint32 AchievementBanner::m_activeBanners = 0;
constexpr quint32 bannerHeight = 45;

AchievementBanner::AchievementBanner(const Userdata::Achievement& achievement)
    : m_showTimer(this)
{
#ifdef GRAPHICSUPPORT
    setObjectName("AchievementBanner");
#endif
    Interpreter::setCppOwnerShip(this);
    qint32 bannerWidth = 350;
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    oxygine::spBox9Sprite pButtonBox = oxygine::spBox9Sprite::create();
    pButtonBox->setResAnim(pAnim);
    pButtonBox->setSize(bannerWidth, bannerHeight);
    setSize(bannerWidth, bannerHeight);
    addChild(pButtonBox);
    WikiDatabase* pWikiDatabase = WikiDatabase::getInstance();
    oxygine::spSprite pIcon = pWikiDatabase->getIcon(nullptr, achievement.icon, GameMap::defaultImageSize);
    pIcon->setPosition(10, pButtonBox->getHeight() / 2 - GameMap::defaultImageSize / 2);
    pButtonBox->addChild(pIcon);

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    spLabel pTextfield = spLabel::create(bannerWidth - 20 - GameMap::defaultImageSize * 2);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(achievement.name);
    pTextfield->setPosition(bannerWidth - 5 - pTextfield->getWidth(), 5);
    pButtonBox->addChild(pTextfield);
}

void AchievementBanner::init()
{
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Achievement));
    setPosition(0, Settings::getHeight() + bannerHeight);
    oxygine::spTween tween = oxygine::createTween(oxygine::Actor::TweenY(Settings::getHeight() - bannerHeight * (m_activeBanners + 1)), oxygine::timeMS(200 * (m_activeBanners + 1)));
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
