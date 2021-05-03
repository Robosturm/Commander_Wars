#pragma  once
#include "3rd_party/oxygine-framework/oxygine-include.h"

class QDomNode;

namespace oxygine
{
    namespace text
    {
        class Node;

        class TextBuilder
        {
        public:
            TextBuilder();
            virtual ~TextBuilder();

            text::Node* parse(QString str);

        private:
            text::Node*  create(QDomNode& reader);
        };
    }
}
