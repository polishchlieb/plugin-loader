#include <Windows.h>
#include <iostream>
#include <thread>
#include <filesystem>
#include <functional>

typedef void(__stdcall* loadFunction)();
typedef void(__stdcall* runFunction)(bool& finished);
typedef void(__stdcall* unloadFunction)();

struct plugin {
	std::thread thread;
	unloadFunction unload;
};

std::vector<plugin> loadPlugins(bool& finished) {
	std::vector<plugin> plugins;

	for (const auto& entry : std::filesystem::directory_iterator("plugins")) {
		const std::string name = entry.path().filename().string();
		std::cout << "Loading " << name << ".." << std::endl;

		HINSTANCE handle = LoadLibrary((LPCWSTR) entry.path().string().c_str());
		if (!handle) {
			std::cout << "Couldn't load " << name << std::endl; 
			continue;
		}

		// assume the library has exported: load, run, unload
		loadFunction load = (loadFunction)GetProcAddress(handle, "load");
		runFunction run = (runFunction)GetProcAddress(handle, "run");
		unloadFunction unload = (unloadFunction)GetProcAddress(handle, "unload");

		load();
		plugins.push_back({ std::thread(run, std::ref(finished)), unload });
	}

	return plugins;
}

int main() {
	bool finished = false;
	auto plugins = loadPlugins(finished);

	std::cin.get();

	finished = true;
	for (auto& plugin : plugins) {
		plugin.thread.join();
		plugin.unload();
	}
	return 0;
}