#pragma  once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"

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
            explicit TextBuilder() = default;
            virtual ~TextBuilder() = default;
            text::spNode parse(QString str);
        private:
            text::spNode create(QDomNode& reader);
        };
    }
}
