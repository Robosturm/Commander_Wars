#include <QBuffer>
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QImageReader>
#include <QSet>
#include <QVariant>
#include <QFile>

#include <algorithm>
#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "3rd_party/oxygine-framework/oxygine/res/ResAtlasGeneric.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"

#include "coreengine/gameconsole.h"
#include "coreengine/settings.h"
#include "coreengine/virtualpaths.h"

#include "spritingsupport/spritecreator.h"

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
            QString filePath;
            QDomElement node;
            qint32 columns{1};
            qint32 rows{1};
            float scaleFactor{1.0f};
            quint32 linearFilter{0};
        };

        /**
         * @brief Number of images decoded in parallel.
         * This must stay bounded: an atlas can declare over 1300 images and launching a decode for
         * every one of them at once keeps every decoded RGBA image alive at the same time (the
         * background atlas alone is ~350 MB decompressed).
         */
        std::size_t getMaxInFlightDecodes()
        {
            const unsigned int cores = std::thread::hardware_concurrency();
            return std::clamp<std::size_t>(cores, 2, 8);
        }

        /**
         * @brief Decodes images on a fixed set of threads that are started once per atlas.
         * std::async is deliberately not used here: it dispatches every single task through the
         * platform thread pool, which on Windows added roughly two seconds of latency per image
         * regardless of the image size.
         */
        class DecodePool final
        {
        public:
            explicit DecodePool(std::size_t threadCount)
            {
                m_threads.reserve(threadCount);
                for (std::size_t i = 0; i < threadCount; ++i)
                {
                    m_threads.emplace_back([this](){ workerLoop(); });
                }
            }
            ~DecodePool()
            {
                {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    m_stop = true;
                }
                m_condition.notify_all();
                for (auto & thread : m_threads)
                {
                    thread.join();
                }
            }
            DecodePool(const DecodePool&) = delete;
            DecodePool& operator=(const DecodePool&) = delete;

            std::future<QImage> submit(QByteArray encoded)
            {
                auto task = std::make_shared<std::packaged_task<QImage()>>([encoded = std::move(encoded)]() mutable
                {
                    QBuffer buffer(&encoded);
                    buffer.open(QIODevice::ReadOnly);
                    QImageReader reader(&buffer);
                    QImage img = reader.read();
                    SpriteCreator::convertToRgba(img);
                    return img;
                });
                std::future<QImage> result = task->get_future();
                {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    m_queue.push_back([task](){ (*task)(); });
                }
                m_condition.notify_one();
                return result;
            }

        private:
            void workerLoop()
            {
                while (true)
                {
                    std::function<void()> job;
                    {
                        std::unique_lock<std::mutex> lock(m_mutex);
                        m_condition.wait(lock, [this](){ return m_stop || !m_queue.empty(); });
                        if (m_queue.empty())
                        {
                            return;
                        }
                        job = std::move(m_queue.front());
                        m_queue.pop_front();
                    }
                    job();
                }
            }

            std::vector<std::thread> m_threads;
            std::deque<std::function<void()>> m_queue;
            std::mutex m_mutex;
            std::condition_variable m_condition;
            bool m_stop{false};
        };
    }

    void ResAtlasGeneric::loadAtlas(CreateResourceContext& context)
    {
        m_current = 0;
        QElapsedTimer atlasTimer;
        atlasTimer.start();
        QDomElement node = context.m_walker.getNode();
        loadBase(node);
        QSet<QString> loadedPaths;
        std::vector<PendingImage> pending;
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
            if (loadedPaths.contains(path))
            {
                CONSOLE_PRINT("Duplicate entry found. " + path, GameConsole::eERROR);
                continue;
            }
            QString imgFilePath = VirtualPaths::find(path);
            if (imgFilePath.isEmpty())
            {
                CONSOLE_PRINT("Invalid item found. " + path, GameConsole::eERROR);
                continue;
            }
            loadedPaths.insert(path);

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
            item.filePath = imgFilePath;
            pending.push_back(std::move(item));
        }

        const std::size_t maxInFlight = getMaxInFlightDecodes();
        DecodePool decodePool(maxInFlight);
        std::vector<std::future<QImage>> futures(pending.size());
        auto scheduleDecode = [&futures, &pending, &decodePool](std::size_t index)
        {
            QByteArray encoded;
            QFile file(pending[index].filePath);
            if (file.open(QIODevice::ReadOnly))
            {
                encoded = file.readAll();
            }
            futures[index] = decodePool.submit(std::move(encoded));
        };
        std::size_t nextToSchedule = std::min(maxInFlight, pending.size());
        for (std::size_t i = 0; i < nextToSchedule; ++i)
        {
            scheduleDecode(i);
        }

        std::vector<spResAnim> anims;
        anims.reserve(pending.size());
        qint64 decodeWaitMs = 0;
        qint64 uploadMs = 0;
        qint64 eventsMs = 0;
        QElapsedTimer sectionTimer;
        for (std::size_t i = 0; i < pending.size(); ++i)
        {
            auto & item = pending[i];
            sectionTimer.start();
            QImage img = futures[i].get();
            decodeWaitMs += sectionTimer.elapsed();
            futures[i] = std::future<QImage>();
            if (nextToSchedule < pending.size())
            {
                scheduleDecode(nextToSchedule);
                ++nextToSchedule;
            }
            sectionTimer.start();
            QCoreApplication::processEvents(QEventLoop::ProcessEventsFlag::AllEvents, 5);
            eventsMs += sectionTimer.elapsed();
            if (img.width() == 0 || img.height() == 0)
            {
                CONSOLE_PRINT("Image is not valid " + item.path, GameConsole::eWARNING);
                continue;
            }
            CONSOLE_PRINT("Loading sprite: " + item.path, GameConsole::eDEBUG);
            sectionTimer.start();
            spResAnim ra = MemoryManagement::create<ResAnim>(this);
            ra->setResPath(item.path);
            ra->init(img, item.columns, item.rows, item.scaleFactor, m_clamp2edge, item.linearFilter, false);
            ra->setParent(this);
            uploadMs += sectionTimer.elapsed();
            init_resAnim(ra, item.file, item.node);
            // add loaded res anim
            context.m_resources->add(ra);
            anims.push_back(ra);
        }
        CONSOLE_PRINT("Loaded atlas " + context.m_xml_name + " with " + QString::number(anims.size()) +
                      " sprites in " + QString::number(atlasTimer.elapsed()) + " ms" +
                      " (decode wait " + QString::number(decodeWaitMs) + " ms" +
                      ", gpu upload " + QString::number(uploadMs) + " ms" +
                      ", events " + QString::number(eventsMs) + " ms)", GameConsole::eINFO);
    }
}


