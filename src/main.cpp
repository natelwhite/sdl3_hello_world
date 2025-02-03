#include "Renderer.hpp"

int main() {
	Renderer renderer {640, 480};
	if (!renderer.init()) {
		return -1;
	}
	SDL_ShaderCross_GraphicsShaderMetadata metadata {
		.num_samplers = 0,
		.num_storage_textures = 0,
		.num_storage_buffers = 0,
		.num_uniform_buffers = 0
	};
	SDL_GPUShader *vert_shader = renderer.loadShaderSource("RawTriangle.vert", &metadata);
	if (vert_shader == nullptr) {
		return -1;
	}
	SDL_GPUShader *frag_shader = renderer.loadShaderSource("SolidColor.frag", &metadata);
	if (frag_shader == nullptr) {
		return -1;
	}

	SDL_GPUGraphicsPipeline *pipeline = renderer.createGraphicsPipeline(vert_shader, frag_shader);
	if (pipeline == nullptr) {
		return -1;
	}

	// no longer needed by gpu after making pipeline
	renderer.releaseShader(vert_shader);
	renderer.releaseShader(frag_shader);

	// main loop
	bool quit = false;
	while (!quit) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch(e.type) {
			case SDL_EVENT_QUIT:
				quit = true;
				break;
			case SDL_EVENT_KEY_DOWN:
				switch(e.key.key) {
				case SDLK_ESCAPE:
					quit = true;
					break;
				case SDLK_R: {
					vert_shader = renderer.loadShaderSource("RawTriangle.vert", &metadata);
					frag_shader = renderer.loadShaderSource("SolidColor.frag", &metadata);
					if (vert_shader == nullptr) {
						SDL_Log("Could not reload vertex shader: %s", SDL_GetError());
						continue;
					}
					if (frag_shader == nullptr) {
						SDL_Log("Could not reload fragment shader: %s", SDL_GetError());
						continue;
					}
					SDL_GPUGraphicsPipeline *new_pipeline = renderer.createGraphicsPipeline(vert_shader, frag_shader);
					if (new_pipeline == nullptr) {
						SDL_Log("Could not create graphics pipeline: %s", SDL_GetError());
						continue;
					}

					renderer.releaseShader(vert_shader);
					renderer.releaseShader(frag_shader);
					renderer.releaseGraphicsPipeline(pipeline);
					pipeline = new_pipeline;
					break;
				}
				case SDLK_C: {
					renderer.compileShader("RawTriangle.vert");
					renderer.compileShader("SolidColor.frag");
					vert_shader = renderer.loadShaderBinary("RawTriangle.vert", &metadata);
					frag_shader = renderer.loadShaderBinary("SolidColor.frag", &metadata);
					if (vert_shader == nullptr) {
						SDL_Log("Could not reload vertex shader: %s", SDL_GetError());
						continue;
					}
					if (frag_shader == nullptr) {
						SDL_Log("Could not reload fragment shader: %s", SDL_GetError());
						continue;
					}
					SDL_GPUGraphicsPipeline *new_pipeline = renderer.createGraphicsPipeline(vert_shader, frag_shader);
					if (new_pipeline == nullptr) {
						SDL_Log("Could not create graphics pipeline: %s", SDL_GetError());
						continue;
					}

					renderer.releaseShader(vert_shader);
					renderer.releaseShader(frag_shader);
					renderer.releaseGraphicsPipeline(pipeline);
					pipeline = new_pipeline;
					SDL_Log("Successfully compiled vertex and fragment shader");
					break;
				}
				}
			}
		}
		renderer.draw(pipeline);
	}
	renderer.releaseGraphicsPipeline(pipeline);
	renderer.quit();
	return 0;
}
