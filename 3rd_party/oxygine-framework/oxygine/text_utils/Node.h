#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/Object.h"
#include "3rd_party/oxygine-framework/oxygine/math/Vector2.h"
#include "3rd_party/oxygine-framework/oxygine/text_utils/Aligner.h"

#include <QDomElement>
#include <vector>

#include <QString>
#include <QPainter>
#include <QPainterPath>

namespace oxygine
{
    class RenderState;
    class Aligner;
    class STDRenderer;
    class RenderState;

    namespace text
    {
        class Node;
        using spNode = oxygine::intrusive_ptr<Node>;
        class DivNode;
        using spDivNode = intrusive_ptr<DivNode>;
        class BrNode;
        using spBrNode = intrusive_ptr<BrNode>;
        class TextNode;
        using spTextNode = oxygine::intrusive_ptr<TextNode>;

        class DrawContext
        {
        public:
            explicit DrawContext() = default;
            virtual ~DrawContext() = default;
            QColor m_color;
            QColor m_primary;
        };

        class Node : public oxygine::ref_counter
        {
        public:
            explicit Node() = default;
            virtual ~Node() = default;

            void appendNode(spNode tn);
            virtual void resize(Aligner& rd);

            void drawChildren(const RenderState& rs, const TextStyle & style, DrawContext& dc, QPainter & painter, Rect & scissorRect, bool scissorEnabled);
            void resizeChildren(Aligner& rd);
            virtual void draw(const RenderState& rs, const TextStyle & style, DrawContext& dc, QPainter & painter, Rect & scissorRect, bool scissorEnabled);
            virtual void xresize(Aligner&) {}

            QPoint getRelativPos() const;
            void setRelativPos(QPoint newRelativPos);
        protected:
            QPoint m_relativPos;
        private:
            spNode m_firstChild;
            spNode m_lastChild;
            spNode m_nextSibling;
        };

        class TextNode: public Node
        {
        public:
            explicit TextNode(const QString & v);
            virtual ~TextNode() = default;
            virtual void xresize(Aligner& rd) override;
            virtual void draw(const RenderState& rs, const TextStyle & style, DrawContext& dc, QPainter & painter, Rect & scissorRect, bool scissorEnabled) override;
        private:
            std::vector<QString> m_splitData;
            std::vector<qint32> m_yPos;
        };

        class DivNode: public Node
        {
        public:
            explicit DivNode(QDomElement& reader);
            virtual ~DivNode() = default;
            virtual void resize(Aligner& rd) override;
            virtual void draw(const RenderState& rs, const TextStyle & style, DrawContext& dc, QPainter & painter, Rect & scissorRect, bool scissorEnabled) override;
        private:
            QColor m_color;
            quint32 m_options;
        };

        class BrNode: public Node
        {
        public:
            explicit BrNode() = default;
            virtual ~BrNode() = default;
            virtual void xresize(Aligner& rd) override
            {
                rd.nextLine();
            }
        };
    }
}
