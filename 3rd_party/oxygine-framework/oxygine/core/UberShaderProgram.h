#pragma once
#include "3rd_party/oxygine-framework/oxygine/core/Object.h"
#include "3rd_party/oxygine-framework/oxygine/core/ShaderProgram.h"

namespace oxygine
{
    class UberShaderProgram
    {
    public:
        struct shader
        {
            spShaderProgram program;
            qint32 flags{0};
        };
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
        explicit UberShaderProgram() = default;
        virtual ~UberShaderProgram();
        ShaderProgram* getShaderProgram(qint32 flags);
        void apply(VideoDriver* driver, spTexture base, spTexture alpha);
        void init(const QString& fracShader, const QString& vertexShader, const QString& fracTableShader);
        void release();
    protected:
        void releaseShaders();
    protected:
        QString m_fracShader;
        QString m_vertexShader;
        QString m_fracTableShader;
        shader m_shaders[_SIZE];
    };
}
