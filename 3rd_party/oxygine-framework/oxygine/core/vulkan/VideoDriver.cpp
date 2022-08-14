#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "3rd_party/oxygine-framework/oxygine/core/vulkan/vulkanrenderer.h"

namespace oxygine
{
    spVideoDriver VideoDriver::instance;
    VideoDriver::Stats VideoDriver::m_stats;
    VertexDeclaration VideoDriver::m_VertexDeclaration;

    void VideoDriver::setUniform(const char* id, const QMatrix4x4& mat)
    {
    }

    void VideoDriver::setUniform(const char* id, const Vector2& v)
    {
        setUniform(id, &v, 1);
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
        m_rt = spTexture::create();
    }

    VideoDriver::~VideoDriver()
    {
    }

    quint32 VideoDriver::getPT(VideoDriver::PRIMITIVE_TYPE pt)
    {
        return 0;
    }

    quint32 VideoDriver::getBT(VideoDriver::BLEND_TYPE pt)
    {
        return 0;
    }

    spTexture VideoDriver::getRenderTarget() const
    {
        return m_rt;
    }

    const VertexDeclaration* VideoDriver::getVertexDeclaration() const
    {
        return &m_VertexDeclaration;
    }

    void VideoDriver::getViewport(Rect& r) const
    {
    }

    void VideoDriver::setScissorRect(const Rect* rect)
    {

    }

    bool VideoDriver::getScissorRect(Rect& r) const
    {
        return false;
    }

    void VideoDriver::setRenderTarget(spTexture & rt)
    {
        m_rt = rt;
    }

    void VideoDriver::_begin(const Rect& viewport, const QColor* clearColor)
    {

        if (clearColor)
        {
            clear(*clearColor);
        }
    }

    void VideoDriver::setBlendFunc(BLEND_TYPE src, BLEND_TYPE dest)
    {
    }

    void VideoDriver::setState(STATE state, quint32 value)
    {
    }

    void VideoDriver::restore()
    {
        setDefaultSettings();
    }

    bool VideoDriver::isReady() const
    {
        return true;
    }

    spTexture VideoDriver::createTexture()
    {
        return spTexture::create();
    }

    void VideoDriver::reset()
    {
    }

    void VideoDriver::begin(const Rect& viewport, const QColor* clearColor)
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

    void VideoDriver::setViewport(const Rect& viewport)
    {
    }

    void VideoDriver::setShaderProgram(ShaderProgram* prog_)
    {
        prog_->bind();
        m_programID = prog_->getID();
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
    }

    void VideoDriver::setUniformInt(const char* id, qint32 v)
    {
    }

    void VideoDriver::setUniform(const char* id, const Uniform4f* v, qint32 num)
    {
    }

    void VideoDriver::setUniform(const char* id, const Vector2* v, qint32 num)
    {
    }

    void VideoDriver::setUniform(const char* id, const Uniform3f* v, qint32 num)
    {
    }

    void VideoDriver::setUniform(const char* id, float val)
    {
    }
}
