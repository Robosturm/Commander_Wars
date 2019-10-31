#include "TextBuilder.h"
#include "Node.h"
#include <qxmlstream.h>
#include "../utils/stringUtils.h"

namespace oxygine
{
    namespace text
    {
        TextBuilder::TextBuilder()
        {

        }

        TextBuilder::~TextBuilder()
        {

        }

        text::Node* TextBuilder::create(QXmlStreamReader& reader)
        {
            text::Node* tn = nullptr;
            if (reader.hasError())
            {
                Q_ASSERT(!"can't parse tagged text");
            }
            QString name = reader.name().toString();
            if (name == "div")
            {
                tn = new text::DivNode(reader);
                QString text = reader.readElementText();
                tn->appendNode(new text::TextNode(text));
                return tn;
            }
            else if (name == "br")
            {
                tn = new text::BrNode();
                QString text = reader.readElementText();
                tn->appendNode(new text::TextNode(text));
                return tn;
            }
            else if (name == "r")
            {
                QString text = reader.readElementText();
                tn = new text::TextNode(text);
            }
            else if (name == "data")
            {
                tn = new text::Node;
            }
            else
            {
                return nullptr;
            }
            while (!reader.atEnd() &&
                   !reader.hasError())
            {
                reader.readNext();
                text::Node* tnchild = create(reader);
                if (tnchild != nullptr)
                {
                    tn->appendNode(tnchild);
                }
            }
            return tn;
        }

        text::Node* TextBuilder::parse(const QString& st)
        {
            QString str;
            if (st.startsWith("<"))
            {
                str = "<data>" + st + "</data>";
            }
            else
            {
                str = "<r>" + st + "</r>";
            }
            QXmlStreamReader reader;
            reader.addData(str);
            reader.readNextStartElement();
            return create(reader);
        }
    }
}
