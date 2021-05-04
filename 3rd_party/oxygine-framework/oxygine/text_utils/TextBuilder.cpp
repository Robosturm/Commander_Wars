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

        text::spNode TextBuilder::create(QDomNode& reader)
        {
            QDomElement element = reader.toElement();
            if (!element.isNull())
            {
                text::spNode tn = nullptr;
                QString name = element.tagName();
                if (name == "div")
                {
                    tn = text::spDivNode::create(element);
                    QString text = element.text();
                    tn->appendNode(text::spTextNode::create(text));
                }
                else if (name == "br")
                {
                    tn = text::spBrNode::create();
                    QString text = element.text();
                    tn->appendNode(text::spTextNode::create(text));
                }
                else if (name == "r")
                {
                    QString text = element.text();
                    tn = text::spTextNode::create(text);
                }
                else if (name == "data")
                {
                    tn = text::spNode::create();
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
                    text::spNode tnchild = create(child);
                    if (tnchild.get() != nullptr)
                    {
                        tn->appendNode(tnchild);
                    }
                    child = child.nextSibling();
                }
                return tn;
            }
            else
            {
                return text::spNode::create();
            }
        }

        text::spNode TextBuilder::parse(QString st)
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
            auto tn = text::spNode::create();
            while(!node.isNull())
            {
                text::spNode tnchild = create(node);
                if (tnchild.get() != nullptr)
                {
                    tn->appendNode(tnchild);
                }
                node = node.nextSibling();
            }
            return tn;
        }
    }
}
