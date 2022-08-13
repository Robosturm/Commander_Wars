#include "ShaderProgram.h"

#include "3rd_party/oxygine-framework/oxygine/core/VertexDeclaration.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

namespace oxygine
{
    ShaderProgram::ShaderProgram()
    {
    }

    quint32 ShaderProgram::getID() const
    {
        return 0;
    }

    void ShaderProgram::bind()
    {
    }

    qint32 ShaderProgram::getUniformLocation(const char* id) const
    {
        return 0;
    }
}
