#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/Object.h"
#include "3rd_party/oxygine-framework/oxygine/math/Vector2.h"
#include "3rd_party/oxygine-framework/oxygine/text_utils/Aligner.h"
#include <qstring.h>
#include <QDomElement>

namespace pugi
{
    class xml_node;
}

namespace oxygine
{
    class RenderState;
    struct glyph;

    class Aligner;
    class STDRenderer;
    class RenderState;

    namespace text
    {
        typedef QVector<Symbol> text_data;


        class DrawContext
        {
        public:
            DrawContext() {}

            QColor m_color;
            QColor m_primary;
        };

        class Node;
        typedef oxygine::intrusive_ptr<Node> spNode;

        class Node : public oxygine::ref_counter
        {
        public:
            Node();
            virtual ~Node();

            void appendNode(spNode tn);
            virtual void resize(Aligner& rd);
            void finalPass(Aligner& rd);

            void drawChildren(DrawContext& dc);
            void resizeChildren(Aligner& rd);
            virtual Symbol* getSymbol(int& pos);

            virtual void draw(DrawContext& dc);

            virtual void xresize(Aligner&) {}
            virtual void xfinalPass(Aligner&) {}

            void updateMaterial(const STDMaterial& mat);
            virtual void xupdateMaterial(const STDMaterial&) {}

        public:
            spNode m_firstChild;
            spNode m_lastChild;
            spNode m_nextSibling;
        };

        class TextNode;
        typedef oxygine::intrusive_ptr<TextNode> spTextNode;

        class TextNode: public Node
        {
        public:
            static void setDefaultMissingSymbol(int);

            TextNode(QString v);

            text_data _data;
            void xresize(Aligner& rd) override;
            void xfinalPass(Aligner& rd) override;
            void draw(DrawContext& dc) override;
            void xupdateMaterial(const STDMaterial& mat) override;

            Symbol* getSymbol(int& pos) override;
        };

        class DivNode;
        using spDivNode = intrusive_ptr<DivNode>;

        class DivNode: public Node
        {
        public:
            DivNode(QDomElement& reader);

            void resize(Aligner& rd) override;
            void draw(DrawContext& dc) override;

        public:
            QColor m_color;
            quint32 m_options;
        };

        class BrNode;
        using spBrNode = intrusive_ptr<BrNode>;

        class BrNode: public Node
        {
        public:
            void xresize(Aligner& rd)
            {
                rd.nextLine();
            }
        };
    }
}
