#ifndef BACKGROUNDMANAGER_H
#define BACKGROUNDMANAGER_H

#include "oxygine-framework.h"

class BackgroundManager : public oxygine::Resources
{
public:
    static BackgroundManager* getInstance();
private:
    explicit BackgroundManager();
    virtual ~BackgroundManager() = default;
    static BackgroundManager* m_pInstance;
};

#endif // BACKGROUNDMANAGER_H
