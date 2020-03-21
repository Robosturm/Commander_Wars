#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include "oxygine-framework.h"

#include <QColor>

class FontManager : public oxygine::Resources
{
public:
    static FontManager* getInstance();

    static const QColor defaultColor;

    inline static oxygine::ResFont* getMainFont16()
    {
        return getInstance()->getResFont("main16");
    }

    inline static oxygine::ResFont* getMainFont24()
    {
        return getInstance()->getResFont("main24");
    }

    inline static oxygine::ResFont* getMainFont48()
    {
        return getInstance()->getResFont("main48");
    }

    inline static oxygine::ResFont* getMainFont72()
    {
        return getInstance()->getResFont("main72");
    }
private:
    explicit FontManager();
    virtual ~FontManager() = default;
    static FontManager* m_pInstance;
};

#endif // FONTMANAGER_H
