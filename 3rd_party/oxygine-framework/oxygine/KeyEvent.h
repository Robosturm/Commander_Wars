#pragma once
#include "Event.h"

#include "QKeyEvent"

namespace oxygine
{

    class KeyEvent
    {
        public:
            KeyEvent() = default;
            KeyEvent(qint32 k, QString text, Qt::KeyboardModifiers modifiers)
                : m_key(static_cast<Qt::Key>(k)),
                  m_text(text),
                  m_modifiers(modifiers)
            {
            }

            KeyEvent(QKeyEvent* event)
                : m_key(static_cast<Qt::Key>(event->key())),
                  m_text(event->text()),
                  m_modifiers(event->modifiers()),
                  m_continousPress(event->isAutoRepeat())
            {
            }
            inline Qt::Key getKey()
            {
                return m_key;
            }
            inline QString getText()
            {
                return m_text;
            }
            inline Qt::KeyboardModifiers getModifiers()
            {
                return m_modifiers;
            }
            bool getContinousPress() const
            {
                return m_continousPress;
            }

            inline void setText(const QString &newText)
            {
                m_text = newText;
            }

            inline bool getInputEvent() const
            {
                return m_inputEvent;
            }

            inline void setInputEvent(bool newInputEvent)
            {
                m_inputEvent = newInputEvent;
            }

            inline bool getCommit() const
            {
                return m_Commit;
            }

            inline void setCommit(bool newCommit)
            {
                m_Commit = newCommit;
            }

        private:
            Qt::Key m_key{static_cast<Qt::Key>(0)};
            QString m_text;
            Qt::KeyboardModifiers m_modifiers{Qt::KeyboardModifier::NoModifier};
            bool m_continousPress{false};
            bool m_inputEvent{false};
            bool m_Commit;
    };
}
