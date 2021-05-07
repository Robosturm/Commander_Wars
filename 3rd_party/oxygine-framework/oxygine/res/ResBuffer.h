#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resource.h"
#include "qfile.h"

namespace oxygine
{
    class ResBuffer: public Resource
    {
    public:
        static spResource create(CreateResourceContext&);

        explicit ResBuffer();
        ~ResBuffer();

        void init(QString file);

        QString getBuffer() const {return m_buffer;}

    private:
        void _load(LoadResourcesContext*) override;
        void _unload() override;

    private:
        QString m_buffer;
        QString m_path;
    };
}
