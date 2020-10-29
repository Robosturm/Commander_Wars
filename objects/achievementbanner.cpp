#include "achievementbanner.h"

#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"
#include "coreengine/settings.h"

quint32 AchievementBanner::m_activeBanners = 0;

AchievementBanner::AchievementBanner(const Userdata::Achievement& achievement)
    : QObject()
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    oxygine::spBox9Sprite pButtonBox = new oxygine::Box9Sprite();
    pButtonBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pButtonBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pButtonBox->setResAnim(pAnim);
    pButtonBox->setSize(300, 60);
    pButtonBox->setPosition(0, Settings::getHeight() + pButtonBox->getHeight());

    pButtonBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Achievement));

    addChild(pButtonBox);

    m_activeBanners++;
    m_showTimer.setSingleShot(true);
    m_showTimer.start(std::chrono::seconds(5));
    connect(&m_showTimer, &QTimer::timeout, this, &AchievementBanner::remove, Qt::QueuedConnection);
}

void AchievementBanner::remove()
{
    m_activeBanners--;
    detach();
}
