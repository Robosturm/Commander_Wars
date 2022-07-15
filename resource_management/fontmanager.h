#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <QColor>
#include <QFont>

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
    inline static QFont getMainFont16()
    {
        static const QFont font = getFontWithPixelSize(m_fonts[MAINFONT], 16);
        return font;
    }
    /**
     * @brief getMainFont24
     * @return
     */
    inline static QFont getMainFont24()
    {
        static const QFont font = getFontWithPixelSize(m_fonts[MAINFONT], 24);
        return font;
    }
    /**
     * @brief getMainFont32
     * @return
     */
    inline static QFont getMainFont32()
    {
        static const QFont font = getFontWithPixelSize(m_fonts[MAINFONT], 32);
        return font;
    }
    /**
     * @brief getMainFont48
     * @return
     */
    inline static QFont getMainFont48()
    {
        static const QFont font = getFontWithPixelSize(m_fonts[MAINFONT], 48);
        return font;
    }
    /**
     * @brief getMainFont72
     * @return
     */
    inline static QFont getMainFont72()
    {
        static const QFont font = getFontWithPixelSize(m_fonts[MAINFONT], 72);
        return font;
    }
    /**
     * @brief getLogoFont
     * @return
     */
    inline static QFont getLogoFont()
    {
        static const QFont font = getFontWithPixelSize(m_fonts[LOGOFONT], 16);
        return font;
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

    static QFont getFontWithPixelSize(const QString & font, qint32 size);
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
