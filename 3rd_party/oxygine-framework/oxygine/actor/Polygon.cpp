#include "3rd_party/oxygine-framework/oxygine/actor/Polygon.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"
#include "3rd_party/oxygine-framework/oxygine/core/UberShaderProgram.h"
#include "3rd_party/oxygine-framework/oxygine/core/VertexDeclaration.h"

#include <QVector>

namespace oxygine
{
    Polygon::Polygon()
        : m_vdecl(nullptr),
          m_own(false),
          m_verticesData(nullptr),
          m_verticesSize(0)

    {

    }

    Polygon::~Polygon()
    {
        if (m_own)
        {
            delete[] m_verticesData;
        }
    }

    void Polygon::setVertices(const void* data, qint32 size, qint32 bformat, bool own)
    {
        if (m_own && data != m_verticesData)
        {
            delete[] m_verticesData;
        }

        m_own = own;
        m_verticesData = reinterpret_cast<const unsigned char*>(data);
        m_verticesSize = size;
        m_vdecl = IVideoDriver::instance->getVertexDeclaration(bformat);
    }

    template <class T>
    void append(std::vector<unsigned char>& buff, const T& t)
    {
        const unsigned char* ptr = (const unsigned char*)&t;
        buff.insert(buff.end(), ptr, ptr + sizeof(t));
    }

    void Polygon::doRender(const RenderState& rs)
    {
        if (!m_verticesSize)
        {
            return;
        }
        m_mat->apply2([ = ]()
        {

            STDRenderer* renderer = STDRenderer::getCurrent();

            static std::vector<unsigned char> buff;
            buff.clear();

            buff.reserve(m_verticesSize);
            qint32 num = m_verticesSize / m_vdecl->size;

            const unsigned char* ptr = (const unsigned char*)m_verticesData;
            for (qint32 i = 0; i < num; ++i)
            {
                const Vector2* pos = (Vector2*)ptr;
                Vector2 t = rs.transform.transform(*pos);

                append(buff, t);
                buff.insert(buff.end(), ptr + sizeof(t), ptr + sizeof(t) + m_vdecl->size - sizeof(t));

                ptr += m_vdecl->size;
            }

            renderer->addVertices(&buff.front(), (unsigned int)buff.size());

        });
    }
}
