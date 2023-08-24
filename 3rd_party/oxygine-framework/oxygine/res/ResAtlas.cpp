#include <QVariant>

#include "3rd_party/oxygine-framework/oxygine/res/ResAtlas.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAtlasGeneric.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"

#include "coreengine/gameconsole.h"

namespace oxygine
{
    void ResAtlas::init_resAnim(spResAnim & rs, const QString & file, const QDomElement & node)
    {
        QString resName = Resource::extractID(node, file, "");
        CONSOLE_PRINT("Registering ressource: " + resName, GameConsole::eDEBUG);
        rs->setName(resName);
        setNode(rs, node);
    }

    spResource ResAtlas::create(CreateResourceContext& context)
    {
        context.m_walker.checkSetAttributes();
        spResAtlas ra;
        spResAtlasGeneric rs = MemoryManagement::create<ResAtlasGeneric>();
        rs->loadAtlas(context);
        ra = rs;

        ra->setName(Resource::extractID(context.m_walker.getNode(), "", QString("!atlas:") + context.m_xml_name));
        context.m_resources->add(ra);
        setNode(ra, context.m_walker.getNode());

        return ra;
    }

    void ResAtlas::loadBase(QDomElement node)
    {
        QVariant value = QVariant(node.attribute("linearFilter"));
        if (value.typeId() == QMetaType::QString &&
            !value.isNull() &&
            node.hasAttribute("linearFilter"))
        {
            if (value.toBool())
            {
                m_linearFilter = GL_LINEAR;
            }
            else
            {
                m_linearFilter = GL_NEAREST;
            }
        }
        else
        {
            m_linearFilter = GL_NEAREST;
        }
        value = QVariant(node.attribute("clamp2edge"));
        if (value.typeId() == QMetaType::QString &&
            !value.isNull() &&
            node.hasAttribute("clamp2edge"))
        {
            m_clamp2edge = value.toBool();
        }
        else
        {
            m_clamp2edge = true;
        }
    }

    spResAnim ResAtlas::createEmpty(const XmlWalker& walker, CreateResourceContext& context)
    {
        spResAnim ra = MemoryManagement::create<ResAnim>(this);
        ra->init(nullptr, 0, 0, walker.getScaleFactor());
        init_resAnim(ra, "", walker.getNode());
        ra->setParent(this);
        context.m_resources->add(ra);

        return ra;
    }
}
