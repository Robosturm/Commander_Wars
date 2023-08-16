#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/text_utils/Aligner.h"
#include "3rd_party/oxygine-framework/oxygine/Clock.h"

#include <QDomElement>
#include <vector>

#include <QString>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>

namespace oxygine
{
    class RenderState;
    class Aligner;
    class STDRenderer;
    class RenderState;

    namespace text
    {
        class Node;
        using spNode = std::shared_ptr<Node>;
        class DivNode;
        using spDivNode = std::shared_ptr<DivNode>;
        class BrNode;
        using spBrNode = std::shared_ptr<BrNode>;
        class TextNode;
        using spTextNode = std::shared_ptr<TextNode>;
        class WigglyNode;
        using spWigglyNode = std::shared_ptr<WigglyNode>;

        class Node
        {
        public:
            explicit Node() = default;
            virtual ~Node() = default;

            void appendNode(spNode tn);
            virtual void resize(Aligner& rd);

            void drawChildren(const RenderState& rs, const TextStyle & style, const QColor & drawColor, QPainter & painter);
            void resizeChildren(Aligner& rd);
            virtual void draw(const RenderState& rs, const TextStyle & style, const QColor & drawColor, QPainter & painter);
            virtual void xresize(Aligner&) {}
            virtual qint32 getWidth(Aligner& aligner)
            {
                return 0;
            }
            virtual void setX(qint32 x)
            {
            }
            Node* getFirstChild() const
            {
                return m_firstChild.get();
            }
            Node* getNextSibling() const
            {
                return m_nextSibling.get();
            }

        private:
            spNode m_firstChild;
            spNode m_lastChild;
            spNode m_nextSibling;
        };

        class TextNode : public Node
        {
        public:
            explicit TextNode(const QString & v);
            virtual ~TextNode() = default;
            virtual void xresize(Aligner& rd) override;
            virtual void draw(const RenderState& rs, const TextStyle & style, const QColor & drawColor, QPainter & painter) override;
            virtual qint32 getWidth(Aligner& rd) override;
            virtual void setX(qint32 x) override;
            const std::vector<QString> & getLines() const
            {
                return m_lines;
            }
            const std::vector<QPoint> & getPositions() const
            {
                return m_offsets;
            }
        private:
#ifdef GRAPHICSUPPORT
            QString * addNewLine(Aligner& rd);
#endif
        protected:
            QString m_text;
            std::vector<QString> m_lines;
            std::vector<QPoint> m_offsets;
            QPainterPath m_path;
        };

        class WigglyNode final : public TextNode
        {
        public:
            explicit WigglyNode(QDomElement& reader, const QString & v);
            virtual ~WigglyNode() = default;
            virtual void draw(const RenderState& rs, const TextStyle & style, const QColor & drawColor, QPainter & painter) override;
        private:
            qint32 m_step{0};
            qint32 m_advance{1};
            timeMS m_lastStepTime{0};
            timeMS m_speed{60};
            float m_maxY{0.0f};
        };

        class DivNode final : public Node
        {
        public:
            explicit DivNode(QDomElement& reader);
           virtual ~DivNode() = default;
            virtual void resize(Aligner& rd) override;
            virtual void draw(const RenderState& rs, const TextStyle & style, const QColor & drawColor, QPainter & painter) override;
        private:
            QColor m_color;
        };

        class BrNode final: public Node
        {
        public:
            explicit BrNode() = default;
            virtual ~BrNode() = default;
            void xresize(Aligner& rd) override
            {
                rd.nextLine(0, 0);
            }
        };
    }
}
