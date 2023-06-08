#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <QColor>
#include "3rd_party/oxygine-framework/oxygine/TextStyle.h"

#include "resource_management/ressourcemanagement.h"

class FontManager final : public QObject
{
    Q_OBJECT
public:

    static const char* const MAINFONT;
    static const char* const LOGOFONT;
    static const char* const MENUFONT;

    static FontManager* getInstance()
    {
        if (m_pInstance == nullptr)
        {
            m_pInstance = new FontManager();
        }
        return m_pInstance;
    }
    /**
     * @brief getMainFont16
     * @return
     */
    inline static oxygine::Font & getMainFont16()
    {
        return m_pInstance->m_fonts[MAINFONT + QString::number(16)];
    }
    /**
     * @brief getMainFont24
     * @return
     */
    inline static oxygine::Font & getMainFont24()
    {
        return m_pInstance->m_fonts[MAINFONT + QString::number(24)];
    }
    /**
     * @brief getMainFont32
     * @return
     */
    inline static oxygine::Font & getMainFont32()
    {
        return m_pInstance->m_fonts[MAINFONT + QString::number(32)];
    }
    /**
     * @brief getMainFont32
     * @return
     */
    inline static oxygine::Font & getMenuFont32()
    {
        return m_pInstance->m_fonts[MENUFONT + QString::number(32)];
    }
    /**
     * @brief getMainFont48
     * @return
     */
    inline static oxygine::Font & getMainFont48()
    {
        return m_pInstance->m_fonts[MAINFONT + QString::number(48)];
    }
    /**
     * @brief getMainFont72
     * @return
     */
    inline static oxygine::Font & getMainFont72()
    {
        return m_pInstance->m_fonts[MAINFONT + QString::number(72)];
    }
    /**
     * @brief getLogoFont
     * @return
     */
    inline static oxygine::Font & getLogoFont()
    {
        return m_pInstance->m_fonts[LOGOFONT + QString::number(16)];
    }
    /**
     * @brief getFont
     * @param fontType
     * @param size
     * @return
     */
    static oxygine::Font & getFont(QString fontType, qint32 size)
    {
        QString key = fontType + QString::number(size);
        if (m_pInstance->m_fonts.contains(key))
        {
            return m_pInstance->m_fonts[key];
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
    static oxygine::Font & getFont(QString fontType)
    {
        QString key = fontType;
        if (m_pInstance->m_fonts.contains(key))
        {
            return m_pInstance->m_fonts[key];
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
   virtual ~FontManager() = default;

private:
    static FontManager* m_pInstance;
    QMap<QString, oxygine::Font> m_fonts;
    QColor m_defaultColor{QColor(250, 210, 0)};
};

#endif // FONTMANAGER_H
