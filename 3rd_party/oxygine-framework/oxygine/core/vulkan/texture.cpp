#include "3rd_party/oxygine-framework/oxygine/core/vulkan/texture.h"

#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "3rd_party/oxygine-framework/oxygine/core/vulkan/vulkanrenderer.h"
#include "3rd_party/oxygine-framework/oxygine/core/vulkan/windowBase.h"

#include <QDebug>
#include <cstring>
#include <functional>

namespace oxygine
{
    quint32 Texture::m_highestTextureCount = 0;

    namespace
    {
        uint32_t findMemoryType(QVulkanWindow* window, uint32_t typeBits, VkMemoryPropertyFlags properties)
        {
            VkPhysicalDeviceMemoryProperties memoryProperties{};
            window->vulkanInstance()->functions()->vkGetPhysicalDeviceMemoryProperties(
                window->physicalDevice(), &memoryProperties);
            for (uint32_t index = 0; index < memoryProperties.memoryTypeCount; ++index)
            {
                if ((typeBits & (1u << index)) != 0 &&
                    (memoryProperties.memoryTypes[index].propertyFlags & properties) == properties)
                {
                    return index;
                }
            }
            return UINT32_MAX;
        }

        bool submitUpload(QVulkanWindow* window, const std::function<void(VkCommandBuffer)>& record)
        {
            auto* funcs = VulkanRenderer::getDeviceFunctions();
            VkCommandBufferAllocateInfo allocation{};
            allocation.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocation.commandPool = window->graphicsCommandPool();
            allocation.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocation.commandBufferCount = 1;
            VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
            if (funcs->vkAllocateCommandBuffers(window->device(), &allocation, &commandBuffer) != VK_SUCCESS)
            {
                return false;
            }
            VkCommandBufferBeginInfo begin{};
            begin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            begin.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            if (funcs->vkBeginCommandBuffer(commandBuffer, &begin) != VK_SUCCESS)
            {
                funcs->vkFreeCommandBuffers(window->device(), window->graphicsCommandPool(), 1, &commandBuffer);
                return false;
            }
            record(commandBuffer);
            funcs->vkEndCommandBuffer(commandBuffer);
            VkSubmitInfo submit{};
            submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submit.commandBufferCount = 1;
            submit.pCommandBuffers = &commandBuffer;
            VkResult result = funcs->vkQueueSubmit(window->graphicsQueue(), 1, &submit, VK_NULL_HANDLE);
            if (result == VK_SUCCESS)
            {
                result = funcs->vkQueueWaitIdle(window->graphicsQueue());
            }
            funcs->vkFreeCommandBuffers(window->device(), window->graphicsCommandPool(), 1, &commandBuffer);
            return result == VK_SUCCESS;
        }
    }

    Texture::~Texture()
    {
        Texture::release();
    }

    void Texture::setLinearFilter(quint32 filter)
    {
        if (m_sampler == VK_NULL_HANDLE)
        {
            return;
        }
        auto* window = static_cast<WindowBase*>(GameWindow::getWindow());
        auto* funcs = VulkanRenderer::getDeviceFunctions();
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = filter == 0 ? VK_FILTER_NEAREST : VK_FILTER_LINEAR;
        samplerInfo.minFilter = samplerInfo.magFilter;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.maxLod = 1.0f;
        funcs->vkDestroySampler(window->device(), m_sampler, nullptr);
        funcs->vkCreateSampler(window->device(), &samplerInfo, nullptr, &m_sampler);
    }

    void Texture::setClamp2Edge(bool clamp2edge)
    {
        if (m_sampler == VK_NULL_HANDLE)
        {
            return;
        }
        auto* window = static_cast<WindowBase*>(GameWindow::getWindow());
        auto* funcs = VulkanRenderer::getDeviceFunctions();
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_NEAREST;
        samplerInfo.minFilter = VK_FILTER_NEAREST;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
        samplerInfo.addressModeU = clamp2edge ? VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE : VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = samplerInfo.addressModeU;
        samplerInfo.addressModeW = samplerInfo.addressModeU;
        samplerInfo.maxLod = 1.0f;
        funcs->vkDestroySampler(window->device(), m_sampler, nullptr);
        funcs->vkCreateSampler(window->device(), &samplerInfo, nullptr, &m_sampler);
    }

    quint32 Texture::getHighestTextureCount()
    {
        return m_highestTextureCount;
    }

    void Texture::release()
    {
        auto* window = GameWindow::getWindow();
        auto* funcs = VulkanRenderer::getDeviceFunctions();
        if (window == nullptr || funcs == nullptr)
        {
            return;
        }
        if (m_sampler != VK_NULL_HANDLE) funcs->vkDestroySampler(window->device(), m_sampler, nullptr);
        if (m_view != VK_NULL_HANDLE) funcs->vkDestroyImageView(window->device(), m_view, nullptr);
        if (m_imageHandle != VK_NULL_HANDLE) funcs->vkDestroyImage(window->device(), m_imageHandle, nullptr);
        if (m_memory != VK_NULL_HANDLE) funcs->vkFreeMemory(window->device(), m_memory, nullptr);
        m_sampler = VK_NULL_HANDLE;
        m_view = VK_NULL_HANDLE;
        m_imageHandle = VK_NULL_HANDLE;
        m_memory = VK_NULL_HANDLE;
    }

    void Texture::init(const QImage & image)
    {
        release();
        auto* window = static_cast<WindowBase*>(GameWindow::getWindow());
        auto* funcs = VulkanRenderer::getDeviceFunctions();
        if (window == nullptr || funcs == nullptr || image.isNull())
        {
            return;
        }
        m_image = image.convertToFormat(QImage::Format_RGBA8888);
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
        imageInfo.extent = { static_cast<uint32_t>(m_image.width()), static_cast<uint32_t>(m_image.height()), 1 };
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        if (funcs->vkCreateImage(window->device(), &imageInfo, nullptr, &m_imageHandle) != VK_SUCCESS)
        {
            return;
        }
        VkMemoryRequirements requirements{};
        funcs->vkGetImageMemoryRequirements(window->device(), m_imageHandle, &requirements);
        VkMemoryAllocateInfo allocation{};
        allocation.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocation.allocationSize = requirements.size;
        allocation.memoryTypeIndex = findMemoryType(window, requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        if (allocation.memoryTypeIndex == UINT32_MAX || funcs->vkAllocateMemory(window->device(), &allocation, nullptr, &m_memory) != VK_SUCCESS)
        {
            release();
            return;
        }
        funcs->vkBindImageMemory(window->device(), m_imageHandle, m_memory, 0);

        VkBuffer stagingBuffer = VK_NULL_HANDLE;
        VkDeviceMemory stagingMemory = VK_NULL_HANDLE;
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = static_cast<VkDeviceSize>(m_image.sizeInBytes());
        bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        funcs->vkCreateBuffer(window->device(), &bufferInfo, nullptr, &stagingBuffer);
        VkMemoryRequirements bufferRequirements{};
        funcs->vkGetBufferMemoryRequirements(window->device(), stagingBuffer, &bufferRequirements);
        allocation.allocationSize = bufferRequirements.size;
        allocation.memoryTypeIndex = findMemoryType(window, bufferRequirements.memoryTypeBits,
                                                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        funcs->vkAllocateMemory(window->device(), &allocation, nullptr, &stagingMemory);
        funcs->vkBindBufferMemory(window->device(), stagingBuffer, stagingMemory, 0);
        void* mapped = nullptr;
        funcs->vkMapMemory(window->device(), stagingMemory, 0, bufferInfo.size, 0, &mapped);
        std::memcpy(mapped, m_image.constBits(), static_cast<size_t>(bufferInfo.size));
        funcs->vkUnmapMemory(window->device(), stagingMemory);

        submitUpload(window, [&](VkCommandBuffer commandBuffer)
        {
            VkImageMemoryBarrier barrier{};
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.image = m_imageHandle;
            barrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
            funcs->vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                                        VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
            VkBufferImageCopy copy{};
            copy.imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
            copy.imageExtent = imageInfo.extent;
            funcs->vkCmdCopyBufferToImage(commandBuffer, stagingBuffer, m_imageHandle,
                                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);
            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            funcs->vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
        });
        funcs->vkDestroyBuffer(window->device(), stagingBuffer, nullptr);
        funcs->vkFreeMemory(window->device(), stagingMemory, nullptr);
        m_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        ++m_highestTextureCount;

        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = m_imageHandle;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = imageInfo.format;
        viewInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
        funcs->vkCreateImageView(window->device(), &viewInfo, nullptr, &m_view);
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_NEAREST;
        samplerInfo.minFilter = VK_FILTER_NEAREST;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.maxLod = 1.0f;
        funcs->vkCreateSampler(window->device(), &samplerInfo, nullptr, &m_sampler);
    }
}
