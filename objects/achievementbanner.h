#ifndef ACHIEVEMENTBANNER_H
#define ACHIEVEMENTBANNER_H

#include <QObject>
#include <QTimer>

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

#include "coreengine/userdata.h"

class AchievementBanner;
using spAchievementBanner = oxygine::intrusive_ptr<AchievementBanner>;

class AchievementBanner : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit AchievementBanner(const Userdata::Achievement& achievement);
    virtual ~AchievementBanner() = default;
    void init();
protected slots:
    void removeBanner();
private:
    QTimer m_showTimer;
    static quint32 m_activeBanners;
};

#endif // ACHIEVEMENTBANNER_H
