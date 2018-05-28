#pragma once

#include "Engine/Engine.hpp"
#include "Display/Display.hpp"
#include "Buffers/CommandBuffer.hpp"
#include "Swapchain/DepthStencil.hpp"
#include "Swapchain/Swapchain.hpp"
#include "RenderStage.hpp"
#include "IManagerRender.hpp"

namespace fl
{
	class FL_EXPORT Renderer :
		public IModule
	{
	private:
		IManagerRender *m_managerRender;

		std::vector<RenderStage *> m_renderStages;

		Swapchain *m_swapchain;
		VkFence m_fenceSwapchainImage;
		uint32_t m_activeSwapchainImage;

		VkPipelineCache m_pipelineCache;

		VkSemaphore m_semaphore;
		VkCommandPool m_commandPool;

		CommandBuffer *m_commandBuffer;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Renderer *Get()
		{
			return reinterpret_cast<Renderer *>(Engine::Get()->GetModule("renderer"));
		}

		/// <summary>
		/// Creates a new renderer module.
		/// </summary>
		Renderer();

		/// <summary>
		/// Deconstructor for the renderer module.
		/// </summary>
		~Renderer();

	public:
		void Update() override;

		void CreateRenderpass(std::vector<RenderpassCreate *> renderpassCreates);

		/// <summary>
		/// Starts a renderpass.
		/// </summary>
		/// <param name="commandBuffer"> The command buffer to use. </param>
		/// <param name="i"> The index of the render pass being rendered. </param>
		/// <returns> VK_SUCCESS on success. </returns>
		bool StartRenderpass(const CommandBuffer &commandBuffer, const unsigned int &i);

		/// <summary>
		/// Ends the renderpass.
		/// </summary>
		/// <param name="commandBuffer"> The command buffer to use. </param>
		void EndRenderpass(const CommandBuffer &commandBuffer, const unsigned int &i);

		/// <summary>
		/// Starts the next render subpass.
		/// </summary>
		/// <param name="commandBuffer"> The command buffer to use. </param>
		void NextSubpass(const CommandBuffer &commandBuffer);

		static uint32_t FindMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties *deviceMemoryProperties, const VkMemoryRequirements *memoryRequirements, const VkMemoryPropertyFlags &requiredProperties);

		/// <summary>
		/// Gets the renderer manager.
		/// </summary>
		/// <returns> The renderer manager. </returns>
		IManagerRender *GetManager() const { return m_managerRender; }

		/// <summary>
		/// Sets the current renderer manager to a new renderer manager.
		/// </summary>
		/// <param name="rendererMaster"> The new renderer manager. </param>
		void SetManager(IManagerRender *managerRender) { m_managerRender = managerRender; }

		std::vector<RenderStage *> GetRenderStages() const { return m_renderStages; }

		RenderStage *GetRenderStage(const int &i) const { return m_renderStages.at(i); }

		Swapchain *GetSwapchain() const { return m_swapchain; }

		VkCommandPool GetVkCommandPool() const { return m_commandPool; }

		CommandBuffer *GetCommandBuffer() const { return m_commandBuffer; }

		uint32_t GetVkActiveSwapchainImage() const { return m_activeSwapchainImage; }

		VkPipelineCache GetVkPipelineCache() const { return m_pipelineCache; }
	private:
		void CreateFences();

		void CreateCommandPool();

		void CreatePipelineCache();

		void RecreatePass(const int &i);
	};
}