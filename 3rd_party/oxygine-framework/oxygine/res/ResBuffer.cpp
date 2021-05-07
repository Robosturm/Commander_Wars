#include "3rd_party/oxygine-framework/oxygine/res/ResBuffer.h"
#include "3rd_party/oxygine-framework/oxygine/res/CreateResourceContext.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"
#include <qtextstream.h>

namespace oxygine
{
    spResource ResBuffer::create(CreateResourceContext& context)
    {
        spResBuffer rs = spResBuffer::create();
        QDomElement node = context.m_walker.getNode();
        QString file = node.attribute("file");

        rs->setName(Resource::extractID(node, file, ""));
        rs->init(context.m_walker.getPath("file"));
        setNode(rs, node);

        context.m_resources->add(rs);

        return rs;
    }


    ResBuffer::ResBuffer()
    {

    }

    ResBuffer::~ResBuffer()
    {

    }

    void ResBuffer::init(QString file)
    {
        m_path = file;
    }

    void ResBuffer::_load(LoadResourcesContext*)
    {
        QFile file(m_path);
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        // copy data to buffer
        m_buffer = stream.readAll();
    }

    void ResBuffer::_unload()
    {
        m_buffer.clear();
    }
}
