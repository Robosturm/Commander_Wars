#pragma  once
#include "3rd_party/oxygine-framework/oxygine-include.h"

class QDomNode;

namespace oxygine
{
    namespace text
    {
        class Node;
        using spNode = intrusive_ptr<Node>;

        class TextBuilder
        {
        public:
            TextBuilder();
            virtual ~TextBuilder();

            text::spNode parse(QString str);

        private:
            text::spNode  create(QDomNode& reader);
        };
    }
}
