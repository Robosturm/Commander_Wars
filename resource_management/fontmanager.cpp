#include <QFontDatabase>
#include <QtGlobal>

#include "coreengine/virtualpaths.h"

#include "resource_management/fontmanager.h"

const char* const FontManager::MAINFONT = "main";
const char* const FontManager::LOGOFONT = "logo";
const char* const FontManager::MENUFONT = "menu";

spFontManager FontManager::m_pInstance;

FontManager::FontManager()
{
#ifdef GRAPHICSUPPORT
    Interpreter::setCppOwnerShip(this);
    setObjectName("FontManager");
    QStringList searchFolders = VirtualPaths::createSearchPath("resources/fonts/");
    for (qint32 i = 0; i < searchFolders.size(); ++i)
    {
        QString folder = searchFolders[i];
        if (QFile::exists(folder + "res.xml"))
        {
            QDomDocument document;
            QFile file(folder + "res.xml");
            file.open(QIODevice::ReadOnly);
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
            auto result = document.setContent(&file);
            bool loaded = static_cast<bool>(result);
            QString errorMessage = result.errorMessage;
            qsizetype errorLine = result.errorLine;
            qsizetype errorColumn = result.errorColumn;
#else
            QString errorMessage;
            int errorLine = 0;
            int errorColumn = 0;
            bool loaded = document.setContent(&file, &errorMessage, &errorLine, &errorColumn);
#endif
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
                            if (QFile::exists(folder + "/" + file))
                            {
                                qint32 res = QFontDatabase::addApplicationFont(folder + "/" + file);
                                if (res >= 0)
                                {
                                    QStringList fonts = QFontDatabase::applicationFontFamilies(res);
                                    if (fonts.size() > 0)
                                    {
                                        auto type = element.attribute("type");
                                        auto & font = m_fonts[type];
                                        font.font = QFont(fonts[0]);
                                        font.font.setPixelSize(element.attribute("pixelSize").toInt());
                                        font.name = type;
                                        font.offsetX = element.attribute("offsetX").toInt();
                                        font.offsetY = element.attribute("offsetY").toInt();
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
                                        if (element.hasAttribute("mainColor"))
                                        {
                                            font.mainColor = QColor(element.attribute("mainColor"));
                                        }
                                        else
                                        {
                                            font.mainColor = m_defaultColor;
                                        }
                                        if (element.hasAttribute("antialiasing") && element.attribute("antialiasing") == "true")
                                        {
                                            font.antialiasing = true;
                                        }
                                        else
                                        {
                                            font.antialiasing = false;
                                        }
                                    }
                                    else
                                    {
                                        CONSOLE_PRINT_MODULE("Unable to load font file: " + folder + "/" + file, GameConsole::eERROR, GameConsole::eResources);
                                    }
                                }
                                else
                                {
                                    CONSOLE_PRINT_MODULE("Unable to load font file: " + folder + "/" + file, GameConsole::eERROR, GameConsole::eResources);
                                }
                            }
                        }
                    }
                    node = node.nextSibling();
                }
            }
            else
            {
                CONSOLE_PRINT("Unable to load: " + folder + "res.xml", GameConsole::eERROR);
                CONSOLE_PRINT("Error: " + errorMessage + " at line " + QString::number(errorLine) + " at column " + QString::number(errorColumn), GameConsole::eERROR);
            }
        }
    }
    if (!m_fonts.contains(MAINFONT + QString::number(16)))
    {
        CONSOLE_PRINT("Failed to load any font files.", GameConsole::eERROR);
    }
#endif
}
void FontManager::setFontColor(QColor color)
{
    m_pInstance->m_defaultColor = color;
}

QColor FontManager::getFontColor()
{
    return m_pInstance->m_defaultColor;
}
