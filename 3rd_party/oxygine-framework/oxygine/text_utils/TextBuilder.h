#pragma  once

#include <QString>

class QDomNode;

namespace oxygine
{
    namespace text
    {
        class Node;
    using spNode = std::shared_ptr<Node>;

        static const char* const AND_AMP_XML_SIGN = "&amp;";
        static const char* const SMALLER_AMP_XML_SIGN = "&lt;";
        static const char* const GREATER_AMP_XML_SIGN = "&gt;";
        static const char* const QOUT_AMP_XML_SIGN = "&quot;";
        static const char* const APOS_AMP_XML_SIGN = "&apos;";

        class TextBuilder final
        {
        public:
            explicit TextBuilder() = default;
           virtual ~TextBuilder() = default;
            text::spNode parse(const QString & str);
        private:
            void ReplaceXmlSignsToSigns(QString & text) const;
            QString & ReplaceSignsToXmlSigns(QString & text) const;
            void FixSigns(QString & text) const;
            text::spNode create(QDomNode& reader);
        };
    }
}
