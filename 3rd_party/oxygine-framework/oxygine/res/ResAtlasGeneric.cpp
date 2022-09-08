#include <QCoreApplication>
#include <QVariant>
#include <QFile>

#include "3rd_party/oxygine-framework/oxygine/res/ResAtlasGeneric.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"

#include "spritingsupport/spritecreator.h"
#include "coreengine/console.h"
#include "coreengine/settings.h"

namespace oxygine
{
    void ResAtlasGeneric::_unload()
    {
    }

    void ResAtlasGeneric::_load()
    {
    }

    void ResAtlasGeneric::loadAtlas(CreateResourceContext& context)
    {
        m_current = 0;
        QDomElement node = context.m_walker.getNode();
        loadBase(node);
        std::vector<spResAnim> anims;
        while (true)
        {
            QCoreApplication::processEvents();
            XmlWalker walker = context.m_walker.next();
            if (walker.empty())
            {
                break;
            }

            QDomElement child_node = walker.getNode();

            QString name = child_node.nodeName();
            if (name != "image")
            {
                continue;
            }

            QString file = child_node.attribute("file");

            if (file.isEmpty())
            {
                createEmpty(walker, context);
                continue;
            }
            bool found = false;
            for (const auto& item : anims)
            {
                if (item->getResPath() == walker.getPath("file"))
                {
                    found = true;
                    break;
                }
            }
            if (found)
            {
                CONSOLE_PRINT("Duplicate entry found. " + walker.getPath("file"), Console::eERROR);
                continue;
            }
            qint32 columns = 0;
            qint32 rows = 0;
            QImage img;
            if (QFile::exists(Settings::getUserPath() + walker.getPath("file")))
            {
                img = QImage(Settings::getUserPath() + walker.getPath("file"));
            }
            else if (QFile::exists(RCC_PREFIX_PATH + walker.getPath("file")))
            {
                img = QImage(RCC_PREFIX_PATH + walker.getPath("file"));
            }
            else
            {
                CONSOLE_PRINT("Invalid item found. " + walker.getPath("file"), Console::eERROR);
                continue;
            }
            QString path = walker.getPath("file");
            CONSOLE_PRINT("Loading sprite: " + path, Console::eDEBUG);
            if (img.width() == 0 || img.height() == 0)
            {
                CONSOLE_PRINT("Image is not valid " + walker.getPath("file"), Console::eWARNING);
                continue;
            }
            rows = child_node.attribute("rows").toInt();
            columns = child_node.attribute("cols").toInt();
            bool ok = false;
            float scaleFactor = child_node.attribute("scale_factor").toFloat(&ok);
            if (!ok)
            {
                scaleFactor = 1.0f;
            }
            bool maskExtend = true;
            if (child_node.hasAttribute("maskExtend"))
            {
                QVariant varMaskExtend = QVariant(child_node.attribute("maskExtend"));
                if (varMaskExtend.typeId() == QMetaType::QString &&
                    !varMaskExtend.isNull() &&
                    child_node.hasAttribute("maskExtend"))
                {
                    maskExtend = varMaskExtend.toBool();
                }
            }
            quint32 linearFilter = m_linearFilter;
            if (child_node.hasAttribute("linearFilter"))
            {
                QVariant linearFilter = QVariant(child_node.attribute("linearFilter"));
                if (linearFilter.typeId() == QMetaType::QString &&
                    !linearFilter.isNull() &&
                    child_node.hasAttribute("linearFilter"))
                {
                    if (linearFilter.toBool())
                    {
                        linearFilter = GL_LINEAR;
                    }
                    else
                    {
                        linearFilter = GL_NEAREST;
                    }
                }
            }
            if (rows <= 0)
            {
                rows = 1;
            }
            if (columns <= 0)
            {
                columns = 1;
            }
            if (path.endsWith("+mask.png") && maskExtend)
            {
                QString basePath = path;
                basePath = basePath.replace("+mask.png", ".png");
                QString overlayPath = basePath;
                if (QFile::exists(Settings::getUserPath() + basePath))
                {
                    overlayPath = Settings::getUserPath() + basePath;
                }
                else
                {
                    overlayPath = oxygine::Resource::RCC_PREFIX_PATH + basePath;
                }
                if (QFile::exists(overlayPath))
                {
                    CONSOLE_PRINT("SpriteCreator::preProcessMask for file " + path, Console::eDEBUG);
                    QImage overlay(overlayPath);
                    SpriteCreator::preProcessMask(img, overlay, columns, rows);
                }
            }
            spResAnim ra = spResAnim::create(this);
            ra->setResPath(path);
            ra->init(img, columns, rows, walker.getScaleFactor() * scaleFactor, context.m_addTransparentBorder, m_clamp2edge, linearFilter);
            ra->setParent(this);
            init_resAnim(ra, file, child_node);
            // add loaded res anim
            context.m_resources->add(ra);
            anims.push_back(ra);
        }

    }
}

