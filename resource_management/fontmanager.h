#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <QColor>
#include "3rd_party/oxygine-framework/oxygine/TextStyle.h"

#include "resource_management/ressourcemanagement.h"

class FontManager : public QObject
{
    Q_OBJECT
public:

    static const char* const MAINFONT;
    static const char* const LOGOFONT;

    static FontManager* getInstance()
    {
        if (m_pInstance == nullptr)
        {
            m_pInstance = new FontManager();
            Interpreter::setCppOwnerShip(m_pInstance);
        }
        return m_pInstance;
    }
    /**
     * @brief getMainFont16
     * @return
     */
    inline static const oxygine::Font & getMainFont16()
    {
        return m_mainFont16;
    }
    /**
     * @brief getMainFont24
     * @return
     */
    inline static const oxygine::Font & getMainFont24()
    {
        return m_mainFont24;
    }
    /**
     * @brief getMainFont32
     * @return
     */
    inline static const oxygine::Font & getMainFont32()
    {
        return m_mainFont32;
    }
    /**
     * @brief getMainFont48
     * @return
     */
    inline static const oxygine::Font & getMainFont48()
    {
        return m_mainFont48;
    }
    /**
     * @brief getMainFont72
     * @return
     */
    inline static const oxygine::Font & getMainFont72()
    {
        return m_mainFont72;
    }
    /**
     * @brief getLogoFont
     * @return
     */
    inline static const oxygine::Font & getLogoFont()
    {
        return m_logoFont;
    }
    /**
     * @brief getFont
     * @param fontType
     * @param size
     * @return
     */
    static const oxygine::Font & getFont(QString fontType, qint32 size)
    {
        QString key = fontType + QString::number(size);
        if (m_fonts.contains(key))
        {
            return m_fonts[key];
        }
        else
        {
            static oxygine::Font dummy;
            return dummy;
        }
    }
    /**
     * @brief getFont
     * @param fontType
     * @return
     */
    static const oxygine::Font & getFont(QString fontType)
    {
        QString key = fontType;
        if (m_fonts.contains(key))
        {
            return m_fonts[key];
        }
        else
        {
            static oxygine::Font dummy;
            return dummy;
        }
    }
public slots:
    /**
     * @brief setFontColor
     * @param color
     */
    static void setFontColor(QColor color);
    /**
     * @brief getFontColor
     * @return
     */
    static QColor getFontColor();
protected:
    friend RessourceManagement<FontManager>;
    explicit FontManager();
private:

    virtual ~FontManager() = default;
    static QColor m_defaultColor;
    static FontManager* m_pInstance;
    static QMap<QString, oxygine::Font> m_fonts;

    static oxygine::Font m_mainFont16;
    static oxygine::Font m_mainFont24;
    static oxygine::Font m_mainFont32;
    static oxygine::Font m_mainFont48;
    static oxygine::Font m_mainFont72;
    static oxygine::Font m_logoFont;
};

#endif // FONTMANAGER_H
