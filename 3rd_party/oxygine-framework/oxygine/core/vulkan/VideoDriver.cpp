#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"

#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "3rd_party/oxygine-framework/oxygine/core/vulkan/vulkanrenderer.h"

#include "coreengine/memorymanagement.h"

#include <cstring>

namespace oxygine
{
    spVideoDriver VideoDriver::instance;

    void VideoDriver::setUniform(const char* id, const QMatrix4x4& mat)
    {
    }

    void VideoDriver::setUniform(const char* id, const Uniform3f& v)
    {
        setUniform(id, &v, 1);
    }

    void VideoDriver::setUniform(const char* id, const Uniform4f& v)
    {
        setUniform(id, &v, 1);
    }

    VideoDriver::VideoDriver()
    {
        m_rt = MemoryManagement::create<Texture>();
    }

    VideoDriver::~VideoDriver()
    {
    }

    quint32 VideoDriver::getPT(VideoDriver::PRIMITIVE_TYPE pt)
    {
        switch (pt)
        {
        case PRIMITIVE_TYPE::POINTS: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
        case PRIMITIVE_TYPE::LINES: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
        case PRIMITIVE_TYPE::LINE_STRIP: return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
        case PRIMITIVE_TYPE::TRIANGLES: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        case PRIMITIVE_TYPE::TRIANGLE_STRIP: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
        case PRIMITIVE_TYPE::TRIANGLE_FAN: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
        case PRIMITIVE_TYPE::LINE_LOOP: return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
        default: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        }
    }

    spTexture VideoDriver::getRenderTarget() const
    {
        return m_rt;
    }

    const VertexDeclaration* VideoDriver::getVertexDeclaration() const
    {
        return &m_VertexDeclaration;
    }

    void VideoDriver::getViewport(QRect& r) const
    {
        r = m_viewport;
    }

    void VideoDriver::setScissorRect(const QRect* rect)
    {
        VkCommandBuffer commandBuffer = GameWindow::getWindow()->currentCommandBuffer();
        if (rect == nullptr)
        {
            m_scissorEnabled = false;
            return;
        }
        m_scissorRect = *rect;
        m_scissorEnabled = true;
        VkRect2D scissor{};
        scissor.offset = { rect->x(), rect->y() };
        scissor.extent = { static_cast<quint32>(qMax(0, rect->width())), static_cast<quint32>(qMax(0, rect->height())) };
        VulkanRenderer::getDeviceFunctions()->vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    QRect VideoDriver::getScissorRect(bool& r) const
    {
        r = m_scissorEnabled;
        return m_scissorRect;
    }

    void VideoDriver::setRenderTarget(spTexture & rt)
    {
        m_rt = rt;
    }

    void VideoDriver::_begin(const QRect& viewport, const QColor* clearColor)
    {
        setViewport(viewport);
        m_scissorEnabled = false;
        if (clearColor)
        {
            clear(*clearColor);
        }
    }

    void VideoDriver::setBlendFunc(BLEND_MODE func)
    {
        m_blendMode = func;
    }

    void VideoDriver::setState(STATE state, bool value)
    {
        if (state == STATE::BLEND)
        {
            m_blendEnabled = value;
        }
    }

    void VideoDriver::restore()
    {
        setDefaultSettings();
    }

    bool VideoDriver::isReady() const
    {
        return GameWindow::getWindow() != nullptr && VulkanRenderer::getDeviceFunctions() != nullptr;
    }

    spTexture VideoDriver::createTexture()
    {
        return spTexture::create();
    }

    void VideoDriver::reset()
    {
    }

    void VideoDriver::begin(const QRect& viewport, const QColor* clearColor)
    {
        _begin(viewport, clearColor);
    }

    void VideoDriver::clear(const QColor& color)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        VkClearColorValue clearColor = {{ color.redF(), color.greenF(), color.blueF(), color.alphaF() }};
        VkClearDepthStencilValue clearDS = { 1.0f, 0 };
        VkClearValue clearValues[2];
        memset(clearValues, 0, sizeof(clearValues));
        clearValues[0].color = clearColor;
        clearValues[1].depthStencil = clearDS;

        VkRenderPassBeginInfo rpBeginInfo;
        memset(&rpBeginInfo, 0, sizeof(rpBeginInfo));
        rpBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rpBeginInfo.renderPass = window->defaultRenderPass();
        rpBeginInfo.framebuffer = window->currentFramebuffer();
        const QSize sz = window->swapChainImageSize();
        rpBeginInfo.renderArea.extent.width = sz.width();
        rpBeginInfo.renderArea.extent.height = sz.height();
        rpBeginInfo.clearValueCount = 2;
        rpBeginInfo.pClearValues = clearValues;
        VkCommandBuffer cmdBuf = window->currentCommandBuffer();
        VulkanRenderer::getDeviceFunctions()->vkCmdBeginRenderPass(cmdBuf, &rpBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void VideoDriver::setViewport(const QRectF& viewport)
    {
        m_viewport = viewport.toRect();
        VkViewport vulkanViewport{};
        vulkanViewport.x = viewport.x();
        vulkanViewport.y = viewport.y();
        vulkanViewport.width = viewport.width();
        vulkanViewport.height = viewport.height();
        vulkanViewport.minDepth = 0.0f;
        vulkanViewport.maxDepth = 1.0f;
        VulkanRenderer::getDeviceFunctions()->vkCmdSetViewport(
            GameWindow::getWindow()->currentCommandBuffer(), 0, 1, &vulkanViewport);
    }

    void VideoDriver::setShaderProgram(ShaderProgram* prog_)
    {
        m_pShaderProgram = prog_;
    }

    void VideoDriver::setTexture(qint32 sampler, spTexture & t)
    {
    }

    void VideoDriver::draw(PRIMITIVE_TYPE pt, const VertexDeclaration* decl, const VertexPCT2* verticesData, qint32 primitives)
    {
    }

    void VideoDriver::draw(PRIMITIVE_TYPE pt, const VertexDeclaration* decl, const VertexPCT2* verticesData, const quint16* indicesData, quint32 numIndices)
    {
    }

    void VideoDriver::setDefaultSettings()
    {
        m_scissorEnabled = false;
        m_blendEnabled = false;
        m_blendMode = BLEND_MODE::NONE;
    }

    void VideoDriver::setUniformInt(const char* id, qint32 v)
    {
    }

    void VideoDriver::setUniform(const char* id, const Uniform4f* v, qint32 num)
    {
    }

    void VideoDriver::setUniform(const char* id, const Uniform3f* v, qint32 num)
    {
    }

    void VideoDriver::setUniform(const char* id, float val)
    {
    }
}
