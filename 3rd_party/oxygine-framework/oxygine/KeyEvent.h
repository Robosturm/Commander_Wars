#pragma once
#include "3rd_party/oxygine-framework/oxygine/Event.h"
#include "QKeyEvent"

namespace oxygine
{

    class KeyEvent
    {
        public:
            KeyEvent(qint32 k, const QString & text, Qt::KeyboardModifiers modifiers)
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

            KeyEvent(QInputMethodEvent* event)
                : m_text(event->commitString()),
                  m_inputEvent(true),
                  m_Length(event->replacementLength()),
                  m_start(event->replacementStart()),
                  m_AttributeList(event->attributes())
            {
                if (m_text.isEmpty())
                {
                    m_text = event->preeditString();
                }
            }

            inline Qt::Key getKey()
            {
                return m_key;
            }
            inline const QString & getText()
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
            const QList<QInputMethodEvent::Attribute> & getAttributeList()
            {
                return m_AttributeList;
            }

            inline qint32 getStart() const
            {
                return m_start;
            }

            inline qint32 getLength() const
            {
                return m_Length;
            }
        private:
            Qt::Key m_key{static_cast<Qt::Key>(0)};
            QString m_text;
            Qt::KeyboardModifiers m_modifiers{Qt::KeyboardModifier::NoModifier};
            bool m_continousPress{false};

            bool m_inputEvent{false};
            qint32 m_Length{0};
            qint32 m_start{0};
            const QList<QInputMethodEvent::Attribute> m_AttributeList;
    };


}
