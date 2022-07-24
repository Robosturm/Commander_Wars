#include "3rd_party/oxygine-framework/oxygine/text_utils/TextBuilder.h"
#include "3rd_party/oxygine-framework/oxygine/text_utils/Node.h"
#include "coreengine/globalutils.h"
#include "coreengine/console.h"

#include <QDomDocument>

namespace oxygine
{
    namespace text
    {

        void TextBuilder::ReplaceHtmlSigns(QString & text) const
        {
            text.replace(AND_AMP_XML_SIGN, "&");
            text.replace(SMALLER_AMP_XML_SIGN, "<");
            text.replace(GREATER_AMP_XML_SIGN, ">");
            text.replace(QOUT_AMP_XML_SIGN, "\"");
            text.replace(APOS_AMP_XML_SIGN, "'");
        }

        bool TextBuilder::FixSign(QString & text, qint32 pos) const
        {
            auto sign = text[pos];
            bool fixed = true;
            if (sign == '&')
            {
                text.replace(pos, 1, AND_AMP_XML_SIGN);
            }
            else if (sign == '<')
            {
                text.replace(pos, 1, SMALLER_AMP_XML_SIGN);
            }
            else if (sign == '>')
            {
                text.replace(pos, 1, GREATER_AMP_XML_SIGN);
            }
            else if (sign == '\"')
            {
                text.replace(pos, 1, QOUT_AMP_XML_SIGN);
            }
            else if (sign == '\'')
            {
                text.replace(pos, 1, APOS_AMP_XML_SIGN);
            }
            else
            {
                fixed = false;
            }
            return fixed;
        }

        text::spNode TextBuilder::create(QDomNode& reader)
        {
            QDomElement element = reader.toElement();
            if (!element.isNull())
            {
                text::spNode tn = text::spNode();
                QString name = element.tagName();
                if (name == "div")
                {
                    tn = text::spDivNode::create(element);
                    QString text = element.text();
                    ReplaceHtmlSigns(text);
                    tn->appendNode(text::spTextNode::create(text));
                }
                else if (name == "br")
                {
                    tn = text::spBrNode::create();
                    QString text = element.text();
                    ReplaceHtmlSigns(text);
                    tn->appendNode(text::spTextNode::create(text));
                }
                else if (name == "r")
                {
                    QString text = element.text();
                    ReplaceHtmlSigns(text);
                    tn = text::spTextNode::create(text);
                }
                else if (name == "data")
                {
                    tn = text::spNode::create();
                }
                else
                {
                    return text::spNode();
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

        text::spNode TextBuilder::parse(const QString & st)
        {
            QString str = st;
            if (st.startsWith("<"))
            {
                str = "<data>" + str + "</data>";
            }
            else
            {
                str = "<data><r>" + str + "</r></data>";
            }
            QDomDocument doc;
            qint32 line;
            qint32 column;
            qint32 lastFixPos = -1;
            QString msg;
            bool fixed = true;

            while (!doc.setContent(str, &msg, &line, &column) && fixed)
            {
                if (lastFixPos != column)
                {
                    fixed = FixSign(str, column - 2);
                    if (!fixed)
                    {
                        fixed = FixSign(str, column - 1);
                    }
                }
                else
                {
                    doc = QDomDocument();
                    CONSOLE_PRINT("Unable to parse xml-text-input for: " + st + " Please use correct xml syntax input.", Console::eERROR);
                    break;
                }
                lastFixPos = column;
            }
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
