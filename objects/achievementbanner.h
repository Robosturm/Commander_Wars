#ifndef ACHIEVEMENTBANNER_H
#define ACHIEVEMENTBANNER_H

#include <QObject>
#include <QTimer>
#include "coreengine/userdata.h"
#include "oxygine-framework.h"

class AchievementBanner : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit AchievementBanner(const Userdata::Achievement& achievement);

protected slots:
    void remove();
private:
    QTimer m_showTimer;
    static quint32 m_activeBanners;
};

#endif // ACHIEVEMENTBANNER_H
