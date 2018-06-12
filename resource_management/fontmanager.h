#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include "oxygine-framework.h"

class FontManager : public oxygine::Resources
{
public:
    static FontManager* getInstance();

private:
    explicit FontManager();
    virtual ~FontManager() = default;
    static FontManager* m_pInstance;
};

#endif // FONTMANAGER_H
