#include "3rd_party/oxygine-framework/oxygine/text_utils/TextBuilder.h"
#include "3rd_party/oxygine-framework/oxygine/text_utils/Node.h"

#include <QDomDocument>

namespace oxygine
{
    namespace text
    {

        void TextBuilder::ReplaceXmlSignsToSigns(QString & text) const
        {
            text.replace(AND_AMP_XML_SIGN, "&");
            text.replace(SMALLER_AMP_XML_SIGN, "<");
            text.replace(GREATER_AMP_XML_SIGN, ">");
            text.replace(QOUT_AMP_XML_SIGN, "\"");
            text.replace(APOS_AMP_XML_SIGN, "'");
        }

        QString & TextBuilder::ReplaceSignsToXmlSigns(QString & text) const
        {
            text.replace("&", AND_AMP_XML_SIGN);
            text.replace("<", SMALLER_AMP_XML_SIGN);
            text.replace(">", GREATER_AMP_XML_SIGN);
            text.replace("\"", QOUT_AMP_XML_SIGN);
            text.replace("'", APOS_AMP_XML_SIGN);
            return text;
        }

        void TextBuilder::FixSigns(QString & text) const
        {
            if (text.contains("</br>"))
            {
                QStringList items = text.split("</br>");
                text = "";
                for (qint32 i = 0; i < items.size() - 1; i++)
                {
                    QStringList subParts = items[i].split("<br>");
                    if (subParts.length() == 2)
                    {
                        text += subParts[0];
                        text += "<br>";
                        text += ReplaceSignsToXmlSigns(subParts[1]);
                        text += "</br>";
                    }
                    else
                    {
                        text += items[i] + "</br>";
                    }
                }
                text += items[items.size() - 1];
            }
            if (text.contains("</r>"))
            {
                QStringList items = text.split("</r>");
                text = "";
                for (qint32 i = 0; i < items.size() - 1; ++i)
                {
                    QStringList subParts = items[i].split("<r>");
                    if (subParts.length() == 2)
                    {
                        text += subParts[0];
                        text += "<r>";
                        text += ReplaceSignsToXmlSigns(subParts[1]);
                        text += "</r>";
                    }
                    else
                    {
                        text += items[i] + "</r>";
                    }
                }
                text += items[items.size() - 1];
            }
            if (text.contains("</div>"))
            {
                QStringList items = text.split("</div>");
                text = "";
                for (qint32 i = 0; i < items.size() - 1; ++i)
                {
                    QStringList subParts = items[i].split("<div");
                    if (subParts.length() == 2)
                    {
                        text += subParts[0];
                        text += "<div";
                        auto end = subParts[1].indexOf('>');
                        text += subParts[1].section('>', 0, 0) + ">";
                        subParts[1].remove(0, end + 1);
                        text += ReplaceSignsToXmlSigns(subParts[1]);
                        text += "</div>";
                    }
                    else
                    {
                        text += items[i] + "</r>";
                    }
                }
                text += items[items.size() - 1];
            }
            if (text.contains("</wiggly>"))
            {
                QStringList items = text.split("</wiggly>");
                text = "";
                for (qint32 i = 0; i < items.size() - 1; ++i)
                {
                    QStringList subParts = items[i].split("<wiggly");
                    if (subParts.length() == 2)
                    {
                        text += subParts[0];
                        text += "<wiggly";
                        auto end = subParts[1].indexOf('>');
                        text += subParts[1].section('>', 0, 0) + ">";
                        subParts[1].remove(0, end + 1);
                        text += ReplaceSignsToXmlSigns(subParts[1]);
                        text += "</wiggly>";
                    }
                    else
                    {
                        text += items[i] + "</r>";
                    }
                }
                text += items[items.size() - 1];
            }
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
                    ReplaceXmlSignsToSigns(text);
                    tn->appendNode(text::spTextNode::create(text));
                }
                else if (name == "br")
                {
                    tn = text::spBrNode::create();
                    QString text = element.text();
                    ReplaceXmlSignsToSigns(text);
                    tn->appendNode(text::spTextNode::create(text));
                }
                else if (name == "r")
                {
                    QString text = element.text();
                    ReplaceXmlSignsToSigns(text);
                    tn = text::spTextNode::create(text);
                }
                else if (name == "wiggly")
                {
                    QString text = element.text();
                    ReplaceXmlSignsToSigns(text);
                    tn = text::spWigglyNode::create(element, text);
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
            FixSigns(str);
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
