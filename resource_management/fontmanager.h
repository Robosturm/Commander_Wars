#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include "oxygine-framework.h"

#include <QObject>
#include <QColor>

class FontManager : public QObject, public oxygine::Resources
{
    Q_OBJECT
public:
    static FontManager* getInstance();



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
public slots:
    static void setFontColor(QColor color);
    static QColor getFontColor();
private:
    explicit FontManager();
    virtual ~FontManager() = default;
    static FontManager* m_pInstance;
    static QColor defaultColor;
};

#endif // FONTMANAGER_H
