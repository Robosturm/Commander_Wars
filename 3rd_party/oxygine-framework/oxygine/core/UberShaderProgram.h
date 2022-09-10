#pragma once
#include "ShaderProgram.h"
#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"
#include <QString>

namespace oxygine
{
    class UberShaderProgram final
    {
    public:
        enum ColorMode
        {
            COLOR_DEFAULT,
            COLOR_TABLE,
            COLOR_MATRIX,
            _SIZE,
        };
        enum
        {
            SAMPLER_BASE,
            SAMPLER_TABLE,
            SAMPLER_MASK,
            SAMPLER_NUM,
        };
        struct shader
        {
            spShaderProgram program;
            ColorMode fracShader{COLOR_DEFAULT};
        };
        explicit UberShaderProgram() = default;
        ~UberShaderProgram();
        ShaderProgram* getShaderProgram(ColorMode fracShader);
        void apply(VideoDriver* driver, spTexture & base);
        void init();
        void release();
    protected:
        void releaseShaders();
    protected:
        QString m_fracShader;
        QString m_vertexShader;
        QString m_fracTableShader;
        QString m_fracMatrixShader;
        shader m_shaders[_SIZE];
    };
}
