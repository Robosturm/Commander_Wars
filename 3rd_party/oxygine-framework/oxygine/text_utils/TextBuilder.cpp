#include "3rd_party/oxygine-framework/oxygine/text_utils/TextBuilder.h"
#include "3rd_party/oxygine-framework/oxygine/text_utils/Node.h"
#include <QDomDocument>

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

        text::Node* TextBuilder::create(QDomNode& reader)
        {
            QDomElement element = reader.toElement();
            if (!element.isNull())
            {
                text::Node* tn = nullptr;
                QString name = element.tagName();
                if (name == "div")
                {
                    tn = new text::DivNode(element);
                    QString text = element.text();
                    tn->appendNode(new text::TextNode(text));
                }
                else if (name == "br")
                {
                    tn = new text::BrNode();
                    QString text = element.text();
                    tn->appendNode(new text::TextNode(text));
                }
                else if (name == "r")
                {
                    QString text = element.text();
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
                if (reader.hasChildNodes())
                {

                }
                // loop through current node childs
                QDomNode child = reader.firstChild();
                while(!child.isNull())
                {
                    text::Node* tnchild = create(child);
                    if (tnchild != nullptr)
                    {
                        tn->appendNode(tnchild);
                    }
                    child = child.nextSibling();
                }
                return tn;
            }
            else
            {
                return new text::Node;
            }
        }

        text::Node* TextBuilder::parse(QString st)
        {
            QString str;
            if (st.startsWith("<"))
            {
                str = "<data>" + st + "</data>";
            }
            else
            {
                str = "<data><r>" + st + "</r></data>";
            }
            QDomDocument doc;
            doc.setContent(str);
            QDomElement root = doc.documentElement();
            QDomNode node = root.firstChild();
            // loop through root childs
            auto tn = new text::Node;
            while(!node.isNull())
            {
                text::Node* tnchild = create(node);
                if (tnchild != nullptr)
                {
                    tn->appendNode(tnchild);
                }
                node = node.nextSibling();
            }
            return tn;
        }
    }
}
