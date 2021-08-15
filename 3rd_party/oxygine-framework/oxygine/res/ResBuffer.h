#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resource.h"
#include "qfile.h"

namespace oxygine
{
    class ResBuffer;
    using spResBuffer = intrusive_ptr<ResBuffer>;
    class ResBuffer : public Resource
    {
    public:
        static spResource create(CreateResourceContext&);

        explicit ResBuffer() = default;
        virtual ~ResBuffer() = default;

        void init(QString file);

        QString getBuffer() const
        {
            return m_buffer;
        }

    private:
        void _load(LoadResourcesContext*) override;
        void _unload() override;

    private:
        QString m_buffer;
        QString m_path;
    };
}
