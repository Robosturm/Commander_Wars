#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"
#include <chrono>

namespace oxygine
{
    class Clock;
    using spClock = intrusive_ptr<Clock>;
    class Clock final : public ref_counter
    {
    public:
        explicit Clock() = default;
        ~Clock() = default;
        timeMS getTime() const;
        void pause();
        void resume();
        void resetPause();
        void update(timeMS globalTime = timeMS(-1));
        timeMS doTick();

        static inline timeMS getTimeMS()
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>((std::chrono::system_clock::now().time_since_epoch()));
        }
    private:
        qint32  m_pauseCounter{0};
        double  m_destTime{0};
        double  m_srcTime{0};
        qint32  m_lastDT{0};
        timeMS  m_lastUpdateTime{-1};
    };
}
