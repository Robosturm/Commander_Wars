#pragma once
#include <QVulkanWindow>
#include <QMutex>

namespace oxygine
{
    class VulkanRenderer;

    class WindowBase : public QVulkanWindow
    {
        Q_OBJECT
    public:
        WindowBase();
        virtual ~WindowBase() = default;
        /**
         * @brief createRenderer
         * @return
         */
        virtual QVulkanWindowRenderer* createRenderer() override;

        void redrawUi();
        bool beginRendering();
    public slots:
        virtual void launchGame() = 0;
    protected:
        virtual void updateData() = 0;
        virtual void timerEvent(QTimerEvent *) override;
    private:
        void setup();
    protected:
        friend class VulkanRenderer;
        bool m_renderEnabled{true};
        qint32 m_repeatedFramesDropped{0};
        QMutex m_pauseMutex;
        qint32 m_pausedCounter{0};
        bool m_quit{false};
        bool m_noUi{false};
    private:
        static QVulkanInstance m_vulkanInstance;
    };
}
