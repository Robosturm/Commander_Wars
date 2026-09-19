#include <QCoreApplication>
#include <QVariant>
#include <QFile>

#include <future>
#include <vector>

#include "3rd_party/oxygine-framework/oxygine/res/ResAtlasGeneric.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"

#include "coreengine/gameconsole.h"
#include "coreengine/settings.h"
#include "coreengine/virtualpaths.h"

namespace oxygine
{
    void ResAtlasGeneric::_unload()
    {
    }

    void ResAtlasGeneric::_load()
    {
    }

    namespace
    {
        struct PendingImage
        {
            QString file;
            QString path;
            QDomElement node;
            qint32 columns{1};
            qint32 rows{1};
            float scaleFactor{1.0f};
            quint32 linearFilter{0};
            std::future<QImage> future;
        };
    }

    void ResAtlasGeneric::loadAtlas(CreateResourceContext& context)
    {
        m_current = 0;
        QDomElement node = context.m_walker.getNode();
        loadBase(node);
        std::vector<QString> loadedPaths;
        std::vector<PendingImage> pending;
        // Pass 1: walk the (not thread safe) DOM tree and collect all image jobs to run.
        // Actual image decoding is deferred to pass 2 so it can happen concurrently.
        while (true)
        {
            QCoreApplication::processEvents(QEventLoop::ProcessEventsFlag::AllEvents, 5);
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
            QString path = walker.getPath("file");
            bool found = false;
            for (const auto& usedPath : loadedPaths)
            {
                if (usedPath == path)
                {
                    found = true;
                    break;
                }
            }
            if (found)
            {
                CONSOLE_PRINT("Duplicate entry found. " + path, GameConsole::eERROR);
                continue;
            }
            QString imgFilePath = VirtualPaths::find(path);
            if (!QFile::exists(imgFilePath))
            {
                CONSOLE_PRINT("Invalid item found. " + path, GameConsole::eERROR);
                continue;
            }
            loadedPaths.push_back(path);

            PendingImage item;
            item.file = file;
            item.path = path;
            item.node = child_node;
            item.rows = child_node.attribute("rows").toInt();
            item.columns = child_node.attribute("cols").toInt();
            if (item.rows <= 0)
            {
                item.rows = 1;
            }
            if (item.columns <= 0)
            {
                item.columns = 1;
            }
            bool ok = false;
            float scaleFactor = child_node.attribute("scale_factor").toFloat(&ok);
            if (!ok)
            {
                scaleFactor = 1.0f;
            }
            item.scaleFactor = walker.getScaleFactor() * scaleFactor;
            quint32 linearFilter = m_linearFilter;
            if (child_node.hasAttribute("linearFilter"))
            {
                QVariant linearFilterValue = QVariant(child_node.attribute("linearFilter"));
                if (linearFilterValue.typeId() == QMetaType::QString &&
                    !linearFilterValue.isNull())
                {
                    linearFilter = linearFilterValue.toBool() ? GL_LINEAR : GL_NEAREST;
                }
            }
            item.linearFilter = linearFilter;
            // kick off the (potentially expensive) image decode on a worker thread right away
            item.future = std::async(std::launch::async, [imgFilePath]()
            {
                return QImage(imgFilePath);
            });
            pending.push_back(std::move(item));
        }

        // Pass 2: collect the decoded images (already running concurrently since pass 1)
        // and create the GL textures on the calling (render) thread.
        std::vector<spResAnim> anims;
        for (auto & item : pending)
        {
            QImage img = item.future.get();
            if (img.width() == 0 || img.height() == 0)
            {
                CONSOLE_PRINT("Image is not valid " + item.path, GameConsole::eWARNING);
                continue;
            }
            CONSOLE_PRINT("Loading sprite: " + item.path, GameConsole::eDEBUG);
            spResAnim ra = MemoryManagement::create<ResAnim>(this);
            ra->setResPath(item.path);
            ra->init(img, item.columns, item.rows, item.scaleFactor, m_clamp2edge, item.linearFilter);
            ra->setParent(this);
            init_resAnim(ra, item.file, item.node);
            // add loaded res anim
            context.m_resources->add(ra);
            anims.push_back(ra);
        }
    }
}


