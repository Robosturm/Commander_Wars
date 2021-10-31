#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/Object.h"
#include "3rd_party/oxygine-framework/oxygine/math/Vector2.h"
#include "3rd_party/oxygine-framework/oxygine/text_utils/Aligner.h"
#include <qstring.h>
#include <QDomElement>
#include <vector>

namespace oxygine
{
    class RenderState;
    struct glyph;
    class Aligner;
    class STDRenderer;
    class RenderState;

    namespace text
    {
        using text_data = std::vector<Symbol>;
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
            void finalPass(Aligner& rd);

            void drawChildren(DrawContext& dc);
            void resizeChildren(Aligner& rd);
            virtual Symbol* getSymbol(int& pos);
            virtual void draw(DrawContext& dc);
            virtual void xresize(Aligner&) {}
            virtual void xfinalPass(Aligner&) {}
            virtual void xupdateMaterial(const Material&) {}
            void updateMaterial(const Material& mat);

        private:
            spNode m_firstChild;
            spNode m_lastChild;
            spNode m_nextSibling;
        };

        class TextNode: public Node
        {
        public:
            static qint32 m_defMissingGlyph;
            explicit TextNode(QString v);
            virtual ~TextNode() = default;
            virtual void xresize(Aligner& rd) override;
            virtual void xfinalPass(Aligner& rd) override;
            virtual void draw(DrawContext& dc) override;
            virtual void xupdateMaterial(const Material& mat) override;
            virtual Symbol* getSymbol(int& pos) override;
        private:
            text_data m_data;
        };

        class DivNode: public Node
        {
        public:
            explicit DivNode(QDomElement& reader);
            virtual ~DivNode() = default;
            virtual void resize(Aligner& rd) override;
            virtual void draw(DrawContext& dc) override;
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
