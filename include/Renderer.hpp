#pragma once
#include <SDL3/SDL.h>
#include <SDL3_shadercross/SDL_shadercross.h>

class Renderer {
	public:
		Renderer(const int &t_width, const int &t_height) : m_width(t_width), m_height(t_height) { }
		bool init();
		bool compileShader(const char *filename);
		SDL_GPUShader* loadShaderBinary(const char *filename, SDL_ShaderCross_GraphicsShaderMetadata *metadata);
		SDL_GPUShader* loadShaderSource(const char* filename, SDL_ShaderCross_GraphicsShaderMetadata *metadata);
		SDL_GPUGraphicsPipeline *createGraphicsPipeline(SDL_GPUShader *vert_shader, SDL_GPUShader *frag_shader);
		void releaseShader(SDL_GPUShader *shader);
		void releaseGraphicsPipeline(SDL_GPUGraphicsPipeline *pipelin);
		void draw(SDL_GPUGraphicsPipeline *pipeline);
		void quit();
	private:
		Uint32 m_width { 600 }, m_height { 400 }; // window width & height
		const SDL_WindowFlags m_windowFlags = SDL_WINDOW_RESIZABLE;
		SDL_GPUShaderFormat m_mutual_format { SDL_GPU_SHADERFORMAT_INVALID };
		const SDL_GPUShaderFormat m_accepted_shader_formats[3] {
			SDL_GPU_SHADERFORMAT_SPIRV,
			SDL_GPU_SHADERFORMAT_DXBC,
			SDL_GPU_SHADERFORMAT_DXIL,
		};
		// for locating shader directories
		const char *m_root { SDL_GetBasePath() }, *m_hlsl { "shaders/source/" }, *m_binaries { "shaders/compiled/" };

		SDL_GPUDevice *m_gpu { nullptr };
		SDL_Window *m_window { nullptr };
};

