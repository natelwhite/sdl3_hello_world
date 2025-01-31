#include "Renderer.cpp"
#include <SDL3/SDL.h>

int main() {
	Renderer renderer {640, 480};
	if (!renderer.init()) {
		return -1;
	}


	SDL_GPUShader *vert_shader = renderer.loadShader("RawTriangle.vert", 0, 0, 0, 0);
	if (vert_shader == nullptr) {
		return -1;
	}
	SDL_GPUShader *frag_shader = renderer.loadShader("SolidColor.frag", 0, 0, 0, 0);
	if (vert_shader == nullptr) {
		return -1;
	}

	SDL_GPUGraphicsPipeline *pipeline = renderer.createGraphicsPipeline(vert_shader, frag_shader);
	if (pipeline == nullptr) {
		return -1;
	}
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
						case SDLK_R:
							SDL_GPUShader *new_vert_shader = renderer.loadShader("RawTriangle.vert", 0, 0, 0, 0);
							SDL_GPUShader *new_frag_shader = renderer.loadShader("SolidColor.frag", 0, 0, 0, 0);
							if (new_vert_shader == nullptr) {
								SDL_Log("Could not reload vertex shader: %s", SDL_GetError());
								continue;
							}
							if (new_frag_shader == nullptr) {
								SDL_Log("Could not reload fragment shader: %s", SDL_GetError());
								continue;
							}
							SDL_GPUGraphicsPipeline *new_pipeline = renderer.createGraphicsPipeline(new_vert_shader, new_frag_shader);
							if (new_pipeline == nullptr) {
								SDL_Log("Could not create graphics pipeline: %s", SDL_GetError());
								continue;
							}

							pipeline = new_pipeline;
							break;
					}
			}
		}
		renderer.draw(pipeline);
	}
	renderer.quit();
	return 0;
}
