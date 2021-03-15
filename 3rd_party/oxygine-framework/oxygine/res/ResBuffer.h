#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resource.h"
#include "qfile.h"

namespace oxygine
{
    class ResBuffer: public Resource
    {
    public:
        static Resource* create(CreateResourceContext&);

        ResBuffer();
        ~ResBuffer();

        void init(QString file);

        QString getBuffer() const {return _buffer;}

    private:
        void _load(LoadResourcesContext*) override;
        void _unload() override;

        QString _buffer;
        QString _path;
    };
}
