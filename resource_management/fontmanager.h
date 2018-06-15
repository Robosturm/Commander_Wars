#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include "oxygine-framework.h"

class FontManager : public oxygine::Resources
{
public:
    static FontManager* getInstance();

    inline static oxygine::ResFont* getMainFont()
    {
        return getInstance()->getResFont("main");
    }

    inline static oxygine::ResFont* getTimesFont10()
    {
        return getInstance()->getResFont("times_10");
    }
private:
    explicit FontManager();
    virtual ~FontManager() = default;
    static FontManager* m_pInstance;
};

#endif // FONTMANAGER_H
