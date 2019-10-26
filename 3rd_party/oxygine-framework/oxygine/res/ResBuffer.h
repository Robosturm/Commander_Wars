#pragma once
#include "../oxygine-include.h"
#include "Resource.h"
#include <vector>
#include "qfile.h"

namespace oxygine
{
    class ResBuffer: public Resource
    {
        INHERITED(Resource);
    public:
        static Resource* create(CreateResourceContext&);

        ResBuffer();
        ~ResBuffer();

        void init(const char* file);

        const std::vector<uchar>& getBuffer() const {return _buffer;}

    private:
        void _load(LoadResourcesContext*) override;
        void _unload() override;

        std::vector<uchar> _buffer;
        std::string _path;
    };
}
