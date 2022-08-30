#include <QFontDatabase>

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"

const char* const FontManager::MAINFONT = "main";
const char* const FontManager::LOGOFONT = "logo";


QColor FontManager::m_defaultColor{QColor(250, 210, 0)};

oxygine::Font FontManager::m_mainFont16;
oxygine::Font FontManager::m_mainFont24;
oxygine::Font FontManager::m_mainFont32;
oxygine::Font FontManager::m_mainFont48;
oxygine::Font FontManager::m_mainFont72;
oxygine::Font FontManager::m_logoFont;

QMap<QString, oxygine::Font> FontManager::m_fonts;
FontManager* FontManager::m_pInstance = nullptr;

FontManager::FontManager()
{
#ifdef GRAPHICSUPPORT
        setObjectName("FontManager");
#endif
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
                                        auto type = element.attribute("type");
                                        auto & font = m_fonts[type];
                                        font.font = QFont(fonts[0]);
                                        font.font.setPixelSize(element.attribute("pixelSize").toInt());
                                        font.offsetX = element.attribute("offsetX").toInt();
                                        font.offsetY = element.attribute("offsetY").toInt();
                                        font.borderWidth = element.attribute("borderWidth").toInt();
                                        if (element.hasAttribute("borderCapStyle"))
                                        {
                                            font.borderCapStyle = static_cast<Qt::PenCapStyle>(element.attribute("borderCapStyle").toInt());
                                        }
                                        if (element.hasAttribute("borderJoin"))
                                        {
                                            font.borderJoin = static_cast<Qt::PenJoinStyle>(element.attribute("borderJoin").toInt());
                                        }
                                        if (element.hasAttribute("bold") && element.attribute("bold") == "true")
                                        {
                                            font.font.setBold(true);
                                        }
                                        else
                                        {
                                            font.font.setBold(false);
                                        }
                                        if (font.borderWidth > 0)
                                        {
                                            font.font.setStyleStrategy(QFont::ForceOutline);
                                        }
                                    }
                                }

                            }
                        }
                    }
                    node = node.nextSibling();
                }
            }
            else
            {
                CONSOLE_PRINT("Unable to load: " + folder + "res.xml", Console::eERROR);
                CONSOLE_PRINT("Error: " + error + " at line " + QString::number(line) + " at column " + QString::number(column), Console::eERROR);
            }
        }
    }
    m_mainFont16 = m_fonts[MAINFONT + QString::number(16)];
    m_mainFont24 = m_fonts[MAINFONT + QString::number(24)];
    m_mainFont32 = m_fonts[MAINFONT + QString::number(32)];
    m_mainFont48 = m_fonts[MAINFONT + QString::number(48)];
    m_mainFont72 = m_fonts[MAINFONT + QString::number(72)];
    m_logoFont = m_fonts[LOGOFONT + QString::number(16)];
}
void FontManager::setFontColor(QColor color)
{
    m_defaultColor = color;
}

QColor FontManager::getFontColor()
{
    return m_defaultColor;
}
