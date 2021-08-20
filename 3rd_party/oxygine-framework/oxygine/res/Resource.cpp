#include "3rd_party/oxygine-framework/oxygine/res/Resource.h"
#include "3rd_party/oxygine-framework/oxygine/res/CreateResourceContext.h"
#include "qfileinfo.h"

namespace oxygine
{
    const char* const Resource::QRC_PREFIX = "qrc";
    const char* const Resource::RCC_PREFIX_PATH = ":/";
    void Resource::load()
    {
        _load();
    }

    void Resource::unload()
    {
        _unload();
    }

    QString Resource::extractID(const QDomElement& node, QString file, QString def)
    {
        QString id = node.attribute("id");
        if (id.isEmpty())
        {
            if (file.isEmpty())
            {
                return def.toLower();
            }
            return extractID(file);
        }

        return id.toLower();
    }

    QString Resource::extractID(QString file)
    {
        QFileInfo info(file);
        return info.baseName().toLower();
    }


    QString Resource::getAttribute(QString attr) const
    {
        return m_node.attribute(attr);
    }

    QString Resource::getName() const
    {
        return m_name;
    }

    void Resource::setName(QString name)
    {
        m_name = name;
    }
}
