#pragma  once
#include "../oxygine-include.h"
#include <string>

class QXmlStreamReader;

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

            text::Node* parse(const QString& str);

        private:
            text::Node*  create(QXmlStreamReader& reader);
        };
    }
}
