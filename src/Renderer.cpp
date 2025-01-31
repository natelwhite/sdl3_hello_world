#include "Renderer.hpp"
bool Renderer::init() { 
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return false;
	}

	m_device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, NULL);
	if (m_device == nullptr) {
		SDL_Log("CreateGPUDevice failed: %s", SDL_GetError());
		return false;
	}

	m_window = SDL_CreateWindow("Hello World", m_width, m_height, m_windowFlags);
	if (m_window == nullptr) {
		SDL_Log("CreateWindow failed: %s", SDL_GetError());
		return false;
	}

	if (!SDL_ClaimWindowForGPUDevice(m_device, m_window)) {
		SDL_Log("ClaimWindowForGPUDevice failed: %s", SDL_GetError());
		return false;
	}

	m_base_path = SDL_GetBasePath();
	return true;
}

SDL_GPUShader* Renderer::loadShader(const char* filename, Uint32 num_samplers, Uint32 num_uniform_buffers, Uint32 num_storage_buffers, Uint32 num_storage_textures){
	// determine if vertex or fragment shader
	SDL_ShaderCross_ShaderStage shader_stage;
	if (SDL_strstr(filename, ".vert")) {
		shader_stage = SDL_SHADERCROSS_SHADERSTAGE_VERTEX;
	} else if (SDL_strstr(filename, ".frag")) {
		shader_stage = SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT;
	} else {
		SDL_Log("Invalid shader stage"); 
		return nullptr;
	}

	// get the full path to the shader given it's name
	char full_path[256];
	SDL_snprintf(full_path, sizeof(full_path), "%s%s%s.hlsl", m_base_path, m_shaders_location, filename);
	// force vulkan format for now
	SDL_GPUShaderFormat device_formats = SDL_GetGPUShaderFormats(m_device);
	SDL_GPUShaderFormat mutual_format = SDL_GPU_SHADERFORMAT_SPIRV;
	if (!(device_formats & SDL_GPU_SHADERFORMAT_SPIRV)) {
		SDL_Log("Unrecognized GPUDevice shader format");
		return nullptr;
	} 

	// get shader code
	const char *entrypoint = "main";
	size_t code_size;
	void *code = SDL_LoadFile(full_path, &code_size);
	if (code == NULL) {
		SDL_Log("Failed to load shader from disk: %s", full_path);
		return nullptr;
	}

	// create info & metadata for parsing hlsl to compile during runtime
	SDL_ShaderCross_HLSL_Info hlsl_info {
		.source = static_cast<const char*>(code),
		.entrypoint = entrypoint,
		.include_dir = NULL,
		.defines = NULL,
		.shader_stage = shader_stage,
		.enable_debug = true,
		.name = NULL,
		.props = 0
	};
	SDL_ShaderCross_GraphicsShaderMetadata metadata {
		.num_samplers = num_samplers,
		.num_storage_textures = num_storage_textures,
		.num_storage_buffers = num_storage_buffers,
		.num_uniform_buffers = num_uniform_buffers
	};

	// compile hlsl to spv
	SDL_GPUShader *result = SDL_ShaderCross_CompileGraphicsShaderFromHLSL(m_device, &hlsl_info, &metadata);
	SDL_free(code);
	if (result == nullptr) {
		SDL_Log("Failed to create shader: %s", SDL_GetError());
		return nullptr;
	}
	return result;
} 

SDL_GPUGraphicsPipeline* Renderer::createGraphicsPipeline(SDL_GPUShader *vert_shader, SDL_GPUShader *frag_shader){
	const SDL_GPUColorTargetBlendState target_blend_state {
		// color
		.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
		.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
		// alpha
		.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
		.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
	};
	const SDL_GPUColorTargetDescription target_description {
		.format = SDL_GetGPUSwapchainTextureFormat(m_device, m_window),
		.blend_state = target_blend_state
	};
	const SDL_GPUGraphicsPipelineTargetInfo target_info {
		.color_target_descriptions = &target_description,
		.num_color_targets = 1,
	};
	const SDL_GPUGraphicsPipelineCreateInfo pipeline_info {
		.vertex_shader = vert_shader,
		.fragment_shader = frag_shader,
		.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
		.rasterizer_state = { SDL_GPU_FILLMODE_FILL },
		.target_info = target_info,
	};
	SDL_GPUGraphicsPipeline *result = SDL_CreateGPUGraphicsPipeline(m_device, &pipeline_info);
	SDL_ReleaseGPUShader(m_device, vert_shader);
	SDL_ReleaseGPUShader(m_device, frag_shader);
	if (result == nullptr) {
		SDL_Log("CreateGPUGraphicsPipeline failed: %s", SDL_GetError());
		return nullptr;
	}
	return result;
}

void Renderer::draw(SDL_GPUGraphicsPipeline *pipeline) {
	SDL_GPUCommandBuffer *cmdbuf = SDL_AcquireGPUCommandBuffer(m_device);
	if (cmdbuf == nullptr) {
		SDL_Log("AcquireGPUCommandBuffer failed%s", SDL_GetError());
		return;
	}
	SDL_GPUTexture *swapchain_texture;
	if (!SDL_WaitAndAcquireGPUSwapchainTexture(cmdbuf, m_window, &swapchain_texture, NULL, NULL)) {
		SDL_Log("WaitAndAcquireGPUSwapchainTexture failed: %s", SDL_GetError());
		return;
	}
	SDL_GPUColorTargetInfo target_info {
		.texture = swapchain_texture,
		.clear_color = (SDL_FColor){0.0f, 0.0f, 0.0f, 1.0f},
		.load_op = SDL_GPU_LOADOP_CLEAR,
		.store_op = SDL_GPU_STOREOP_STORE
	};
	SDL_GPURenderPass *render_pass = SDL_BeginGPURenderPass(cmdbuf, &target_info, 1, NULL);
	SDL_BindGPUGraphicsPipeline(render_pass, pipeline);
	SDL_DrawGPUPrimitives(render_pass, 3, 1, 0, 0);
	SDL_EndGPURenderPass(render_pass);
	SDL_SubmitGPUCommandBuffer(cmdbuf);
}

void Renderer::quit() {
	SDL_ReleaseWindowFromGPUDevice(m_device, m_window);
	SDL_DestroyWindow(m_window);
	SDL_DestroyGPUDevice(m_device);
}
