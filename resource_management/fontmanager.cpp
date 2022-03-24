#include <QFontDatabase>

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"

const char* const FontManager::MAINFONT = "main";
const char* const FontManager::LOGOFONT = "logo";

QColor FontManager::m_defaultColor{QColor(250, 210, 0)};
QMap<QString, QString> FontManager::m_fonts;
FontManager* FontManager::m_pInstance = nullptr;

FontManager::FontManager()
{
    setObjectName("FontManager");
    QStringList searchFolders;
    searchFolders.append("resources/fonts/");
    searchFolders.append(QString(oxygine::Resource::RCC_PREFIX_PATH) + "resources/fonts/");
    QStringList mods = Settings::getMods();
    for (const auto & mod : qAsConst(mods))
    {
        searchFolders.append(mod + "/fonts/");
    }
    for (qint32 i = searchFolders.size() - 1; i >= 0; --i)
    {
        QString folder = searchFolders[i];
        if (QFile::exists(folder + "res.xml"))
        {
            QDomDocument document;
            QFile file(folder + "res.xml");
            file.open(QIODevice::ReadOnly);
            QString error;
            qint32 line;
            qint32 column;
            bool loaded = document.setContent(&file, &error, &line, &column);
            if (loaded)
            {
                auto rootElement = document.documentElement();
                auto node = rootElement.firstChild();
                while (!node.isNull())
                {
                    while (node.isComment())
                    {
                        node = node.nextSibling();
                    }
                    if (!node.isNull())
                    {
                        if (node.nodeName() == "font")
                        {
                            auto element = node.toElement();
                            QString file = element.attribute("file");
                            if (QFile::exists(folder + file))
                            {
                                qint32 res = QFontDatabase::addApplicationFont(folder + file);
                                if (res >= 0)
                                {
                                    QStringList fonts = QFontDatabase::applicationFontFamilies(res);
                                    if (fonts.size() > 0)
                                    {
                                        m_fonts[element.attribute("type")] = fonts[0];
                                    }
                                }
                            }
                        }
                    }
                    node = node.nextSibling();
                }
                m_fonts[MAINFONT] = "Helvetica";
            }
            else
            {
                CONSOLE_PRINT("Unable to load: " + folder + "res.xml", Console::eERROR);
                CONSOLE_PRINT("Error: " + error + " at line " + QString::number(line) + " at column " + QString::number(column), Console::eERROR);
            }
        }
    }
}

const QString &FontManager::getLogoFontName()
{
    return m_fonts[LOGOFONT];
}

const QString &FontManager::getMainFontName()
{
    return m_fonts[MAINFONT];
}

void FontManager::setFontColor(QColor color)
{
    m_defaultColor = color;
}

QColor FontManager::getFontColor()
{
    return m_defaultColor;
}

QFont FontManager::getFontWithPixelSize(const QString & font, qint32 size)
{
    QFont qFont(font);
    qFont.setPixelSize(size);
    return qFont;
}
