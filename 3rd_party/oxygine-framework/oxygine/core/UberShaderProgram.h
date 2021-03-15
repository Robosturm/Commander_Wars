#pragma once
#include "3rd_party/oxygine-framework/oxygine/core/Object.h"
#include "3rd_party/oxygine-framework/oxygine/core/Restorable.h"
#include "3rd_party/oxygine-framework/oxygine/core/ShaderProgram.h"

namespace oxygine
{

    class UberShaderProgramBase: public Restorable
    {
    public:
        struct shader
        {
            shader()
                : flags(0)
            {
            }
            spShaderProgram program;
            int flags;
        };

        UberShaderProgramBase();
        ~UberShaderProgramBase();

        void init(const QString& fracShader, const QString& vertexShader, const QString& fracTableShader);

        void release();

        virtual ShaderProgram*         getShaderProgram(int flags) = 0;


    protected:
        void* _getRestorableObject() {return this;}
        void _restore(Restorable*, void*);

        QString _fracShader;
        QString _vertexShader;
        QString _fracTableShader;

        virtual void releaseShaders() {}
    };

    class UberShaderProgram : public UberShaderProgramBase
    {
    public:
        UberShaderProgram();
        ~UberShaderProgram();

        enum
        {
            ALPHA_PREMULTIPLY = 1,
            SEPARATE_ALPHA = 1 << 1,
            MASK_R_CHANNEL = 1 << 2,
            MASK = 1 << 3,
            ADD_COLOR = 1 << 4,
            COLOR_TABLE = 1 << 5,
            _SIZE = 1 << 6
        };

        enum
        {
            SAMPLER_BASE,
            SAMPLER_TABLE,
            SAMPLER_ALPHA,
            SAMPLER_MASK,
            SAMPLER_NUM,
        };

        ShaderProgram*  getShaderProgram(int flags) override;

        void apply(IVideoDriver* driver, spNativeTexture base, spNativeTexture alpha);

    protected:
        void releaseShaders() override;
        shader _shaders[_SIZE];
    };
}
