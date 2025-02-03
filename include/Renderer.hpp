#pragma once
#include <SDL3/SDL.h>
#include <SDL3_shadercross/SDL_shadercross.h>

class Renderer {
	public:
		Renderer(const int &t_width, const int &t_height) : m_width(t_width), m_height(t_height) { }
		bool init();
		SDL_GPUShader* loadShader(const char* filename, Uint32 num_samplers, Uint32 num_uniform_buffers, Uint32 num_storage_buffers, Uint32 num_storage_textures);
		SDL_GPUGraphicsPipeline *createGraphicsPipeline(SDL_GPUShader *vert_shader, SDL_GPUShader *frag_shader);
		void releaseShader(SDL_GPUShader *shader);
		void releaseGraphicsPipeline(SDL_GPUGraphicsPipeline *pipelin);
		void draw(SDL_GPUGraphicsPipeline *pipeline);
		void quit();
	private:
		Uint32 m_width { 600 }, m_height { 400 }; // window width & height
		const SDL_WindowFlags m_windowFlags = SDL_WINDOW_RESIZABLE;
		const SDL_GPUShaderFormat m_accepted_shader_formats[4] {
			SDL_GPU_SHADERFORMAT_SPIRV,
			SDL_GPU_SHADERFORMAT_DXBC,
			SDL_GPU_SHADERFORMAT_DXIL,
			SDL_GPU_SHADERFORMAT_MSL
		};
		// for locating shader directories
		const char *m_root { SDL_GetBasePath() }, *m_hlsl { "shaders/source/" };

		SDL_GPUDevice *m_gpu { nullptr };
		SDL_Window *m_window { nullptr };
};

