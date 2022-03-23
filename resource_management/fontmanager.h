#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <QColor>
#include <QFont>

#include "resource_management/ressourcemanagement.h"

class FontManager : public QObject
{
    Q_OBJECT
public:
    enum SpecialChars
    {
        unlockChar = 1,
        lockChar = 2
    };

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
    inline static QFont getMainFont16()
    {
        return QFont(m_fonts[MAINFONT], 12);
    }
    /**
     * @brief getMainFont24
     * @return
     */
    inline static QFont getMainFont24()
    {
        return QFont(m_fonts[MAINFONT], 18);
    }
    /**
     * @brief getMainFont48
     * @return
     */
    inline static QFont getMainFont48()
    {
        return QFont(m_fonts[MAINFONT], 44);
    }
    /**
     * @brief getMainFont48
     * @return
     */
    inline static QFont getMainFont32()
    {
        return QFont(m_fonts[MAINFONT], 28);
    }
    /**
     * @brief getMainFont72
     * @return
     */
    inline static QFont getMainFont72()
    {
        return QFont(m_fonts[MAINFONT], 68);
    }
    /**
     * @brief getLogoFont
     * @return
     */
    inline static QFont getLogoFont()
    {
        return QFont(m_fonts[LOGOFONT], 14);
    }
    /**
     * @brief getMainFontName
     * @return
     */
    static const QString &getMainFontName();
    /**
     * @brief getLogoFontName
     * @return
     */
    static const QString &getLogoFontName();
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
    /**
     * @brief getFont
     * @param fontType
     * @param size
     * @return
     */
    static QFont getFont(QString fontType, qint32 size)
    {
        return QFont(m_fonts[fontType], size);
    }

protected:
    friend RessourceManagement<FontManager>;
    FontManager();
private:

    virtual ~FontManager() = default;
    static QColor m_defaultColor;
    static QMap<QString, QString> m_fonts;
    static FontManager* m_pInstance;
};

#endif // FONTMANAGER_H
