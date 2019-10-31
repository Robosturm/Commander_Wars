#pragma once
#include "../oxygine-include.h"
#include "Aligner.h"
#include "../core/Object.h"
#include "../math/Vector2.h"
#include <string>
#include <vector>

#include <qxmlstream.h>

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
        typedef std::vector<Symbol> text_data;


        class DrawContext
        {
        public:
            DrawContext() {}

            QColor color;
            QColor primary;
        };

        class Node
        {
        public:
            Node();
            virtual ~Node();

            void appendNode(Node* tn);
            virtual void resize(Aligner& rd);
            void finalPass(Aligner& rd);

            void drawChildren(DrawContext& dc);
            void resizeChildren(Aligner& rd);
            virtual Symbol* getSymbol(int& pos);

            virtual void draw(DrawContext& dc);

            virtual void xresize(Aligner& rd) {}
            virtual void xfinalPass(Aligner& rd) {}

            void updateMaterial(const STDMaterial& mat);
            virtual void xupdateMaterial(const STDMaterial& mat) {}


            Node* _firstChild;
            Node* _lastChild;
            Node* _nextSibling;
        };

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

        class DivNode: public Node
        {
        public:
            DivNode(QXmlStreamReader& reader);

            void resize(Aligner& rd) override;
            void draw(DrawContext& dc) override;

            QColor color;
            unsigned int options;
        };

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
