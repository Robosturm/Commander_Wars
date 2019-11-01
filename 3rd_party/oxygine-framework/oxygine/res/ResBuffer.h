#pragma once
#include "../oxygine-include.h"
#include "Resource.h"
#include <vector>
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

        const QString& getBuffer() const {return _buffer;}

    private:
        void _load(LoadResourcesContext*) override;
        void _unload() override;

        QString _buffer;
        QString _path;
    };
}
