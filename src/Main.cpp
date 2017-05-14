#include "Graphics/Renderer_Vulkan.h"

#include <chrono>
#include <iostream>

static unsigned long long _fps = 0, _tps = 0;

int main(int argc, char **argv) {
	try {
		bool running = true;

		std::unique_ptr<ProjectVoxel::Graphics::Renderer> renderer =
				std::make_unique<ProjectVoxel::Graphics::Vulkan::Renderer>();


		renderer->GetWindow()->AddCloseCallback([&running](const ProjectVoxel::Graphics::Window &window) {
			running = false;
		});

		renderer->GetWindow()->SetVisible(true);

		auto lastFrame = std::chrono::high_resolution_clock::now();
		auto lastTick = std::chrono::high_resolution_clock::now();
		auto lastSecond = std::chrono::high_resolution_clock::now();
		while (running) {
			auto now = std::chrono::high_resolution_clock::now();

			auto delta = now - lastTick;
			while (delta >= std::chrono::seconds(1) / 20.0) {
				renderer->GetWindow()->HandleEvents();

				_tps++;
				lastTick = now;
				delta -= std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1) / 20.0);
			}

			delta = now - lastFrame;
			if (delta >= std::chrono::seconds(1) / 60.0) {
				renderer->SwapBuffers();

				_fps++;
				lastFrame = now;
			}

			delta = now - lastSecond;
			if (delta >= std::chrono::seconds(1)) {
				std::cout << "FPS: " << _fps << "\nTPS: " << _tps << std::endl;

				_fps = _tps = 0;
				lastSecond = now;
			}
		}
	}
	catch (char *e) {
		std::cerr << e << std::endl;
		char c;
		std::cin >> c;
	}
	catch (std::string e) {
		std::cerr << e << std::endl;
		char c;
		std::cin >> c;
	}
}