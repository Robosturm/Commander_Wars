#include "ResBuffer.h"
#include "CreateResourceContext.h"
#include "Resources.h"
#include "qtextstream.h"

namespace oxygine
{
    Resource* ResBuffer::create(CreateResourceContext& context)
    {
        ResBuffer* rs = new ResBuffer();
        pugi::xml_node node = context.walker.getNode();
        std::string file = node.attribute("file").value();

        rs->setName(Resource::extractID(node, file, ""));
        rs->init(context.walker.getPath("file").c_str());
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

    void ResBuffer::init(const char* file)
    {
        _path = file;
    }

    void ResBuffer::_load(LoadResourcesContext*)
    {
        QFile file(_path.c_str());
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        // copy data to buffer
        std::string data = stream.readAll().toStdString();
        _buffer = std::vector<uchar>(data.begin(), data.end());
    }

    void ResBuffer::_unload()
    {
        _buffer.clear();
    }
}
