#include "3rd_party/oxygine-framework/oxygine/res/ResBuffer.h"
#include "3rd_party/oxygine-framework/oxygine/res/CreateResourceContext.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"
#include <qtextstream.h>

namespace oxygine
{
    Resource* ResBuffer::create(CreateResourceContext& context)
    {
        ResBuffer* rs = new ResBuffer();
        QDomElement node = context.walker.getNode();
        QString file = node.attribute("file");

        rs->setName(Resource::extractID(node, file, ""));
        rs->init(context.walker.getPath("file"));
        setNode(rs, node);

        context.resources->add(rs);

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
        _path = file;
    }

    void ResBuffer::_load(LoadResourcesContext*)
    {
        QFile file(_path);
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        // copy data to buffer
        _buffer = stream.readAll();
    }

    void ResBuffer::_unload()
    {
        _buffer.clear();
    }
}
